// Generated read-only DISPLAY labels from the existing embedded firmware catalogue.
// No printer, database, sync, tag encoding or storage code is changed.
#pragma once
#include <Arduino.h>
struct DisplayMaterialName {const char *id;const char *brand;const char *name;};
static const DisplayMaterialName displayMaterialNames[] = {
  {"01001","Creality","Hyper PLA"},
  {"02001","Creality","Hyper PLA-CF"},
  {"06002","Creality","Hyper PETG"},
  {"03001","Creality","Hyper ABS"},
  {"04001","Creality","CR-PLA"},
  {"05001","Creality","CR-Silk"},
  {"06001","Creality","CR-PETG"},
  {"07001","Creality","CR-ABS"},
  {"00001","Generic","Generic PLA"},
  {"00002","Generic","Generic PLA-Silk"},
  {"00003","Generic","Generic PETG"},
  {"00004","Generic","Generic ABS"},
  {"00005","Generic","Generic TPU"},
  {"00006","Generic","Generic PLA-CF"},
  {"00007","Generic","Generic ASA"},
  {"08001","Creality","Ender-PLA"},
  {"09001","Creality","EN-PLA+"},
  {"10001","Creality","HP-TPU"},
  {"11001","Creality","CR-Nylon"},
  {"13001","Creality","CR-PLA Carbon"},
  {"14001","Creality","CR-PLA Matte"},
  {"15001","Creality","CR-PLA Fluo"},
  {"16001","Creality","CR-TPU"},
  {"17001","Creality","CR-Wood"},
  {"18001","Creality","HP Ultra PLA"},
  {"19001","Creality","HP-ASA"},
  {"00008","Generic","Generic PA"},
  {"00009","Generic","Generic PA-CF"},
  {"00010","Generic","Generic BVOH"},
  {"00011","Generic","Generic PVA"},
  {"00012","Generic","Generic HIPS"},
  {"00013","Generic","Generic PET-CF"},
  {"00014","Generic","Generic PETG-CF"},
  {"00015","Generic","Generic PA6-CF"},
  {"00016","Generic","Generic PAHT-CF"},
  {"00017","Generic","Generic PPS"},
  {"00018","Generic","Generic PPS-CF"},
  {"00019","Generic","Generic PP"},
  {"00020","Generic","Generic PET"},
  {"00021","Generic","Generic PC"},
};
static bool displayLookupMaterial(const String &id, String &brand, String &name) {
  for (const auto &record : displayMaterialNames) {
    if (id == record.id) {brand=record.brand;name=record.name;return true;}
  }
  return false;
}
