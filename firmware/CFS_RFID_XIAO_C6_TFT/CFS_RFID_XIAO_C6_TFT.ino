// CFS RFID + Creality Printer Push Agent - single-board XIAO ESP32-C6 merge
// Release baseline: v1.0.0 (same operating logic as tested v0.3.9)
// v0.1 proof build generated from the user's working Spool_ID RFID firmware
// and working K2_Push_Agent firmware.
//
// AP mode remains explicitly controlled by AP_ENABLED in the web settings.
// The second-board C6 hostname setting has been removed; browser sync/push
// targets this same device on port 8081.
//
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <Update.h>
#include <MD5Builder.h>
#include "libssh_esp32.h"
#include <libssh/libssh.h>
#include "src/includes.h"
#include "display_material_names.h"
#include "cfs_display_logo.h"
#include "cfs_action_icons.h"
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

// Seeed Studio XIAO ESP32-C6 + RC522 wiring
// D8/GPIO19  -> RC522 SCK
// D9/GPIO20  -> RC522 MISO
// D10/GPIO18 -> RC522 MOSI
// D3/GPIO21  -> RC522 SDA/SS (chip select)
// D2/GPIO2   -> RC522 RST
// D1/GPIO1   -> buzzer/speaker signal
#define RFID_SCK_PIN   19
#define RFID_MISO_PIN  20
#define RFID_MOSI_PIN  18
#define RFID_SS_PIN    21
#define RFID_RST_PIN    2
#define SPK_PIN         1
#define TFT_CS_PIN     22 // D4
#define TFT_DC_PIN     23 // D5
#define TFT_RST_PIN    16 // D6

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);
Adafruit_ST7789 tft(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::MIFARE_Key ekey;
WebServer webServer(80);
AES aes;
File upFile;
String upMsg;
bool saveDbFailed = false;
MD5Builder md5;

IPAddress Server_IP(10, 1, 0, 1);
IPAddress Subnet_Mask(255, 255, 255, 0);
String spoolData = "AB1240276A210100100000FF016500000100000000000000";
String AP_SSID = "CFS_RFID_Setup";
String AP_PASS = "CHANGE_ME_123";
String WIFI_SSID = "";
String WIFI_PASS = "";
String WIFI_HOSTNAME = "cfs-rfid";
String PRINTER_HOSTNAME = "";
bool AP_ENABLED = true;

// Printer push-agent settings. The printer host itself is user-configurable
// through PRINTER_HOSTNAME and is loaded from /config.ini.
const int   PRINTER_PORT = 22;
const char *PRINTER_USER = "root";
const char *PRINTER_PASS = "SET_YOUR_PRINTER_PASSWORD";
const int   RAW_SERVER_PORT = 8081;
const bool  PRINTER_IS_K1 = false;
const char *REMOTE_DIR_K1 = "/usr/data/creality/userdata/box/";
const char *REMOTE_DIR_OTHER = "/mnt/UDISK/creality/userdata/box/";
const char *REMOTE_FILENAME = "material_database.json";

// XIAO ESP32-C6 RF switch. These pins are internal to the board and must
// not be reused by the RC522 when the external U.FL antenna is selected.
const bool USE_EXTERNAL_ANTENNA = false;
const int ANT_SWITCH_POWER_PIN = 3;
const int ANT_SWITCH_SELECT_PIN = 14;

const unsigned int SSH_TASK_STACK = 40960;
WiFiServer rawServer(RAW_SERVER_PORT);
static SemaphoreHandle_t jobReadySem;
static SemaphoreHandle_t jobDoneSem;
static bool jobResultOk = false;
static String jobResultMsg;
static bool jobIsTest = false;
static String jobTestCommand;
static String jobTestOutput;
bool encrypted = false;
// Only user-armed write operations may modify a tag. Default is passive read.
enum class RfidAction : uint8_t { Passive, Read, Write };
static RfidAction rfidAction = RfidAction::Passive;
static bool writeSelectionApplied = false;
static bool tagHeld = false;
static String heldUid;
static unsigned long nextPresenceCheck = 0;
static uint8_t absentChecks = 0;
static unsigned long previewUntil = 0;
bool readArmed = false;
bool readReady = false;
bool readError = false;
String readTagId = "";
String readMaterialType = "";
String readColor = "";
String readWeight = "";
String readPrinterType = "";
unsigned long tagCooldownUntil = 0;


// -----------------------------------------------------------------------------
// ST7789: optional status display, deliberately separate from RFID operations.
// Draw only from loop(), after MIFARE transactions have completed. The display
// shares SPI SCK/MOSI with RC522 but has its own chip select.
// -----------------------------------------------------------------------------
static const uint16_t SCREEN_BG=0x1082, SCREEN_CARD=0x2145, SCREEN_BLUE=0x2CBB;
static const uint16_t SCREEN_WHITE=0xFFFF, SCREEN_MUTED=0xAD55;
static const uint16_t SCREEN_GREEN=0x57D4, SCREEN_RED=0xF965;
static bool displayReady = false;
static bool displayDirty = false;
// A non-blocking 1.7 s operation result overlay.
static constexpr uint32_t RESULT_SPLASH_MS = 1700;
static uint32_t resultSplashStarted = 0;
static bool resultSplashActive = false;
static bool resultSplashSuccess = false;
static String resultSplashLabel;
static void textCentered(const String &txt,int centerX,int baseY,const GFXfont *font,uint16_t color);
static void drawResultSplash() {
  const uint16_t accent = resultSplashSuccess ? 0x262B : 0xD1E7;
  digitalWrite(RFID_SS_PIN,HIGH);
  tft.fillScreen(SCREEN_BG);
  tft.fillRoundRect(8,8,224,224,12,SCREEN_CARD);
  // Single solid badge: no low-resolution circular outline.
  tft.fillCircle(120,89,46,accent);
  if(resultSplashSuccess) {
    for(int k=-4;k<=4;k++) {
      tft.drawLine(95,91+k,112,107+k,SCREEN_WHITE);
      tft.drawLine(112,107+k,146,74+k,SCREEN_WHITE);
    }
  } else {
    for(int k=-4;k<=4;k++) {
      tft.drawLine(101+k,70,139+k,108,SCREEN_WHITE);
      tft.drawLine(139+k,70,101+k,108,SCREEN_WHITE);
    }
  }
  textCentered(resultSplashSuccess?"SUCCESS":"FAILED",120,172,&FreeSansBold12pt7b,SCREEN_WHITE);
  textCentered(resultSplashLabel,120,201,&FreeSans9pt7b,SCREEN_MUTED);
  digitalWrite(TFT_CS_PIN,HIGH);
}
static void showResultSplash(bool success,const String &label) {
  resultSplashSuccess=success;
  resultSplashLabel=label;
  resultSplashStarted=millis();
  resultSplashActive=true;
  displayDirty=true;
}
static String displayMode="READY", displayUid="", displayMaterial="", displayBrand="", displayWeight="";
static String selectedBrand="", selectedMaterial="", selectedColor="0000FF", selectedWeight="1 KG";
static String lastReadMaterial="", lastReadBrand="", lastReadColor="", lastReadWeight="";
static uint32_t displayColor = 0x0000FF;

static String screenTrim(const String &v, size_t maxChars) {
  if(v.length()<=maxChars)return v;
  return v.substring(0,maxChars>2?maxChars-2:0)+"..";
}
static uint16_t rgb565(const String &hex) {
  if(hex.length()!=6)return SCREEN_CARD;
  uint32_t n=strtoul(hex.c_str(),nullptr,16);
  return (uint16_t)(((n>>19)&0x1F)<<11 | ((n>>10)&0x3F)<<5 | ((n>>3)&0x1F));
}
// Transfer state is only display telemetry received from the web UI.
// No printer database, SSH or SCP routine reads or writes these variables.
static String tftTransferMode;
static uint32_t tftTransferLastSeen=0;
static void paintLogo() {
  for (int y=0;y<30;y++) for(int x=0;x<30;x++) {
    uint16_t color=pgm_read_word(&CFS_LOGO_30[y*30+x]);
    if(color) tft.drawPixel(13+x,12+y,color);
  }
}
static void textCentered(const String &txt,int centerX,int baseY,const GFXfont *font,uint16_t color) {
  tft.setFont(font);tft.setTextColor(color);
  int16_t x1,y1;uint16_t w,h;
  tft.getTextBounds(txt,0,0,&x1,&y1,&w,&h);
  tft.setCursor(centerX-(int)w/2,baseY);tft.print(txt);
  tft.setFont(NULL);
}
static String elide(const String &txt,int limit) {
  if(txt.length()<=limit)return txt;
  return txt.substring(0,limit-2)+"..";
}
static void headerDisplay(bool ready) {
  tft.fillRoundRect(8,8,224,224,10,SCREEN_CARD);
  tft.fillRoundRect(8,8,224,45,9,SCREEN_BLUE);
  tft.fillRect(8,36,224,17,SCREEN_BLUE);
  // Balanced left and right 38 px icon zones; title at true screen centre.
  paintLogo();
  textCentered("CFS RFID",120,37,&FreeSans9pt7b,SCREEN_WHITE);
  tft.fillCircle(207,30,9,ready?SCREEN_GREEN:SCREEN_RED);
}
static void drawPromptIcon(bool writing) {
  // An antialiased icon bitmap replaces the old low-resolution line drawing.
  // The pre-rendered square fits entirely inside the filled 51px badge.
  const uint16_t badge = writing ? 0xFCA0 : SCREEN_BLUE;
  tft.fillCircle(120,106,51,badge);
  tft.drawRGBBitmap(87,73,writing?CFS_WRITE_ICON_66:CFS_READ_ICON_66,66,66);
}
static void drawTransferNotice() {
  headerDisplay(false);
  bool pushing=tftTransferMode=="push";
  // Solid enclosed FDM printer silhouette, sized for 240x240 rather than
  // thin-outline office-printer or wireframe artwork.
  const uint16_t frame=0x94B2, inside=SCREEN_CARD, bright=SCREEN_WHITE;
  tft.fillRoundRect(70,67,100,90,11,frame);
  tft.fillRoundRect(77,74,86,76,6,inside);
  tft.fillRoundRect(85,81,70,8,3,bright);           // gantry
  tft.fillRoundRect(92,89,5,44,2,frame);            // rail
  tft.fillRoundRect(143,89,5,44,2,frame);
  tft.fillRoundRect(112,92,17,20,3,bright);         // toolhead
  tft.fillTriangle(117,112,124,112,120,120,bright);// nozzle
  tft.fillRoundRect(92,132,56,8,3,bright);         // bed
  tft.fillRoundRect(84,144,72,5,2,frame);          // base
  // Download (pull) goes down; upload (push) goes up.
  const uint16_t arrow=pushing?SCREEN_GREEN:SCREEN_BLUE;
  tft.fillRoundRect(177,89,11,35,3,arrow);
  if(pushing) tft.fillTriangle(171,94,194,94,182,78,arrow);
  else tft.fillTriangle(171,120,194,120,182,137,arrow);
  textCentered(pushing?"PUSHING":"SYNCING",120,178,&FreeSansBold12pt7b,SCREEN_WHITE);
  textCentered("Please wait",120,203,&FreeSans9pt7b,SCREEN_WHITE);
  tft.setFont(NULL);tft.setTextSize(1);tft.setTextColor(SCREEN_MUTED);
  textCentered("Do not scan tags",120,223,&FreeSans9pt7b,SCREEN_MUTED);
}
static void drawDisplay() {
  if(!displayReady)return;
  if(resultSplashActive && (uint32_t)(millis()-resultSplashStarted)>=RESULT_SPLASH_MS){
    resultSplashActive=false;displayDirty=true;
  }
  // Guard against a browser disappearing mid-transfer. This ONLY clears the
  // visual busy banner, and does not alter any printer/database operation.
  if(tftTransferMode.length() && millis()-tftTransferLastSeen>90000){
    tftTransferMode="";displayDirty=true;
  }
  if(!displayDirty)return;
  displayDirty=false;
  digitalWrite(RFID_SS_PIN,HIGH);
  tft.fillScreen(SCREEN_BG);
  if(tftTransferMode.length()) {drawTransferNotice();digitalWrite(TFT_CS_PIN,HIGH);return;}
  if(resultSplashActive){drawResultSplash();return;}
  headerDisplay(true);
  if(displayMode=="READY TO READ" || displayMode=="READY TO WRITE") {
    bool writing=displayMode=="READY TO WRITE";
    drawPromptIcon(writing);
    textCentered(writing?"WRITE TAG":"READ TAG",120,185,&FreeSansBold12pt7b,SCREEN_WHITE);
    textCentered(writing?"Present tag to write":"Present tag to read",120,214,&FreeSans9pt7b,SCREEN_MUTED);
    digitalWrite(TFT_CS_PIN,HIGH);return;
  }
  bool isRead=displayMode=="TAG READ" || displayMode=="PASSIVE READ";
  bool isError=displayMode=="READ ERROR" || displayMode=="RFID ERROR" || displayMode=="WRITE ERROR";
  String brand=isRead?lastReadBrand:selectedBrand;
  String material=isRead?lastReadMaterial:selectedMaterial;
  String color=isRead?lastReadColor:selectedColor;
  if(!brand.length())brand="Filament";
  if(!material.length())material="Select in browser";
  textCentered(elide(brand,19),120,78,&FreeSansBold12pt7b,SCREEN_WHITE);
  // Two rows reserved for long product/style names.
  String first=material, second="";
  if(material.length()>17) {
    int split=material.lastIndexOf(' ',17);
    if(split<6)split=17;
    first=material.substring(0,split);
    second=material.substring(split);second.trim();
  }
  textCentered(elide(first,20),120,106,&FreeSansBold12pt7b,SCREEN_WHITE);
  if(second.length())textCentered(elide(second,20),120,131,&FreeSans9pt7b,SCREEN_WHITE);
  uint16_t swatch=rgb565(color);
  tft.fillCircle(120,159,29,swatch);
  color.trim();color.replace("#", "");color.toUpperCase();
  String hex="#"+color;
  textCentered(hex,120,207,&FreeSans9pt7b,SCREEN_WHITE);
  String footer = (isRead && displayUid.length())
    ? ("UID " + displayUid)
    : String("Use web UI for read/write");
  textCentered(footer,120,225,&FreeSans9pt7b,isError?SCREEN_RED:SCREEN_MUTED);
  digitalWrite(TFT_CS_PIN,HIGH);
}
static void handleTftTransfer() {
  // Display-only signal from existing browser controls. Never starts an SSH
  // session, accesses a database, or changes RFID mode.
  String mode=webServer.arg("mode");
  if(mode=="pull"||mode=="push")tftTransferMode=mode;
  else if(mode=="done")tftTransferMode="";
  else {webServer.send(400,"text/plain","Invalid display mode");return;}
  tftTransferLastSeen=millis();displayDirty=true;
  webServer.send(200,"text/plain","OK");
}
static void screenStatus(const String &mode) {
  displayMode=mode;
  displayDirty=true;
  if(mode=="TAG READ" || mode=="PASSIVE READ")showResultSplash(true,"Tag read successfully");
  else if(mode=="WRITE COMPLETE")showResultSplash(true,"Tag written");
  else if(mode=="READ ERROR")showResultSplash(false,"Tag read failed");
  else if(mode=="WRITE ERROR")showResultSplash(false,"Tag write failed");
  else if(mode=="RFID ERROR")showResultSplash(false,"RFID communication");
  else resultSplashActive=false;
}
static void screenInit() {
  // The working standalone test initialized RC522 before this display.
  digitalWrite(RFID_SS_PIN,HIGH);
  tft.init(240,240);tft.setRotation(0);
  displayReady=true;displayDirty=true;drawDisplay();
  uint8_t ver=mfrc522.PCD_ReadRegister(MFRC522::VersionReg);
  uint8_t tx=mfrc522.PCD_ReadRegister(MFRC522::TxControlReg);
  Serial.printf("After TFT: RC522 VersionReg=0x%02X TxControlReg=0x%02X\\n",ver,tx);
  if(ver==0 || ver==0xFF)screenStatus("RFID ERROR");
}
static void handleDisplayResolve() {
  // Optional label sent by browser once it resolves a scanned ID locally.
  // Only presentation changes; it never alters spoolData or tag state.
  if(displayMode=="TAG READ" && webServer.arg("materialId")==readMaterialType) {
    lastReadBrand=webServer.arg("brand").substring(0,48);
    lastReadMaterial=webServer.arg("name").substring(0,48);
    // The browser may resolve the name while the 1.7 s read-success
    // overlay is showing. Store the new label but do not redraw/restart
    // that overlay; its expiry already schedules the normal view redraw.
    if (!resultSplashActive) displayDirty=true;
  }
  webServer.send(200,"text/plain","OK");
}

// DISPLAY-ONLY material label cache. Never accesses printer/DB transfer code.
// The web UI publishes a slim index from the material list already in its memory.
static const char *DISPLAY_INDEX_FILE="/display_labels.txt";
static const char *DISPLAY_INDEX_TMP="/display_labels.tmp";
static bool displayIndexUploading=false;
static uint16_t displayIndexRows=0;
static const uint16_t DISPLAY_INDEX_MAX_ROWS=500;
static String displayCacheSafe(String s, size_t limit) {
  s.replace("\t", " "); s.replace("\r", " "); s.replace("\n", " ");
  if(s.length()>limit)s.remove(limit);
  return s;
}
static bool displayCachedLabel(const String &id, String &brand, String &name) {
  File f=LittleFS.open(DISPLAY_INDEX_FILE,"r");
  if(!f)return false;
  bool found=false;
  while(f.available()) {
    String line=f.readStringUntil('\n');
    int a=line.indexOf('\t'), b=line.indexOf('\t',a+1);
    if(a>0 && b>a && line.substring(0,a)==id) {
      brand=line.substring(a+1,b);
      name=line.substring(b+1); name.trim();
      found=name.length()>0;
      break;
    }
    yield();
  }
  f.close();
  return found;
}
static void handleDisplayCache() {
  // Presentation metadata only. Never touches /matdb.json, printer, or spoolData.
  String op=webServer.arg("op");
  if(op=="begin") {
    File f=LittleFS.open(DISPLAY_INDEX_TMP,"w");
    if(!f){webServer.send(507,"text/plain","Cannot open display index");return;}
    f.close(); displayIndexUploading=true;displayIndexRows=0;
    webServer.send(200,"text/plain","OK"); return;
  }
  if(!displayIndexUploading){webServer.send(409,"text/plain","No active index update");return;}
  if(op=="append") {
    String rows=webServer.arg("rows");
    if(rows.length()>1400){webServer.send(413,"text/plain","Batch too large");return;}
    File f=LittleFS.open(DISPLAY_INDEX_TMP,"a");
    if(!f){webServer.send(507,"text/plain","Display index append failed");return;}
    int pos=0;
    while(pos<(int)rows.length() && displayIndexRows<DISPLAY_INDEX_MAX_ROWS) {
      int end=rows.indexOf('\n',pos);
      if(end<0)end=rows.length();
      String line=rows.substring(pos,end);
      int a=line.indexOf('\t'),b=line.indexOf('\t',a+1);
      if(a>0 && b>a) {
        String id=displayCacheSafe(line.substring(0,a),8);
        String brand=displayCacheSafe(line.substring(a+1,b),40);
        String name=displayCacheSafe(line.substring(b+1),60);
        bool valid=id.length()==5;
        for(size_t i=0;i<id.length();i++)if(!isDigit(id[i]))valid=false;
        if(valid && name.length()) {f.printf("%s\t%s\t%s\n",id.c_str(),brand.c_str(),name.c_str());displayIndexRows++;}
      }
      pos=end+1;
    }
    f.close(); webServer.send(200,"text/plain","OK");return;
  }
  if(op=="commit") {
    displayIndexUploading=false;
    if(displayIndexRows==0){LittleFS.remove(DISPLAY_INDEX_TMP);webServer.send(422,"text/plain","Empty display index");return;}
    // Only the independent display cache file is replaced.
    LittleFS.remove(DISPLAY_INDEX_FILE);
    if(!LittleFS.rename(DISPLAY_INDEX_TMP,DISPLAY_INDEX_FILE)){
      webServer.send(500,"text/plain","Index rename failed");return;
    }
    Serial.printf("Display label cache updated: %u records\n",displayIndexRows);
    webServer.send(200,"text/plain","OK");return;
  }
  webServer.send(400,"text/plain","Unsupported cache operation");
}

static void handleDisplayStatus() {
  // Read-only convenience endpoint, no changes to existing handlers.
  String j="{\"mode\":\""+displayMode+"\",\"selectedColor\":\""+selectedColor+"\"}";
  webServer.send(200,"application/json",j);
}

// waitForConnectResult() can time out and return before the SDK actually
// finishes associating + getting a DHCP lease, especially right after a
// cold boot - the connection then often completes a moment later in the
// background, which is confusing (Serial says FAILED, router says
// connected). Poll status directly with a longer window instead.
static bool waitForStaConnect(unsigned long timeoutMs)
{
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  return WiFi.status() == WL_CONNECTED;
}

// -----------------------------------------------------------------------------
// Integrated printer push agent (SSH/SCP)
// -----------------------------------------------------------------------------
static const char *remoteDir()
{
  return PRINTER_IS_K1 ? REMOTE_DIR_K1 : REMOTE_DIR_OTHER;
}

static void selectAntenna()
{
  pinMode(ANT_SWITCH_POWER_PIN, OUTPUT);
  digitalWrite(ANT_SWITCH_POWER_PIN, LOW); // power the RF switch on

  if (USE_EXTERNAL_ANTENNA) {
    pinMode(ANT_SWITCH_SELECT_PIN, OUTPUT);
    digitalWrite(ANT_SWITCH_SELECT_PIN, HIGH); // select U.FL / external antenna
    Serial.println("Antenna: external (U.FL)");
  } else {
    pinMode(ANT_SWITCH_SELECT_PIN, INPUT); // floats low -> onboard ceramic antenna
    Serial.println("Antenna: onboard ceramic");
  }
}

// Opens a session + does password auth. Caller must ssh_free() on failure,
// or use the returned session and clean it up when done.
static ssh_session openAuthedSession(String &err)
{
  ssh_session session = ssh_new();
  if (session == NULL) {
    err = "ssh_new() failed";
    return NULL;
  }

  int verbosity = SSH_LOG_WARNING;
  ssh_options_set(session, SSH_OPTIONS_HOST, PRINTER_HOSTNAME.c_str());
  ssh_options_set(session, SSH_OPTIONS_PORT, &PRINTER_PORT);
  ssh_options_set(session, SSH_OPTIONS_USER, PRINTER_USER);
  ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

  if (ssh_connect(session) != SSH_OK) {
    err = String("connect failed: ") + ssh_get_error(session);
    ssh_free(session);
    return NULL;
  }

  // NOTE: as in the PoC, this does not pin/verify the host key fingerprint.
  // Fine for a trusted home LAN; add pinning if this ever leaves that
  // context.

  if (ssh_userauth_password(session, NULL, PRINTER_PASS) != SSH_AUTH_SUCCESS) {
    err = String("auth failed: ") + ssh_get_error(session);
    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
  }

  return session;
}

static bool runExec(ssh_session session, const char *command, String &err)
{
  ssh_channel channel = ssh_channel_new(session);
  if (channel == NULL || ssh_channel_open_session(channel) != SSH_OK) {
    err = "could not open channel";
    if (channel) ssh_channel_free(channel);
    return false;
  }
  if (ssh_channel_request_exec(channel, command) != SSH_OK) {
    err = String("exec failed: ") + ssh_get_error(session);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }
  // Drain any output so the remote side isn't blocked writing to us.
  char buf[256];
  int n;
  while ((n = ssh_channel_read(channel, buf, sizeof(buf), 0)) > 0) { /* discard */ }
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return true;
}

// Same as runExec but treats a nonzero remote exit status as failure.
// Used for the final rename step in a push, where "the command ran" isn't
// good enough - we need to know it actually succeeded before trusting the
// live database file.
static bool runExecChecked(ssh_session session, const char *command, String &err)
{
  ssh_channel channel = ssh_channel_new(session);
  if (channel == NULL || ssh_channel_open_session(channel) != SSH_OK) {
    err = "could not open channel";
    if (channel) ssh_channel_free(channel);
    return false;
  }
  if (ssh_channel_request_exec(channel, command) != SSH_OK) {
    err = String("exec failed: ") + ssh_get_error(session);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }
  char buf[256];
  int n;
  while ((n = ssh_channel_read(channel, buf, sizeof(buf), 0)) > 0) { /* discard */ }
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  int status = ssh_channel_get_exit_status(channel);
  ssh_channel_free(channel);
  if (status != 0) {
    err = String("command exited with status ") + status + ": " + command;
    return false;
  }
  return true;
}

// Same as runExec but captures stdout so a caller can see the result.
// Used by the /test route and to verify a pushed file's size before
// trusting it enough to become the live database.
static bool runExecCaptured(ssh_session session, const char *command, String &output, String &err)
{
  ssh_channel channel = ssh_channel_new(session);
  if (channel == NULL || ssh_channel_open_session(channel) != SSH_OK) {
    err = "could not open channel";
    if (channel) ssh_channel_free(channel);
    return false;
  }
  if (ssh_channel_request_exec(channel, command) != SSH_OK) {
    err = String("exec failed: ") + ssh_get_error(session);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }
  char buf[256];
  int n;
  while ((n = ssh_channel_read(channel, buf, sizeof(buf), 0)) > 0) {
    output.concat(buf, n);
    if (output.length() > 2048) break; // cap it, this is just a connectivity check
  }
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return true;
}

// Streams `contentLength` bytes from `client` straight into the SSH
// channel, CHUNK_SIZE at a time - never buffers the whole body.
static const size_t STREAM_CHUNK_SIZE = 2048;

static bool streamClientToSsh(WiFiClient &client, ssh_channel channel, long remaining, String &err)
{
  char buf[STREAM_CHUNK_SIZE];
  unsigned long start = millis();
  while (remaining > 0) {
    int avail = client.available();
    if (avail > 0) {
      int want = avail < (int)sizeof(buf) ? avail : (int)sizeof(buf);
      if ((long)want > remaining) want = (int)remaining;
      int n = client.read((uint8_t *)buf, want);
      if (n <= 0) {
        err = "client read error mid-body";
        return false;
      }
      if (ssh_channel_write(channel, buf, n) < 0) {
        err = "ssh write failed";
        return false;
      }
      remaining -= n;
      start = millis();
    } else {
      if (!client.connected()) {
        err = "client disconnected mid-body";
        return false;
      }
      if (millis() - start > 15000) {
        err = "timed out reading request body";
        return false;
      }
      delay(2);
    }
  }
  return true;
}

// Reads one scp-protocol ack/status byte from the channel: 0 = OK,
// 1 = warning, 2 = fatal error, either followed by a human-readable
// message terminated by '\n'. Returns true only on a clean 0 ack.
static bool readScpAck(ssh_channel channel, String &err, unsigned long timeoutMs = 10000)
{
  char status;
  unsigned long start = millis();
  while (true) {
    int n = ssh_channel_read(channel, &status, 1, 0);
    if (n == 1) break;
    if (n < 0) {
      err = "channel read error waiting for scp ack";
      return false;
    }
    if (millis() - start > timeoutMs) {
      err = "timed out waiting for scp ack";
      return false;
    }
    delay(5);
  }
  if (status == 0) return true;

  // Non-zero status: a message follows, terminated by '\n'.
  String msg;
  start = millis();
  while (true) {
    char c;
    int n = ssh_channel_read(channel, &c, 1, 0);
    if (n == 1) {
      if (c == '\n') break;
      msg += c;
      if (msg.length() > 512) break;
    } else if (n < 0) {
      break;
    } else if (millis() - start > 5000) {
      break;
    } else {
      delay(5);
    }
  }
  err = String("remote scp ") + (status == 1 ? "warning: " : "fatal error: ") + msg;
  return false;
}

// Pushes exactly `contentLength` bytes read live from `client` to
// Pushes exactly `contentLength` bytes read live from `client` to a
// temporary file on the printer, then - only once that transfer is fully
// verified - atomically renames it onto the real material_database.json.
// scp itself writes straight to the destination path as bytes arrive and
// is NOT atomic: if the transfer is interrupted partway (a dropped WiFi
// connection anywhere along the chain), a plain "scp -t realfile.json"
// would leave the printer's live, actively-read database half-written.
// Writing to a temp name and renaming into place at the end means a
// failed push never touches the live file at all - worst case, a stray
// .tmp file is left behind and the real database is untouched.
static bool scpPushFileStreaming(ssh_session session, WiFiClient &client, long contentLength, String &err)
{
  String remotePath = String(remoteDir()) + REMOTE_FILENAME;
  String tmpPath = remotePath + ".tmp";

  ssh_channel channel = ssh_channel_new(session);
  if (channel == NULL || ssh_channel_open_session(channel) != SSH_OK) {
    err = "could not open channel";
    if (channel) ssh_channel_free(channel);
    return false;
  }

  String cmd = String("scp -p -t ") + tmpPath;
  if (ssh_channel_request_exec(channel, cmd.c_str()) != SSH_OK) {
    err = String("scp exec failed: ") + ssh_get_error(session);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  // The remote scp -t sink sends a ready byte as soon as it starts up.
  // Writing the header before this arrives is racing the remote process -
  // usually harmless on a fast local link, but not guaranteed, and it
  // means we'd never see a rejection (e.g. disk full, permission denied)
  // reported here instead of silently landing on the printer.
  if (!readScpAck(channel, err)) {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  String tmpFilename = String(REMOTE_FILENAME) + ".tmp";
  String header = "C0644 " + String(contentLength) + " " + tmpFilename + "\n";
  if (ssh_channel_write(channel, header.c_str(), header.length()) < 0) {
    err = "write header failed";
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  if (!readScpAck(channel, err)) {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  bool ok = streamClientToSsh(client, channel, contentLength, err);

  if (ok) {
    const char zero = 0; // trailing null byte per the scp sink protocol
    if (ssh_channel_write(channel, &zero, 1) < 0) {
      err = "write trailing byte failed";
      ok = false;
    }
  }

  if (ok) {
    ok = readScpAck(channel, err);
  }

  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);

  if (!ok) {
    // Transfer itself failed - leave the (possibly partial) .tmp file for
    // inspection rather than the live file. Best-effort cleanup; if this
    // fails too it's harmless, just a stray file.
    String cleanupErr;
    runExec(session, (String("rm -f ") + tmpPath).c_str(), cleanupErr);
    return false;
  }

  // Belt-and-suspenders: confirm the temp file landed at exactly the
  // size we sent before trusting it enough to become the live database.
  String sizeOut, sizeErr;
  String sizeCmd = String("stat -c%s ") + tmpPath;
  if (!runExecCaptured(session, sizeCmd.c_str(), sizeOut, sizeErr)) {
    err = "could not verify pushed file size: " + sizeErr;
    return false;
  }
  sizeOut.trim();
  if (sizeOut.toInt() != contentLength) {
    err = "pushed file size mismatch (expected " + String(contentLength) + ", got " + sizeOut + ") - live database left untouched";
    runExec(session, (String("rm -f ") + tmpPath).c_str(), err);
    return false;
  }

  // Only now, with a fully-verified temp file in hand, replace the live
  // database. A rename on the same filesystem is atomic - the printer
  // will only ever see the old complete file or the new complete file,
  // never something in between.
  String mvCmd = String("mv -f ") + tmpPath + " " + remotePath;
  if (!runExecChecked(session, mvCmd.c_str(), err)) {
    err = "transfer succeeded but could not activate it: " + err;
    return false;
  }

  // The write and rename above are only in the page cache at this point -
  // not guaranteed durable until the filesystem actually flushes them to
  // flash, which normally happens in the background every few seconds.
  // A reboot issued immediately after (as our own /pushdb?reboot=1 does,
  // over an already-open SSH session with none of the natural delay a
  // phone app doing this over the network would have) can race ahead of
  // that flush and discard the unflushed write entirely, silently
  // reverting the printer back to whatever was last durably on disk.
  String syncErr;
  if (!runExecChecked(session, "sync", syncErr)) {
    err = "transfer and activation succeeded, but could not sync to disk: " + syncErr;
    return false;
  }

  return true;
}

// Streams `remaining` bytes from the SSH channel straight into `client`,
// CHUNK_SIZE at a time - never buffers the whole body.
static bool streamSshToClient(ssh_channel channel, WiFiClient &client, long remaining, String &err)
{
  char buf[STREAM_CHUNK_SIZE];
  unsigned long start = millis();
  while (remaining > 0) {
    int want = remaining < (long)sizeof(buf) ? (int)remaining : (int)sizeof(buf);
    int n = ssh_channel_read(channel, buf, want, 0);
    if (n > 0) {
      size_t written = client.write((const uint8_t *)buf, n);
      if (written != (size_t)n) {
        err = "client write failed (short write)";
        return false;
      }
      remaining -= n;
      start = millis();
    } else if (n == 0) {
      if (millis() - start > 15000) {
        err = "timed out reading scp body";
        return false;
      }
      delay(2);
    } else {
      err = "ssh channel read error on body";
      return false;
    }
  }
  return true;
}

// Pulls remoteDir()+REMOTE_FILENAME from the printer via the scp -f
// (source) exec protocol and streams it straight into `client` - the read
// counterpart to scpPushFileStreaming, same protocol the Android app's
// getJsonDB() uses, just chunked rather than buffered.
static bool scpPullFileStreaming(ssh_session session, WiFiClient &client, String &err)
{
  ssh_channel channel = ssh_channel_new(session);
  if (channel == NULL || ssh_channel_open_session(channel) != SSH_OK) {
    err = "could not open channel";
    if (channel) ssh_channel_free(channel);
    return false;
  }

  String remotePath = String(remoteDir()) + REMOTE_FILENAME;
  String cmd = String("scp -f ") + remotePath;
  if (ssh_channel_request_exec(channel, cmd.c_str()) != SSH_OK) {
    err = String("scp -f exec failed: ") + ssh_get_error(session);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  const char zero = 0;
  if (ssh_channel_write(channel, &zero, 1) < 0) {
    err = "write init ack failed";
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  // Read the "C0644 <size> <name>\n" header, one byte at a time. This
  // line is tiny (well under 100 bytes) so a String is fine here - only
  // the file body itself needs to avoid full buffering.
  String header;
  char c;
  unsigned long start = millis();
  while (true) {
    int n = ssh_channel_read(channel, &c, 1, 0);
    if (n == 1) {
      if (c == '\n') break;
      header += c;
      if (header.length() > 512) {
        err = "scp header too long";
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return false;
      }
    } else if (n == 0) {
      if (millis() - start > 10000) {
        err = "timed out reading scp header";
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return false;
      }
      delay(5);
    } else {
      err = "channel read error on header";
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return false;
    }
  }
  if (header.length() == 0 || header[0] != 'C') {
    err = "unexpected scp header: " + header;
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }
  int sp1 = header.indexOf(' ');
  int sp2 = header.indexOf(' ', sp1 + 1);
  if (sp1 < 0 || sp2 < 0) {
    err = "malformed scp header";
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }
  long filesize = header.substring(sp1 + 1, sp2).toInt();

  if (ssh_channel_write(channel, &zero, 1) < 0) {
    err = "write header ack failed";
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return false;
  }

  // Now we know the size - send the HTTP response headers before
  // streaming the body, so the client gets a proper Content-Length.
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print("Content-Length: " + String(filesize) + "\r\n");
  client.print("Access-Control-Allow-Origin: *\r\n");
  client.print("Connection: close\r\n\r\n");

  bool ok = streamSshToClient(channel, client, filesize, err);
  if (ok) {
    ssh_channel_write(channel, &zero, 1); // final ack
  }

  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return ok;
}

// Reads one line (up to and including \n) from a raw client socket.
// Headers are small (well under 1KB total) so String is fine here -
// it's only the request/response BODY that needs true streaming.
static bool readLineFromClient(WiFiClient &client, String &line, unsigned long timeoutMs)
{
  line = "";
  unsigned long start = millis();
  while (true) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (line.endsWith("\r")) line.remove(line.length() - 1);
        return true;
      }
      line += c;
      if (line.length() > 1024) return false; // sanity guard against junk input
      start = millis();
    } else {
      if (!client.connected()) return false;
      if (millis() - start > timeoutMs) return false;
      delay(2);
    }
  }
}

static void sendRawError(WiFiClient &client, int code, const char *msg, const String &detail)
{
  String body = String(msg) + ": " + detail;
  client.print("HTTP/1.1 " + String(code) + " " + String(msg) + "\r\n");
  client.print("Content-Type: text/plain\r\n");
  client.print("Access-Control-Allow-Origin: *\r\n");
  client.print("Content-Length: " + String(body.length()) + "\r\n");
  client.print("Connection: close\r\n\r\n");
  client.print(body);
}

// Handles one request on the raw streaming server: minimal HTTP/1.1
// parsing (request line + headers only, no body buffering), then
// dispatches to the streaming SCP pull/push. Runs inline in sshTask,
// so it has the same big stack the SSH work needs - no task handoff.
static void handleRawRequest(WiFiClient &client)
{
  String requestLine;
  if (!readLineFromClient(client, requestLine, 10000) || requestLine.length() == 0) {
    client.stop();
    return;
  }

  int sp1 = requestLine.indexOf(' ');
  int sp2 = requestLine.indexOf(' ', sp1 + 1);
  if (sp1 < 0 || sp2 < 0) {
    client.stop();
    return;
  }
  String method = requestLine.substring(0, sp1);
  String pathAndQuery = requestLine.substring(sp1 + 1, sp2);
  int qm = pathAndQuery.indexOf('?');
  String path = qm >= 0 ? pathAndQuery.substring(0, qm) : pathAndQuery;
  String query = qm >= 0 ? pathAndQuery.substring(qm + 1) : "";

  long contentLength = -1;
  String headerLine;
  while (readLineFromClient(client, headerLine, 10000) && headerLine.length() > 0) {
    String lower = headerLine;
    lower.toLowerCase();
    if (lower.startsWith("content-length:")) {
      contentLength = headerLine.substring(headerLine.indexOf(':') + 1).toInt();
    }
  }

  Serial.printf("%% raw %s %s (contentLength=%ld)\n", method.c_str(), pathAndQuery.c_str(), contentLength);

  String err;
  ssh_session session = openAuthedSession(err);
  if (session == NULL) {
    sendRawError(client, 502, "SSH connect/auth failed", err);
    client.stop();
    return;
  }

  if (method == "GET" && path == "/getdb") {
    bool ok = scpPullFileStreaming(session, client, err);
    Serial.printf("%% /getdb (raw) result: ok=%d err=%s\n", ok, err.c_str());
    // On failure after headers were already sent, nothing more we can do -
    // the client will see a short/truncated body and can retry.
  } else if (method == "POST" && path == "/pushdb") {
    if (contentLength <= 0) {
      sendRawError(client, 400, "Bad Request", "missing or empty body");
    } else {
      bool ok = scpPushFileStreaming(session, client, contentLength, err);
      if (ok && query.indexOf("reboot=1") >= 0) {
        // The Android app's reboot command runs over a brand new SSH
        // session (fresh TCP handshake, fresh auth) rather than reusing
        // an existing one - that naturally costs it a real, if
        // accidental, pause between the file write completing and the
        // reboot firing. Our session reuse is faster but removes that
        // pause entirely, giving the printer's own material-handling
        // process (very plausibly something watching the file via
        // inotify) no time to react to the change before it reboots.
        // Replicate that breathing room deliberately instead of by
        // accident.
        delay(3000);
        String rebootErr;
        if (!runExec(session, "reboot", rebootErr)) {
          err = "pushed OK, but reboot command failed: " + rebootErr;
        }
      }
      if (ok) {
        String msg = "pushed OK";
        client.print("HTTP/1.1 200 OK\r\n");
        client.print("Content-Type: text/plain\r\n");
        client.print("Access-Control-Allow-Origin: *\r\n");
        client.print("Content-Length: " + String(msg.length()) + "\r\n");
        client.print("Connection: close\r\n\r\n");
        client.print(msg);
      } else {
        sendRawError(client, 502, "Push failed", err);
      }
      Serial.printf("%% /pushdb (raw) result: ok=%d err=%s\n", ok, err.c_str());
    }
  } else {
    sendRawError(client, 404, "Not Found", pathAndQuery);
  }

  ssh_disconnect(session);
  ssh_free(session);
  client.stop();
}

static void sshTask(void *pvParameters)
{
  libssh_begin();

  while (true) {
    // 1. Any raw streaming client waiting? (non-blocking check)
    WiFiClient rawClient = rawServer.available();
    if (rawClient && rawClient.connected()) {
      handleRawRequest(rawClient);
      continue;
    }

    // 2. Any /test job waiting? (short timeout so we keep polling the raw
    // server too rather than blocking here indefinitely)
    if (xSemaphoreTake(jobReadySem, pdMS_TO_TICKS(50)) == pdTRUE) {
      String err;
      ssh_session session = openAuthedSession(err);
      bool ok = false;
      if (session != NULL) {
        jobTestOutput = "";
        ok = runExecCaptured(session, jobTestCommand.c_str(), jobTestOutput, err);
        ssh_disconnect(session);
        ssh_free(session);
      }
      jobResultOk = ok;
      jobResultMsg = ok ? String("ok") : err;
      xSemaphoreGive(jobDoneSem);
      continue;
    }
  }
}

static void setCorsHeaders()
{
  // The streaming service uses port 8081 while the UI uses port 80,
  // so it is still a different browser origin even though both now live on
  // this same XIAO C6. Allow it on the trusted local device.
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  webServer.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  webServer.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

static void handleTest()
{
  setCorsHeaders();
  // Connectivity check only: opens the same SSH session as the real push
  // flow and runs a harmless read-only command. Does not touch
  // material_database.json or any printer state.
  jobIsTest = true;
  jobTestCommand = webServer.hasArg("cmd") ? webServer.arg("cmd") : "echo hello_from_c6 && hostname && uname -a";

  Serial.printf("%% /test: running '%s'\n", jobTestCommand.c_str());

  xSemaphoreGive(jobReadySem);
  if (xSemaphoreTake(jobDoneSem, pdMS_TO_TICKS(20000)) != pdTRUE) {
    jobIsTest = false;
    webServer.send(504, "text/plain", "timed out waiting for SSH test");
    return;
  }
  jobIsTest = false;

  if (jobResultOk) {
    webServer.send(200, "text/plain", "OK - printer responded:\n" + jobTestOutput);
  } else {
    webServer.send(502, "text/plain", "FAILED: " + jobResultMsg);
  }
  Serial.printf("%% /test result: ok=%d msg=%s output=%s\n",
                jobResultOk, jobResultMsg.c_str(), jobTestOutput.c_str());
}


void setup()
{
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("CFS RFID + Printer Push Agent booting on XIAO ESP32-C6...");

  selectAntenna();
  LittleFS.begin(true);
  loadConfig();
  pinMode(RFID_SS_PIN,OUTPUT);digitalWrite(RFID_SS_PIN,HIGH);
  pinMode(TFT_CS_PIN,OUTPUT);digitalWrite(TFT_CS_PIN,HIGH);
  SPI.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
  mfrc522.PCD_Init();
  delay(60);
  Serial.printf("Before TFT: RC522 VersionReg=0x%02X\n",mfrc522.PCD_ReadRegister(MFRC522::VersionReg));
  screenInit();
  key = {255, 255, 255, 255, 255, 255};
  pinMode(SPK_PIN, OUTPUT);
  if (AP_SSID == "" || AP_PASS == "")
  {
    AP_SSID = "K2_RFID";
    AP_PASS = "password";
  }

  Serial.printf("Config: WIFI_SSID='%s' AP_ENABLED=%d WIFI_HOSTNAME='%s'\n",
                WIFI_SSID.c_str(), AP_ENABLED, WIFI_HOSTNAME.c_str());

  bool wifiConfigured = (WIFI_SSID != "" && WIFI_PASS != "");

  if (AP_ENABLED)
  {
    // AP requested, or no WiFi network configured at all - bring the AP up
    // now, before attempting STA, so a mode switch never has to happen
    // after (and risk disrupting) a live STA connection.
    Serial.println("Starting in AP+STA mode");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(Server_IP, Server_IP, Subnet_Mask);
    WiFi.softAP(AP_SSID.c_str(), AP_PASS.c_str());
    WiFi.softAPConfig(Server_IP, Server_IP, Subnet_Mask);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    if (wifiConfigured)
    {
            WiFi.setAutoReconnect(true);
      WiFi.setHostname(WIFI_HOSTNAME.c_str());
      // Modem sleep powers the radio down between beacons to save power,
      // adding latency to every request in exchange for savings this
      // mains-powered device doesn't need.
      WiFi.setSleep(false);
      WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
      bool staConnected = waitForStaConnect(20000);
      Serial.printf("STA connect: %s\n", staConnected ? "OK" : "FAILED");
      if (staConnected) {
        Serial.print("STA IP: ");
        Serial.println(WiFi.localIP());
      }
    }
  }
  else
  {
    // AP explicitly disabled and WiFi is configured - go STA-only. If it
    // fails to connect, fall back to bringing the AP up so the device
    // can't end up totally unreachable - but only in the failure path,
    // never after a connection has already succeeded.
    Serial.println("Starting in STA-only mode (AP disabled in settings)");
    WiFi.mode(WIFI_STA);
        WiFi.setAutoReconnect(true);
    WiFi.setHostname(WIFI_HOSTNAME.c_str());
    WiFi.setSleep(false);
    if (!wifiConfigured) {
      Serial.println("No WiFi credentials configured; AP is explicitly OFF");
    }
    if (wifiConfigured) WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    bool staConnected = wifiConfigured && waitForStaConnect(20000);
    Serial.printf("STA connect: %s\n", staConnected ? "OK" : "FAILED");

    if (staConnected)
    {
      Serial.print("STA IP: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("STA connect failed; AP remains OFF because AP_ENABLED is disabled");
    }
  }

  if (WIFI_HOSTNAME != "")
  {
    String mdnsHost = WIFI_HOSTNAME;
    mdnsHost.replace(".local", "");
    MDNS.begin(mdnsHost.c_str());
  }

  webServer.on("/config", HTTP_GET, handleConfig);
  webServer.on("/index.html", HTTP_GET, handleIndex);
  webServer.on("/", HTTP_GET, handleIndex);
  webServer.on("/material_database.json", HTTP_GET, handleDb);
  webServer.on("/savedb", HTTP_POST, []() {
    if (saveDbFailed) {
      webServer.send(500, "text/plain", "could not open /matdb.json for write");
    } else {
      webServer.send(200, "text/plain", "OK");
    }
  }, handleSaveDbUpload);
  webServer.on("/config", HTTP_POST, handleConfigP);
  webServer.on("/spooldata", HTTP_POST, handleSpoolData);
  webServer.on("/readtag", HTTP_POST, handleReadTag);
  webServer.on("/writetag", HTTP_POST, handleWriteTag);
  webServer.on("/canceltag", HTTP_POST, handleCancelTag);
  webServer.on("/readresult", HTTP_GET, handleReadResult);
  webServer.on("/display/status",HTTP_GET,handleDisplayStatus);
  webServer.on("/display/transfer",HTTP_POST,handleTftTransfer);
  webServer.on("/display/cache",HTTP_POST,handleDisplayCache);
  webServer.on("/display/resolve",HTTP_POST,handleDisplayResolve);
  webServer.on("/update.html", HTTP_POST, []() {
    webServer.send(200, "text/plain", upMsg);
    delay(1000);
    ESP.restart();
    }, []() {
    handleFwUpdate();
  });
  webServer.on("/updatedb.html", HTTP_POST, []() {
    webServer.send(200, "text/plain", upMsg);
    delay(1000);
    ESP.restart();
  }, []() {
    handleDbUpdate();
  });
  webServer.on("/test", HTTP_GET, handleTest);
  webServer.onNotFound(handle404);

  jobReadySem = xSemaphoreCreateBinary();
  jobDoneSem = xSemaphoreCreateBinary();
  rawServer.begin();
  xTaskCreatePinnedToCore(
    sshTask, "sshTask", SSH_TASK_STACK, NULL,
    (tskIDLE_PRIORITY + 3), NULL, portNUM_PROCESSORS - 1);

  webServer.begin();
  Serial.println("Setup complete, web server listening on port 80");
  Serial.printf("Printer streaming service on port %d (/getdb, /pushdb)\n", RAW_SERVER_PORT);
}

void loop()
{
  webServer.handleClient();
  if (previewUntil && (int32_t)(millis()-previewUntil)>=0) {
    previewUntil=0;
    if (rfidAction==RfidAction::Passive) screenStatus("READY");
  }
  drawDisplay();

  // Cooling down after the last tag tap (debounce), but the web server
  // above keeps running the whole time - unlike a delay() here, which
  // would freeze it for the same window.
  if (millis() < tagCooldownUntil)
    return;

  if (tagHeld) {
    if (millis() < nextPresenceCheck) return;
    nextPresenceCheck=millis()+170;
    byte atqa[2];byte atqaLen=sizeof(atqa);
    MFRC522::StatusCode seen=mfrc522.PICC_WakeupA(atqa,&atqaLen);
    if(seen==MFRC522::STATUS_OK || seen==MFRC522::STATUS_COLLISION) {
      absentChecks=0;
      mfrc522.PICC_HaltA();
    } else if (++absentChecks>=4) {
      tagHeld=false;heldUid="";absentChecks=0;
      mfrc522.PCD_StopCrypto1();
      Serial.println("Tag removed - ready for a new presentation");
    }
    return;
  }
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  String tapUid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (mfrc522.uid.uidByte[i] < 0x10) tapUid += "0";
    tapUid += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.printf("Tag present: UID=%s action=%d\n", tapUid.c_str(), (int)rfidAction);
  tagHeld=true;heldUid=tapUid;absentChecks=0;nextPresenceCheck=millis()+300;
  displayUid=tapUid;

  encrypted = false;

  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.printf("Unsupported PICC type: %d\n", (int)piccType);
    tone(SPK_PIN, 400, 400);
    screenStatus("RFID ERROR");
    tagCooldownUntil = millis() + 2000;
    return;
  }

  createKey();

  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 7, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.printf("Default-key auth failed: %s - retrying with computed key\n", mfrc522.GetStatusCodeName(status));
    if (!mfrc522.PICC_IsNewCardPresent())
      return;
    if (!mfrc522.PICC_ReadCardSerial())
      return;
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 7, &ekey, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
      Serial.printf("Computed-key auth also failed: %s - giving up on this tap\n", mfrc522.GetStatusCodeName(status));
      tone(SPK_PIN, 400, 150);
      delay(300);
      tone(SPK_PIN, 400, 150);
      screenStatus("READ ERROR");
      tagCooldownUntil = millis() + 2000;
      return;
    }
    encrypted = true;
    Serial.println("Authenticated with computed key - tag is already personalized");
  }
  else
  {
    Serial.println("Authenticated with default key - tag is blank/unpersonalized");
  }

  if (rfidAction != RfidAction::Write)
  {
    const bool explicitRead = rfidAction == RfidAction::Read;
    Serial.println("Read armed - reading tag instead of writing");
    String previousTagId=readTagId,previousMaterial=readMaterialType;
    String previousColor=readColor,previousWeight=readWeight,previousPrinter=readPrinterType;
    bool previousReady=readReady,previousError=readError;
    performTagRead();
    bool scanFailed=readError;
    String scanId=readTagId,scanMaterial=readMaterialType,scanColor=readColor;
    String scanWeight=readWeight,scanPrinter=readPrinterType;
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    rfidAction = RfidAction::Passive;
    readArmed = false;
    if (explicitRead) readReady = true;
    else {
      readTagId=previousTagId;readMaterialType=previousMaterial;
      readColor=previousColor;readWeight=previousWeight;readPrinterType=previousPrinter;
      readReady=previousReady;readError=previousError;
    }
    if (scanFailed)
    {
      Serial.println("Read failed / unrecognized tag format");
      screenStatus("READ ERROR");
      tone(SPK_PIN, 400, 150);
      delay(300);
      tone(SPK_PIN, 400, 150);
    }
    else
    {
      // Presentation-only lookup against the bundled catalogue snapshot.
      // Unknown/custom identifiers remain visible as IDs; no database writes.
      String friendlyBrand, friendlyName;
      if (displayCachedLabel(scanMaterial, friendlyBrand, friendlyName) ||
          displayLookupMaterial(scanMaterial, friendlyBrand, friendlyName)) {
        lastReadBrand=friendlyBrand;
        lastReadMaterial=friendlyName;
      } else {
        lastReadBrand="Material ID";
        lastReadMaterial=scanMaterial;
      }
      lastReadColor=scanColor;lastReadWeight=scanWeight;
      screenStatus(explicitRead ? "TAG READ" : "PASSIVE READ");
      if (!explicitRead) previewUntil=millis()+6700;
      Serial.printf("Read OK: materialType=%s color=%s weight=%s printerType=%s\n",
                    scanMaterial.c_str(), scanColor.c_str(), scanWeight.c_str(), scanPrinter.c_str());
      tone(SPK_PIN, 1000, 200);
    }
    tagCooldownUntil = millis() + 2000;
    return;
  }

  Serial.printf("Writing spoolData (%u bytes): %s\n", spoolData.length(), spoolData.c_str());

  bool writeOk=true;
  byte blockData[17];
  byte encData[16];
  int blockID = 4;
  for (int i = 0; i < spoolData.length(); i += 16)
  {
    spoolData.substring(i, i + 16).getBytes(blockData, 17);
    if (blockID >= 4 && blockID < 7)
    {
      aes.encrypt(1, blockData, encData);
      status=(MFRC522::StatusCode)mfrc522.MIFARE_Write(blockID, encData, 16);
      if(status!=MFRC522::STATUS_OK) {
        Serial.printf("Block %d write failed: %s\n",blockID,mfrc522.GetStatusCodeName(status));
        writeOk=false;
        break;
      }
    }
    blockID++;
  }

  if (writeOk && !encrypted)
  {
    byte buffer[18];
    byte byteCount = sizeof(buffer);
    byte block = 7;
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if(status!=MFRC522::STATUS_OK) {
      Serial.printf("Key block read failed: %s\n",mfrc522.GetStatusCodeName(status));
      writeOk=false;
    }
    int y = 0;
    if(writeOk) for (int i = 10; i < 16; i++)
    {
      buffer[i] = ekey.keyByte[y];
      y++;
    }
    for (int i = 0; i < 6; i++)
    {
      buffer[i] = ekey.keyByte[i];
    }
    if(writeOk) {
      status=(MFRC522::StatusCode)mfrc522.MIFARE_Write(7, buffer, 16);
      if(status!=MFRC522::STATUS_OK) {
        Serial.printf("Key block write failed: %s\n",mfrc522.GetStatusCodeName(status));
        writeOk=false;
      }
    }
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  if(writeOk) {
    Serial.println("Write complete (write commands acknowledged; not read-back verified)");
    screenStatus("WRITE COMPLETE");
    tone(SPK_PIN, 1000, 200);
  } else {
    Serial.println("Write failed - one or more block operations failed");
    screenStatus("WRITE ERROR");
    tone(SPK_PIN,400,300);
  }
  rfidAction=RfidAction::Passive;
  tagCooldownUntil = millis() + 2000;
}

// Reads back what WriteSpoolData wrote (matching the Android app's ReadTag()):
// sector 1 (blocks 4-6, 48 bytes) holds "AB124"+vendorId(4)+batch(2)+
// filamentId(6)+color(7)+length(4)+serialNum(6)+reserve(14), AES-encrypted
// once the tag has been personalized; sector 2 (blocks 8-10, 48 bytes,
// always default-keyed) holds the printer-type string in plain text.
// Called from loop() with sector 1 already authenticated (key or ekey,
// per the `encrypted` flag) - only sector 2 needs authenticating here.
void performTagRead()
{
  readError = false;
  readTagId = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (mfrc522.uid.uidByte[i] < 0x10) readTagId += "0";
    readTagId += String(mfrc522.uid.uidByte[i], HEX);
  }
  readTagId.toUpperCase();

  String part1 = "";
  byte buffer[18];
  byte byteCount;
  for (byte block = 4; block <= 6; block++)
  {
    byteCount = sizeof(buffer);
    MFRC522::StatusCode st = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (st != MFRC522::STATUS_OK)
    {
      Serial.printf("Read failed on block %d: %s\n", block, mfrc522.GetStatusCodeName(st));
      readError = true;
      return;
    }
    byte plain[16];
    if (encrypted)
    {
      aes.decrypt(1, buffer, plain);
    }
    else
    {
      for (byte i = 0; i < 16; i++) plain[i] = buffer[i];
    }
    for (byte i = 0; i < 16; i++) part1 += (char)plain[i];
  }

  // Sector 2 is separate from sector 1 - needs its own authentication.
  String part2 = "";
  mfrc522.PCD_StopCrypto1();
  MFRC522::StatusCode st2 = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 11, &key, &(mfrc522.uid));
  if (st2 == MFRC522::STATUS_OK)
  {
    for (byte block = 8; block <= 10; block++)
    {
      byteCount = sizeof(buffer);
      MFRC522::StatusCode st = mfrc522.MIFARE_Read(block, buffer, &byteCount);
      if (st == MFRC522::STATUS_OK)
      {
        for (byte i = 0; i < 16; i++) part2 += (char)buffer[i];
      }
    }
  }
  else
  {
    Serial.printf("Sector 2 auth failed: %s (printer type will be blank)\n", mfrc522.GetStatusCodeName(st2));
  }

  String tagData = part1 + part2;
  Serial.printf("Decoded part1 (raw): '%s'\n", part1.c_str());

  if (!tagData.startsWith("AB124") || tagData.length() < 48)
  {
    readError = true;
    return;
  }

  String materialId = tagData.substring(12, 17);
  materialId.trim();
  String colorHex = tagData.substring(18, 24);
  colorHex.trim();
  String lengthCode = tagData.substring(24, 28);
  lengthCode.trim();
  String printerType = "";
  if (tagData.length() >= 96)
  {
    printerType = tagData.substring(48, 96);
    printerType.trim();
  }

  readMaterialType = materialId;
  readColor = colorHex;
  readWeight = GetMaterialWeightLabel(lengthCode);
  readPrinterType = printerType;
}

void createKey()
{
  int x = 0;
  byte uid[16];
  byte bufOut[16];
  for (int i = 0; i < 16; i++)
  {
    if (x >= 4)
      x = 0;
    uid[i] = mfrc522.uid.uidByte[x];
    x++;
  }
  aes.encrypt(0, uid, bufOut);
  for (int i = 0; i < 6; i++)
  {
    ekey.keyByte[i] = bufOut[i];
  }
}

void handleIndex()
{
  webServer.send_P(200, "text/html", indexData);
}

void handle404()
{
  webServer.send(404, "text/plain", "Not Found");
}

void handleConfig()
{
  String htmStr = AP_SSID + "|-|" + WIFI_SSID + "|-|" + WIFI_HOSTNAME + "|-|" + PRINTER_HOSTNAME + "|-|" + (AP_ENABLED ? "1" : "0");
  webServer.setContentLength(htmStr.length());
  webServer.send(200, "text/plain", htmStr);
}

void handleConfigP()
{
  if (webServer.hasArg("ap_ssid") && webServer.hasArg("ap_pass") && webServer.hasArg("wifi_ssid") && webServer.hasArg("wifi_pass") && webServer.hasArg("wifi_host") && webServer.hasArg("printer_host"))
  {
    AP_SSID = webServer.arg("ap_ssid");
    if (!webServer.arg("ap_pass").equals("********"))
    {
      AP_PASS = webServer.arg("ap_pass");
    }
    WIFI_SSID = webServer.arg("wifi_ssid");
    if (!webServer.arg("wifi_pass").equals("********"))
    {
      WIFI_PASS = webServer.arg("wifi_pass");
    }
    WIFI_HOSTNAME = webServer.arg("wifi_host");
    PRINTER_HOSTNAME = webServer.arg("printer_host");
    // Checkboxes only appear in form data when checked - so treat presence
    // of the arg as "on". Refuse to disable the AP unless WiFi credentials
    // are actually set, so the device can't be locked out of being reachable.
    bool wantApEnabled = webServer.hasArg("ap_enabled");
    AP_ENABLED = wantApEnabled;
    File file = LittleFS.open("/config.ini", "w");
    if (file)
    {
      file.print("\r\nAP_SSID=" + AP_SSID + "\r\nAP_PASS=" + AP_PASS + "\r\nWIFI_SSID=" + WIFI_SSID + "\r\nWIFI_PASS=" + WIFI_PASS + "\r\nWIFI_HOST=" + WIFI_HOSTNAME + "\r\nPRINTER_HOST=" + PRINTER_HOSTNAME + "\r\nAP_ENABLED=" + (AP_ENABLED ? "1" : "0") + "\r\n");
      file.close();
    }
    String htmStr = "OK";
    webServer.setContentLength(htmStr.length());
    webServer.send(200, "text/plain", htmStr);
    delay(1000);
    ESP.restart();
  }
  else
  {
    webServer.send(417, "text/plain", "Expectation Failed");
  }
}

void handleDb()
{
  // Preference order: a locally-edited plain copy (from the new material
  // manager, saved via /savedb), then a previously uploaded .json (stored
  // gzipped via /updatedb.html), then the firmware's embedded default.
  File jsonFile = LittleFS.open("/matdb.json", "r");
  if (jsonFile) {
    webServer.streamFile(jsonFile, "application/json");
    jsonFile.close();
    return;
  }

  File dataFile = LittleFS.open("/matdb.gz", "r");
  if (!dataFile) {
    webServer.sendHeader("Content-Encoding", "gzip");
    webServer.send_P(200, "application/json", material_database, sizeof(material_database));
  }
  else
  {
    webServer.streamFile(dataFile, "application/json");
    dataFile.close();
  }
}

void handleSaveDbUpload()
{
  // Writes the edited material_database.json straight to flash as each
  // chunk arrives, instead of buffering the whole body in a String first.
  // The old approach (webServer.arg("plain")) needed the entire file to
  // fit in RAM at once - the MCU must not buffer a database of this size
  // heap, and this file is now 300KB+, so that was guaranteed to fail
  // (silently return an empty/truncated body, or just crash) well before
  // it got anywhere near that size. Chunked writes have no such limit.
  HTTPUpload &upload = webServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    saveDbFailed = false;
    // Defensive: if a previous upload was interrupted (e.g. a WiFi drop
    // mid-transfer) without the library calling us with
    // UPLOAD_FILE_ABORTED - which isn't guaranteed for every kind of
    // disconnect - upFile could still be open from that attempt.
    // LittleFS only supports a handful of simultaneously open files, so
    // a leaked handle here would make every subsequent file operation on
    // the device fail silently, looking like the whole UI is "locked" on
    // any client that connects. Always start clean.
    if (upFile) {
      upFile.close();
    }
    if (LittleFS.exists("/matdb.json")) {
      LittleFS.remove("/matdb.json");
    }
    upFile = LittleFS.open("/matdb.json", "w");
    if (!upFile) saveDbFailed = true;
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (upFile) {
      size_t written = upFile.write(upload.buf, upload.currentSize);
      if (written != upload.currentSize) saveDbFailed = true;
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (upFile) {
      upFile.close();
    }
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    saveDbFailed = true;
    if (upFile) upFile.close();
  }
}

void handleDbUpdate()
{
  upMsg = "";
  if (webServer.uri() != "/updatedb.html") {
    upMsg = "Error";
    return;
  }
  HTTPUpload &upload = webServer.upload();
  if (upload.filename != "material_database.json") {
    upMsg = "Invalid database file<br><br>" + upload.filename;
    return;
  }
  if (upload.status == UPLOAD_FILE_START) {
    if (upFile) {
      upFile.close();
    }
    if (LittleFS.exists("/matdb.gz")) {
      LittleFS.remove("/matdb.gz");
    }
    upFile = LittleFS.open("/matdb.gz", "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (upFile) {
      upFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (upFile) {
      upFile.close();
      upMsg = "Database update complete, Rebooting";
    }
  }
}

void handleFwUpdate()
{
  upMsg = "";
  if (webServer.uri() != "/update.html") {
    upMsg = "Error";
    return;
  }
  HTTPUpload &upload = webServer.upload();
  if (!upload.filename.endsWith(".bin")) {
    upMsg = "Invalid update file<br><br>" + upload.filename;
    return;
  }
  if (upload.status == UPLOAD_FILE_START) {
    if (LittleFS.exists("/update.bin")) {
      LittleFS.remove("/update.bin");
    }
    upFile = LittleFS.open("/update.bin", "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (upFile) {
      upFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (upFile) {
      upFile.close();
    }
    updateFw();
  }
}

void updateFw()
{
  if (LittleFS.exists("/update.bin")) {
    File updateFile;
    updateFile = LittleFS.open("/update.bin", "r");
    if (updateFile) {
      size_t updateSize = updateFile.size();
      if (updateSize > 0) {
        md5.begin();
        md5.addStream(updateFile, updateSize);
        md5.calculate();
        String md5Hash = md5.toString();
        updateFile.close();
        updateFile = LittleFS.open("/update.bin", "r");
        if (updateFile) {
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace, U_FLASH)) {
            updateFile.close();
            upMsg = "Update failed<br><br>" + errorMsg(Update.getError());
            return;
          }
          int md5BufSize = md5Hash.length() + 1;
          char md5Buf[md5BufSize];
          md5Hash.toCharArray(md5Buf, md5BufSize) ;
          Update.setMD5(md5Buf);
          long bsent = 0;
          int cprog = 0;
          while (updateFile.available()) {
            uint8_t ibuffer[1];
            updateFile.read((uint8_t *)ibuffer, 1);
            Update.write(ibuffer, sizeof(ibuffer));
            bsent++;
            int progr = ((double)bsent /  updateSize) * 100;
            if (progr >= cprog) {
              cprog = progr + 10;
            }
          }
          updateFile.close();
          LittleFS.remove("/update.bin");
          if (Update.end(true))
          {
            String uHash = md5Hash.substring(0,10);
            String iHash = Update.md5String().substring(0,10);
            iHash.toUpperCase();
            uHash.toUpperCase();
            upMsg = "Uploaded:&nbsp; " + uHash + "<br>Installed: " + iHash + "<br><br>Update complete, Rebooting.";
          }
          else
          {
            upMsg = "Update failed";
          }
        }
      }
      else {
        updateFile.close();
        LittleFS.remove("/update.bin");
        upMsg = "Error, file is invalid";
        return;
      }
    }
  }
  else
  {
    upMsg = "No update file found";
  }
}

// RFID operation controls ONLY; no database or printer operations.
void handleWriteTag() {
  if(!writeSelectionApplied) {
    webServer.send(409,"text/plain","Apply filament selection before arming write");return;
  }
  rfidAction=RfidAction::Write;
  readArmed=false;
  previewUntil=0;
  screenStatus("READY TO WRITE");
  webServer.send(200,"text/plain","READY TO WRITE");
}
void handleCancelTag() {
  rfidAction=RfidAction::Passive;
  readArmed=false;previewUntil=0;
  screenStatus("READY");
  webServer.send(200,"text/plain","CANCELLED");
}

void handleReadTag()
{
  rfidAction=RfidAction::Read;
  previewUntil=0;
  readArmed = true;
  screenStatus("READY TO READ");
  readReady = false;
  readError = false;
  webServer.send(200, "text/plain", "OK");
}

void handleReadResult()
{
  // Hand-built JSON: every field here is either a fixed vocabulary value
  // (hex UID, hex color, digits) or came from our own write format, so no
  // escaping is needed - matches this sketch's existing style of not
  // pulling in a JSON library on the MCU.
  String json = "{\"ready\":" + String(readReady ? "true" : "false");
  json += ",\"error\":" + String(readError ? "true" : "false");
  json += ",\"tagId\":\"" + readTagId + "\"";
  json += ",\"materialType\":\"" + readMaterialType + "\"";
  json += ",\"color\":\"" + readColor + "\"";
  json += ",\"weight\":\"" + readWeight + "\"";
  json += ",\"printerType\":\"" + readPrinterType + "\"}";
  webServer.send(200, "application/json", json);
}

void handleSpoolData()
{
  if (webServer.hasArg("materialColor") && webServer.hasArg("materialType") && webServer.hasArg("materialWeight"))
  {
    selectedBrand=webServer.arg("displayBrand");
    selectedMaterial=webServer.arg("displayMaterial");
    selectedWeight=webServer.arg("materialWeight");
    String materialColor = webServer.arg("materialColor");
    selectedColor=materialColor;selectedColor.replace("#", "");
    selectedColor.toUpperCase();
    rfidAction=RfidAction::Passive; readArmed=false; previewUntil=0;
    writeSelectionApplied=true;
    screenStatus("READY");
    materialColor.replace("#", "");
    String filamentId = "1" + webServer.arg("materialType"); // material_database.json
    String vendorId = "0276";                                               // 0276 creality
    String color = "0" + materialColor;
    String filamentLen = GetMaterialLength(webServer.arg("materialWeight"));
    String serialNum = String(random(100000, 999999)); // 000001
    String reserve = "000000";
    spoolData = "AB124" + vendorId + "A2" + filamentId + color + filamentLen + serialNum + reserve + "00000000";
    File file = LittleFS.open("/spool.ini", "w");
    if (file)
    {
      file.print(spoolData);
      file.close();
    }
    String htmStr = "OK";
    webServer.setContentLength(htmStr.length());
    webServer.send(200, "text/plain", htmStr);
  }
  else
  {
    webServer.send(417, "text/plain", "Expectation Failed");
  }
}

String GetMaterialLength(String materialWeight)
{
  if (materialWeight == "1 KG")
  {
    return "0330";
  }
  else if (materialWeight == "750 G")
  {
    return "0247";
  }
  else if (materialWeight == "600 G")
  {
    return "0198";
  }
  else if (materialWeight == "500 G")
  {
    return "0165";
  }
  else if (materialWeight == "250 G")
  {
    return "0082";
  }
  return "0330";
}

String GetMaterialWeightLabel(String lengthCode)
{
  if (lengthCode == "0330") return "1 KG";
  if (lengthCode == "0247") return "750 G";
  if (lengthCode == "0198") return "600 G";
  if (lengthCode == "0165") return "500 G";
  if (lengthCode == "0082") return "250 G";
  return "1 KG";
}

String errorMsg(int errnum)
{
  if (errnum == UPDATE_ERROR_OK) {
    return "No Error";
  } else if (errnum == UPDATE_ERROR_WRITE) {
    return "Flash Write Failed";
  } else if (errnum == UPDATE_ERROR_ERASE) {
    return "Flash Erase Failed";
  } else if (errnum == UPDATE_ERROR_READ) {
    return "Flash Read Failed";
  } else if (errnum == UPDATE_ERROR_SPACE) {
    return "Not Enough Space";
  } else if (errnum == UPDATE_ERROR_SIZE) {
    return "Bad Size Given";
  } else if (errnum == UPDATE_ERROR_STREAM) {
    return "Stream Read Timeout";
  } else if (errnum == UPDATE_ERROR_MD5) {
    return "MD5 Check Failed";
  } else if (errnum == UPDATE_ERROR_MAGIC_BYTE) {
    return "Magic byte is wrong, not 0xE9";
  } else {
    return "UNKNOWN";
  }
}

void loadConfig()
{
  if (LittleFS.exists("/config.ini"))
  {
    File file = LittleFS.open("/config.ini", "r");
    if (file)
    {
      String iniData;
      while (file.available())
      {
        char chnk = file.read();
        iniData += chnk;
      }
      file.close();
      if (instr(iniData, "AP_SSID="))
      {
        AP_SSID = split(iniData, "AP_SSID=", "\r\n");
        AP_SSID.trim();
      }

      if (instr(iniData, "AP_PASS="))
      {
        AP_PASS = split(iniData, "AP_PASS=", "\r\n");
        AP_PASS.trim();
      }

      if (instr(iniData, "WIFI_SSID="))
      {
        WIFI_SSID = split(iniData, "WIFI_SSID=", "\r\n");
        WIFI_SSID.trim();
      }

      if (instr(iniData, "WIFI_PASS="))
      {
        WIFI_PASS = split(iniData, "WIFI_PASS=", "\r\n");
        WIFI_PASS.trim();
      }

      if (instr(iniData, "WIFI_HOST="))
      {
        WIFI_HOSTNAME = split(iniData, "WIFI_HOST=", "\r\n");
        WIFI_HOSTNAME.trim();
      }

      if (instr(iniData, "PRINTER_HOST="))
      {
        PRINTER_HOSTNAME = split(iniData, "PRINTER_HOST=", "\r\n");
        PRINTER_HOSTNAME.trim();
      }


      if (instr(iniData, "AP_ENABLED="))
      {
        String apEnabledStr = split(iniData, "AP_ENABLED=", "\r\n");
        apEnabledStr.trim();
        AP_ENABLED = apEnabledStr != "0";
      }
      
    }
  }
  else
  {
    File file = LittleFS.open("/config.ini", "w");
    if (file)
    {
      file.print("\r\nAP_SSID=" + AP_SSID + "\r\nAP_PASS=" + AP_PASS + "\r\nWIFI_SSID=" + WIFI_SSID + "\r\nWIFI_PASS=" + WIFI_PASS + "\r\nWIFI_HOST=" + WIFI_HOSTNAME + "\r\nPRINTER_HOST=" + PRINTER_HOSTNAME + "\r\nAP_ENABLED=" + (AP_ENABLED ? "1" : "0") + "\r\n");
      file.close();
    }
  }

  if (LittleFS.exists("/spool.ini"))
  {
    File file = LittleFS.open("/spool.ini", "r");
    if (file)
    {
      String iniData;
      while (file.available())
      {
        char chnk = file.read();
        iniData += chnk;
      }
      file.close();
      spoolData = iniData;
    }
  }
  else
  {
    File file = LittleFS.open("/spool.ini", "w");
    if (file)
    {
      file.print(spoolData);
      file.close();
    }
  }
}

String split(String str, String from, String to)
{
  String tmpstr = str;
  tmpstr.toLowerCase();
  from.toLowerCase();
  to.toLowerCase();
  int pos1 = tmpstr.indexOf(from);
  int pos2 = tmpstr.indexOf(to, pos1 + from.length());
  String retval = str.substring(pos1 + from.length(), pos2);
  return retval;
}

bool instr(String str, String search)
{
  int result = str.indexOf(search);
  if (result == -1)
  {
    return false;
  }
  return true;
}
