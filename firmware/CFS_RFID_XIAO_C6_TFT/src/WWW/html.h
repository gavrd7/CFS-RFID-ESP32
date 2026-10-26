static const char indexData[] PROGMEM = R"==(<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Creality CFS RFID Manager</title>
  <style>
    .btn {
      background-color: #0047AB;
      border: none;
      color: #FFFFFF;
      padding: 16px 30px 16px 30px;
      font-size: 16px;
      cursor: pointer;
      font-weight: bold;
      margin: 10px;
      border-radius: 30px;
    }

    .btn:hover {
      background-color: #1976D2;
    }

    svg {
      fill: #0047AB;
      background-color: #E0E0E0;
      border: none;
      cursor: pointer;
    }

    svg:hover {
      fill: #1976D2;
      background-color: #E0E0E0;
    }

    body {
      font-family: arial, sans-serif;
      background-color: #E0E0E0;
      margin: 0;
    }

    label {
      color: #000000;
      font-size: 18px;
      font-weight: bold;
      font-family: arial, sans-serif;
    }

    .main {
      padding: 0;
      position: absolute;
      top: 0;
      right: 0;
      bottom: 0;
      left: 0;
      background-color: #E0E0E0;
      font-size: 16px;
      font-weight: bold;
      color: #000000;
      font-family: arial, sans-serif;
    }

    input[type="color"] {
      border: 1px solid #000000;
      background-color: #ffffff;
      width: 140px;
      height: 40px;
      margin: 10px;
      cursor: pointer;
    }

    table {
      font-family: arial, sans-serif;
      border-collapse: collapse;
    }

    select {
      height: 40px;
      width: 140px;
      padding: 10px;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
    }

    .msg {
      color: #000000;
      font-size: 16px;
      font-weight: bold;
      text-shadow: none;
    }

    .errormsg {
      color: #ff0000;
      font-size: 16px;
      font-weight: bold;
      text-shadow: none;
    }

    dialog {
      padding: 0 !important;
      border: none;
      margin: 0;
      min-width: 100%;
      min-height: 100%;
      max-width: 100%;
      max-height: 100%;
      background-color: #E0E0E0;
    }

    td {
      border: none;
      text-align: left;
      padding: 8px;
    }

    th {
      border: none;
      color: white;
      background-color: #154c79;
      text-align: center;
      padding: 8px;
    }

    input[type="text"] {
      padding: 10px;
      font-size: 16px;
      margin: 2px;
      border: 1px solid #000000;
      cursor: pointer;
    }

    input[type="text"]:invalid {
      border: 1px solid red;
    }

    input[type="text"]:invalid:focus {
      background-color: rgba(255, 0, 0, 0.486);
      background-blend-mode: overlay;
    }

    progress {
      border-radius: 6px;
      width: 200px;
      height: 16px;
      overflow: hidden;
    }

    progress::-webkit-progress-bar {
      background-color: #ffffff;
      border-radius: 6px;
    }

    progress::-webkit-progress-value {
      background-color: #3498db;
      border-bottom-left-radius: 6px;
      border-top-left-radius: 6px;
      float: left;
    }

    #loader {
      z-index: 1;
      width: 50px;
      height: 50px;
      margin: 0 0 0 0;
      border: 6px solid #f3f3f3;
      border-radius: 50%;
      border-top: 6px solid #3498db;
      width: 50px;
      height: 50px;
      -webkit-animation: spin 2s linear infinite;
      animation: spin 2s linear infinite;
    }

    @-webkit-keyframes spin {
      0% {
        -webkit-transform: rotate(0deg);
      }

      100% {
        -webkit-transform: rotate(360deg);
      }
    }

    @keyframes spin {
      0% {
        transform: rotate(0deg);
      }

      100% {
        transform: rotate(360deg);
      }
    }

    /* v0.2 visual refresh: original HTML IDs, events, and script preserved */
    :root { color-scheme:dark; --bg:#0b111b;--panel:#162232;--panel2:#1e2e42;--border:#304358;--ink:#edf5ff;--muted:#acc1d6;--accent:#389bfa;--success:#63daa8; }
    * { box-sizing:border-box; }
    html { min-height:100%; background:var(--bg); }
    body { margin:0; min-height:100vh; font-family:system-ui,-apple-system,Segoe UI,sans-serif; background:radial-gradient(ellipse at 50% -25%,#213f63 0,transparent 62%),var(--bg); color:var(--ink); }
    .main { position:static; width:min(100%,900px); min-height:0; margin:0 auto; padding:28px 20px 44px; background:transparent; color:var(--ink); font:inherit; text-align:left; }
    body > center > .main { min-height:100vh; }
    .mv-head { width:100%; max-width:740px; margin:0 auto 22px; display:flex; justify-content:space-between; align-items:center; gap:12px; text-align:left; }
    .mv-head-title { display:flex; gap:12px; align-items:center; }
    .mv-logo { width:46px; height:46px; display:grid; place-items:center; border-radius:14px; background:#234970; color:#c8e5ff; font-size:24px; }
    .mv-head strong { display:block; font-size:22px; letter-spacing:-.045em; line-height:1.2; }
    .mv-sub { color:var(--muted); font-size:13px; font-weight:500; }
    .mv-pill { display:inline-flex; padding:7px 11px; border:1px solid #3e5c75; background:#1b3445; color:#a4e1c5; border-radius:20px; font-size:12px; white-space:nowrap; }
    .mv-intro { max-width:740px; margin:0 auto 14px; font-size:13px; color:var(--muted); text-align:left; }
    body > center > .main > table { width:100%; max-width:740px; margin:0 auto; background:var(--panel); border:1px solid var(--border); border-radius:18px; border-collapse:separate; border-spacing:0; padding:10px 16px 20px; box-shadow:0 16px 45px #0004; }
    body > center > .main > table tr:first-child td { padding:6px 10px; }
    body > center > .main > table tr:nth-child(2) { display:none; }
    body > center > .main > table tr:not(:first-child):not(:nth-child(2)) { border-bottom:1px solid var(--border); }
    td { border:none; padding:10px; color:var(--ink); vertical-align:middle; }
    th { background:#254b72; border:none; color:var(--ink); padding:11px; }
    body > center > .main > table td:first-child { width:42%; }
    b,label { color:var(--ink); font-size:14px; font-weight:600; font-family:inherit; }
    select,input[type='text'],input[type='password'],input[type='number'] { background:var(--panel2); color:var(--ink); border:1px solid #45617a; border-radius:10px; font-size:15px; padding:10px 12px; min-height:43px; max-width:100%; }
    select { width:100%; min-width:0; margin:0; height:auto; }
    input[type='text'] { margin:0; }
    input[type='color'] { width:100%; height:46px; margin:0; padding:3px; border:1px solid #45617a; border-radius:10px; background:var(--panel2); }
    select:focus-visible,input:focus-visible,.btn:focus-visible,svg:focus-visible { outline:2px solid #8ccbff;outline-offset:3px; }
    .btn { background:#277fd0; border:1px solid #4ca2eb; color:white; padding:12px 19px; border-radius:11px; font:600 14px system-ui,sans-serif; margin:5px; cursor:pointer; box-shadow:none; }
    .btn:hover { background:#398fe0; }
    svg { fill:#b8dcfc; background:#223750; padding:6px; border-radius:10px; box-sizing:content-box; cursor:pointer; }
    svg:hover { fill:#fff; background:#2e5f90; }
    #message { max-width:740px; margin:12px auto 0; padding:12px 15px; background:#1b2b3c; border:1px solid var(--border); border-radius:12px; text-align:center; }
    .msg { color:#c3e8ff; } .errormsg { color:#ff989d; }
    dialog { position:fixed; inset:0; width:100%; height:100%; max-width:none;max-height:none; background:var(--bg); overflow-y:auto; border:none; padding:0!important; color:var(--ink); }
    dialog::backdrop { background:#03070de8; }
    dialog .main { min-height:0; padding:24px 18px 55px; }
    dialog table { background:var(--panel); border:1px solid var(--border); border-collapse:separate; border-spacing:0; max-width:100%; width:min(100%,740px); margin:10px auto; border-radius:14px; overflow:hidden; }
    dialog td { padding:10px 12px; overflow-wrap:anywhere; }
    dialog select { max-width:350px; }
    dialog .btn { white-space:normal; }
    dialog input[type='checkbox'] { accent-color:#3d9ef3; width:18px;height:18px;vertical-align:middle; }
    #mmListView,#mmEditView { margin:0 auto;max-width:800px; text-align:center; }
    /* #mmList remains a table; overflow is handled by .mv-table-scroll. */
    #mmMessage { padding:12px;color:#b7e1ff; }
    #param { width:min(100%,760px); }
    progress { max-width:100%; }
    @media(max-width:600px) {
      .main { padding:18px 10px 28px; }
      .mv-head { padding:0 6px; }
      .mv-head strong { font-size:19px; }
      .mv-pill { font-size:11px; }
      body > center > .main > table { padding:8px; }
      body > center > .main > table td { padding:9px 5px; }
      body > center > .main > table td:first-child { width:40%; }
      b,label { font-size:13px; }
      .btn { padding:11px 13px;margin:4px; }
      dialog td { padding:9px 7px; }
      dialog input[type='text'] { width:100%; }
    }


    /* v0.2.1 layout corrections. No application logic or API changes. */
    .mv-head { max-width:860px; }
    .mv-head-title { min-width:0; }
    .mv-logo { width:142px;min-width:142px;height:54px;padding:6px 10px;display:flex;justify-content:center;align-items:center;background:#f5f9fc;border-radius:12px;overflow:hidden; }
    .mv-logo img { display:block;width:100%;max-height:46px;object-fit:contain; }
    .mv-logo-fallback { color:#17406a;font-size:18px;font-weight:900;letter-spacing:.03em; }
    .mv-intro { max-width:860px; }
    body > center > .main > table, #message { max-width:860px; }
    .mv-action-row { width:100%;display:flex;flex-wrap:wrap;justify-content:center;align-items:center;gap:10px;margin:12px auto; }
    .mv-action-row .btn { margin:0; }
    .mv-reboot-row { margin:16px auto 4px; }
    #mmListView, #mmEditView { width:100%;max-width:1100px; }
    #mDialog .main { width:min(100%,1160px); }
    #mDialog #mmListView > table { width:100%;max-width:none; }
    .mv-table-scroll { width:100%;overflow-x:auto;border-radius:14px;border:1px solid var(--border);margin:14px auto; }
    #mmList { display:table;width:100%;min-width:575px;max-width:none;margin:0;border:none;border-collapse:collapse;border-spacing:0;background:var(--panel);table-layout:auto;text-align:left; }
    #mmList th { text-align:left; padding:14px 16px;white-space:nowrap;background:#244969; }
    #mmList td { padding:10px 16px;border-top:1px solid #2b3b4f;vertical-align:middle;word-break:normal;overflow-wrap:normal; }
    #mmList tr:nth-child(even) td { background:rgba(255,255,255,.025); }
    #mmList tr:hover td { background:rgba(65,145,219,.1); }
    #mmList td:nth-child(1) { width:18%; }
    #mmList td:nth-child(2) { width:38%; }
    #mmList td:nth-child(3) { width:16%; }
    #mmList td:nth-child(4),#mmList td:nth-child(5) { width:14%;text-align:center; }
    #mmList .btn { margin:2px;padding:9px 14px;min-width:68px; }
    #mmList td:last-child .btn { background:#3b485e;border-color:#54647b; }
    #mmList td:last-child .btn:hover { background:#58647a; }
    #mDialog #mmTemplatePick { width:min(100%,440px);max-width:440px; }
    dialog .main > table { max-width:900px; }
    @media(max-width:650px) {
       .mv-head { flex-wrap:wrap;gap:10px; }
       .mv-logo { width:112px;min-width:112px;height:46px; }
       .mv-logo-fallback { font-size:14px; }
       .mv-head strong { font-size:19px; }
       .mv-action-row { gap:7px; }
       .mv-action-row .btn { flex:0 1 auto; }
       #mmList th,#mmList td { padding:9px 10px; }
       #mmList .btn { padding:8px 10px;min-width:57px; }
    }
  </style>
  <script>
    var filaments;
    var dbVersion;
    var jsonDb;
    var paramStr;
    // Retries only on a real network-level failure (timeout, dropped
    // connection, DNS failure) - never on an actual HTTP error response,
    // since that's the device telling us something specific and retrying
    // it blindly would just get the same answer. Exponential backoff
    // (1s, 2s, 4s...) gives a flaky/roaming WiFi link a moment to settle
    // before trying again, instead of hammering it immediately.
    function xhrRetry(method, url, body, contentType, onSuccess, onError, opts) {
      opts = opts || {};
      var maxRetries = opts.maxRetries != null ? opts.maxRetries : 3;
      var timeoutMs = opts.timeoutMs != null ? opts.timeoutMs : 8000;
      var attempt = 0;
      function tryOnce() {
        var xhr = new XMLHttpRequest();
        xhr.timeout = timeoutMs;
        xhr.onreadystatechange = function () {
          if (this.readyState != 4) return;
          if (this.status >= 200 && this.status < 300) {
            onSuccess(this);
          } else if (this.status == 0) {
            retryOrFail("connection dropped");
          } else {
            onError(this, "HTTP " + this.status + ": " + this.responseText);
          }
        };
        xhr.ontimeout = function () {
          retryOrFail("timed out after " + Math.round(timeoutMs / 1000) + "s");
        };
        xhr.open(method, url, true);
        if (contentType) xhr.setRequestHeader("Content-type", contentType);
        xhr.send(body);
      }
      function retryOrFail(reason) {
        attempt++;
        if (attempt > maxRetries) {
          onError(null, reason + " (gave up after " + maxRetries + " retries)");
          return;
        }
        setTimeout(tryOnce, Math.min(1000 * Math.pow(2, attempt - 1), 8000));
      }
      tryOnce();
    }
    // Same idea as xhrRetry but for the fetch()-based printer push calls, which need
    // an AbortController since fetch has no built-in timeout.
    function fetchRetry(url, fetchOpts, onSuccess, onError, opts) {
      opts = opts || {};
      var maxRetries = opts.maxRetries != null ? opts.maxRetries : 2;
      var timeoutMs = opts.timeoutMs != null ? opts.timeoutMs : 30000;
      var attempt = 0;
      function tryOnce() {
        var controller = new AbortController();
        var timer = setTimeout(function () { controller.abort(); }, timeoutMs);
        var merged = Object.assign({}, fetchOpts, { signal: controller.signal });
        fetch(url, merged).then(function (res) {
          clearTimeout(timer);
          res.text().then(function (t) {
            if (res.ok) onSuccess(res, t);
            else onError(res, "HTTP " + res.status + ": " + t);
          });
        }).catch(function (e) {
          clearTimeout(timer);
          retryOrFail(e && e.name === "AbortError" ? "timed out after " + Math.round(timeoutMs / 1000) + "s" : "network error: " + e.message);
        });
      }
      function retryOrFail(reason) {
        attempt++;
        if (attempt > maxRetries) {
          onError(null, reason + " (gave up after " + maxRetries + " retries)");
          return;
        }
        setTimeout(tryOnce, Math.min(1000 * Math.pow(2, attempt - 1), 8000));
      }
      tryOnce();
    }
    const compress = byteArray => {
      const cs = new CompressionStream('gzip');
      const writer = cs.writable.getWriter();
      writer.write(byteArray);
      writer.close();
      return new Response(cs.readable).arrayBuffer();
    };
    function sendData() {
      document.getElementById("message").innerHTML = "<label class=\"msg\">Saving spool settings...</label>";
      var materialBrand = document.getElementById("materialBrand").value;
      var materialType = document.getElementById("materialType").value;
      var materialWeight = document.getElementById("materialWeight").value;
      var materialColor = document.getElementById("materialColor").value;
      var label = document.getElementById("materialType").selectedOptions[0]?.textContent || materialType;
      var postdata = "materialType=" + encodeURIComponent(materialType) + "&materialWeight=" + encodeURIComponent(materialWeight) + "&materialColor=" + encodeURIComponent(materialColor) + "&displayBrand=" + encodeURIComponent(materialBrand) + "&displayMaterial=" + encodeURIComponent(label);
      localStorage.setItem("materialBrand", materialBrand);
      localStorage.setItem("materialType", materialType);
      localStorage.setItem("materialWeight", materialWeight);
      localStorage.setItem("materialColor", materialColor);
      xhrRetry("POST", "/spooldata", postdata, "application/x-www-form-urlencoded",
        function () {
          document.getElementById("message").innerHTML = "<label class=\"msg\">Spool settings saved</label>";
          setTimeout(function () {
            document.getElementById("message").innerHTML = "<label class=\"msg\">&nbsp;</label>";
          }, 5000);
        },
        function (xhr, reason) {
          document.getElementById("message").innerHTML = "<label class=\"errormsg\">Error: " + reason + "</label>";
        }
      );
    }
    function loadFilaments(vendor) {
      if (vendor == "") { vendor = "Generic"; }
      const objDb = JSON.parse(jsonDb);
      filaments = "";
      for (let i in objDb.result.list) {
        if (objDb.result.list[i].base.brand == vendor) {
          filaments += "<option value=\"" + objDb.result.list[i].base.id + "\">" + objDb.result.list[i].base.name + "</option>";
        }
      }
    }
    function change_types() {
      var selectedValue = document.getElementById("materialBrand").value;
      loadFilaments(selectedValue);
      document.getElementById("types").innerHTML = "<select name=\"materialType\" id=\"materialType\">" + filaments + "</select>";
    }
    function loadCache() {
      if (localStorage.getItem("materialBrand") == null) {
        loadFilaments("");
        document.getElementById("types").innerHTML = "<select name=\"materialType\" id=\"materialType\">" + filaments + "</select>";
        return;
      }
      document.getElementById("materialBrand").value = localStorage.getItem("materialBrand");
      var selectedValue = document.getElementById("materialBrand").value;
      change_types(selectedValue);
      document.getElementById("materialType").value = localStorage.getItem("materialType");
      document.getElementById("materialWeight").value = localStorage.getItem("materialWeight");
      document.getElementById("materialColor").value = localStorage.getItem("materialColor");
    }
    function clearCache() {
      localStorage.clear();
      document.getElementById("materialBrand").value = "Generic";
      change_types();
      document.getElementById("materialType").value = "00001";
      document.getElementById("materialWeight").value = "1 KG";
      document.getElementById("materialColor").value = "#0000FF";
      sendData();
    }
    function openConf() {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState == 4) {
          if (this.status == 200) {
            if (this.responseText.includes("|-|")) {
              var configData = this.responseText.split("|-|");
              document.getElementById("ap_ssid").value = configData[0];
              document.getElementById("wifi_ssid").value = configData[1];
              document.getElementById("wifi_host").value = configData[2];
              document.getElementById("printer_host").value = configData[3];
              document.getElementById("ap_enabled").checked = configData.length > 4 ? configData[4] == "1" : true;
            } else {
              document.getElementById("ap_ssid").value = "K2_RFID";
              document.getElementById("wifi_ssid").value = "";
              document.getElementById("wifi_host").value = "k2.local";
            }
            document.getElementById("cDialog").showModal();
            document.activeElement.blur();
          }
        }
      };
      xhr.open("GET", "/config", true);
      xhr.send();
    }
    function saveConf() {
      if (!document.getElementById("ap_enabled").checked && document.getElementById("wifi_ssid").value == "") {
        alert("Set a WiFi network before disabling the Access Point, or you may lose access to this device.");
        return;
      }
      var postdata = "ap_ssid=" + document.getElementById("ap_ssid").value + "&ap_pass=" + document.getElementById("ap_pass").value + "&wifi_ssid=" + document.getElementById("wifi_ssid").value + "&wifi_pass=" + document.getElementById("wifi_pass").value + "&wifi_host=" + document.getElementById("wifi_host").value + "&printer_host=" + document.getElementById("printer_host").value + (document.getElementById("ap_enabled").checked ? "&ap_enabled=1" : "") + "&submit";
      xhrRetry("POST", "/config", postdata, "application/x-www-form-urlencoded",
        function () {
          dialog.close();
          document.getElementById("message").innerHTML = "<label class=\"msg\">Config settings saved</label>";
          setTimeout(function () {
            document.getElementById("message").innerHTML = "<label class=\"msg\">&nbsp;</label>";
          }, 5000);
        },
        function (xhr, reason) {
          document.getElementById("message").innerHTML = "<label class=\"errormsg\">Error saving config: " + reason + "</label>";
        }
      );
    }
    var readPollTimer = null;
    function armWriteTag() {
      if (readPollTimer) { clearInterval(readPollTimer); readPollTimer=null; }
      fetch("/writetag",{method:"POST"}).then(async r=>{
        let msg=await r.text();if(!r.ok)throw Error(msg);
        document.getElementById("message").textContent="Present tag to WRITE (one operation only)";
      }).catch(e=>document.getElementById("message").textContent=e.message);
    }
    function cancelTagAction() {
      if(readPollTimer){clearInterval(readPollTimer);readPollTimer=null;}
      fetch("/canceltag",{method:"POST"}).then(()=>{
        document.getElementById("message").textContent="Tag operation cancelled - passive scanning active";
      });
    }

    function armReadTag() {
      document.getElementById("message").innerHTML = "<label class=\"msg\">Present a tag to read it...</label>";
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState == 4) {
          if (this.status == 200) {
            if (readPollTimer) clearInterval(readPollTimer);
            readPollTimer = setInterval(pollReadResult, 1000);
          } else {
            document.getElementById("message").innerHTML = "<label class=\"errormsg\">Error: " + this.responseText + "</label>";
          }
        }
      };
      xhr.open("POST", "/readtag", true);
      xhr.send();
    }
    function findBrandForMaterialId(id) {
      try {
        const objDb = JSON.parse(jsonDb);
        for (const item of objDb.result.list) {
          if (item && item.base && String(item.base.id) == String(id)) return item.base.brand;
        }
      } catch (e) {}
      return null;
    }
    function pollReadResult() {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var r;
          try { r = JSON.parse(this.responseText); } catch (e) { return; }
          if (!r.ready) return;
          clearInterval(readPollTimer);
          readPollTimer = null;
          if (r.error) {
            document.getElementById("message").innerHTML = "<label class=\"errormsg\">Unrecognized or blank tag</label>";
            return;
          }
          var brand = findBrandForMaterialId(r.materialType);
          if (brand) {
            document.getElementById("materialBrand").value = brand;
            change_types();
          }
          document.getElementById("materialType").value = r.materialType;
          document.getElementById("materialWeight").value = r.weight;
          document.getElementById("materialColor").value = "#" + r.color;
          // Resolve the tag's material ID to a friendly TFT label without modifying it.
          var friendly = document.getElementById("materialType").selectedOptions[0];
          var displayName = friendly && friendly.value == r.materialType ? friendly.textContent : r.materialType;
          var labelBody = "materialId=" + encodeURIComponent(r.materialType) + "&brand=" + encodeURIComponent(brand || "") + "&name=" + encodeURIComponent(displayName);
          fetch("/display/resolve", {method:"POST",headers:{"Content-Type":"application/x-www-form-urlencoded"},body:labelBody}).catch(function(){});
          document.getElementById("message").innerHTML = "<label class=\"msg\">Read tag " + r.tagId +
            (r.printerType ? " (written by: " + r.printerType + ")" : "") + "</label>";
          setTimeout(function () {
            document.getElementById("message").innerHTML = "<label class=\"msg\">&nbsp;</label>";
          }, 6000);
        }
      };
      xhr.open("GET", "/readresult", true);
      xhr.send();
    }
    function loadVendors() {
      try {
        const objDb = JSON.parse(jsonDb);
        let vendors = "";
        const seenBrands = new Set();
        for (const item of objDb.result.list) {
          if (item && item.base && item.base.brand && !seenBrands.has(item.base.brand)) {
            vendors += "<option value=\"" + item.base.brand + "\">" + item.base.brand + "</option>";
            seenBrands.add(item.base.brand);
          }
        }
        document.getElementById("brands").innerHTML = "<select name=\"materialBrand\" id=\"materialBrand\" onchange=\"change_types()\">" + vendors + "</select>";
      } catch (error) {
        document.getElementById("brands").innerHTML = "<select name=\"materialBrand\" id=\"materialBrand\" onchange=\"change_types()\"><option value=\"Generic\">Generic</option><option value=\"Creality\">Creality</option></select>";
      }
    }
    function loadDb() {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState == 4) {
          if (this.status == 200) {
            if (this.responseText.includes('"kvParam":')) {
              jsonDb = this.responseText;
              const objDb = JSON.parse(jsonDb);
              dbVersion = objDb.result.version;


              loadVendors();
              loadCache();
            } else {
              document.getElementById("message").innerHTML = "<label class=\"errormsg\">Error: failed to load material_database.json</label>";
            }
          }
        }
      };
      xhr.open("GET", "/material_database.json", true);
      xhr.send();
    }
    function loadParam() {
      var matID = document.getElementById("materialType").value;
      const objDb = JSON.parse(jsonDb);
      for (let i in objDb.result.list) {
        if (objDb.result.list[i].base.id == matID) {
          paramStr = "<tr><td></td><td><br>";
          paramStr += "<label style=\"display: block;text-align: right;\">";
          paramStr += " <svg xmlns=\"http://www.w3.org/2000/svg\" height=\"32px\" viewBox=\"0 0 24 24\" width=\"32px\"";
          paramStr += "onClick=\"dialogp.close();\">";
          paramStr += "<path d=\"M0 0h24v24H0z\" fill=\"none\" />";
          paramStr += "<path d=\"M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z\" />";
          paramStr += "</svg></label></td></tr>";
          paramStr += "<th colspan=\"2\"><b>Filament Type: " + objDb.result.list[i].base.name + "</b></th>";
          for (var key in objDb.result.list[i].kvParam) {
            paramStr += "<tr><td><b>" + key + "</b></td><td width=50px><b>" + objDb.result.list[i].kvParam[key] + "</b></td></tr>";
          }
        }
      }
      paramStr += "</tr>";
      document.getElementById("param").innerHTML = paramStr;
      document.getElementById("pDialog").showModal();
      document.activeElement.blur();
    }
    function loadUpdate() {
      document.getElementById("uDialog").showModal();
      document.activeElement.blur();
    }
    var matDbObj = null;
    var currentEditIndex = -1;
    function persistLocal() {
      // Sent as a file upload (not a raw JSON body) so the ESP8266 can
      // write it to flash in chunks as it arrives, instead of needing to
      // hold the whole ~300KB+ file in RAM first.
      var blob = new Blob([jsonDb], { type: "application/json" });
      var formData = new FormData();
      formData.append("file", blob, "material_database.json");
      xhrRetry("POST", "/savedb", formData, null,
        function () { /* saved silently, callers already show their own message */ },
        function (xhr, reason) { mmMsg("Local save failed: " + reason, true); },
        { timeoutMs: 30000 }
      );
    }
    function mmMsg(text, isError) {
      document.getElementById("mmMessage").innerHTML =
        "<label class=\"" + (isError ? "errormsg" : "msg") + "\">" + text + "</label>";
    }
    function generateUniqueMaterialId() {
      // Creality's own catalog uses 00001-19xxx (Generic is 000xx, their
      // named product lines are 01xxx-19xxx). Custom entries live up in the
      // 90000-99999 range instead so they can never collide with those, or
      // with anything a future official firmware update might add.
      var used = {};
      for (var i = 0; i < matDbObj.result.list.length; i++) {
        used[matDbObj.result.list[i].base.id] = true;
      }
      for (var n = 90001; n <= 99999; n++) {
        var candidate = String(n);
        if (!used[candidate]) return candidate;
      }
      return "";
    }
    function openMaterialManager() {
      if (!jsonDb) {
        mmMsg("Database not loaded yet", true);
        return;
      }
      matDbObj = JSON.parse(jsonDb);
      document.getElementById("mmMessage").innerHTML = "";
      document.getElementById("mmEditView").style.display = "none";
      document.getElementById("mmListView").style.display = "block";
      document.getElementById("mmSync").style.display =
        document.getElementById("printer_host").value.length > 0 ? "inline-block" : "none";
      renderMaterialList();
      document.getElementById("mDialog").showModal();
      document.activeElement.blur();
    }
    function renderMaterialList() {
      var rows = "<tr><th>Brand</th><th>Name</th><th>Type</th><th></th><th></th></tr>";
      var list = matDbObj.result.list;
      var templateOpts = "<option value=\"-1\">-- Blank --</option>";
      for (var i = 0; i < list.length; i++) {
        var b = list[i].base;
        rows += "<tr><td>" + b.brand + "</td><td>" + b.name + "</td><td>" + b.meterialType +
          "</td><td><input type=\"button\" class=\"btn\" value=\"Edit\" onClick=\"editMaterialItem(" + i + ")\"></td>" +
          "<td><input type=\"button\" class=\"btn\" value=\"Delete\" onClick=\"deleteMaterialItem(" + i + ")\"></td></tr>";
        templateOpts += "<option value=\"" + i + "\">" + b.brand + " " + b.name + " (" + b.meterialType + ")</option>";
      }
      document.getElementById("mmList").innerHTML = rows;
      document.getElementById("mmTemplatePick").innerHTML = templateOpts;
      if (list.length > 0) document.getElementById("mmTemplatePick").value = "0";
    }
    function deleteMaterialItem(i) {
      var item = matDbObj.result.list[i];
      if (!confirm("Delete \"" + item.base.name + "\" (" + item.base.brand + ")?")) return;
      matDbObj.result.list.splice(i, 1);
      matDbObj.result.count = matDbObj.result.list.length;
      jsonDb = JSON.stringify(matDbObj, null, 2);
      persistLocal();
      loadVendors();
      renderMaterialList();
      mmMsg("Deleted. Remember to Push to Printer to make this live.", false);
    }
    function newMaterialItem() {
      // Clone the picked existing item as a starting template (full kvParam
      // set is large - editing an existing profile is far more practical
      // than authoring one from scratch) - every field stays editable below.
      var pick = parseInt(document.getElementById("mmTemplatePick").value);
      var template = (pick >= 0 && matDbObj.result.list[pick]) ?
        JSON.parse(JSON.stringify(matDbObj.result.list[pick])) :
        { base: { id: "", brand: "", name: "", meterialType: "", colors: ["#ffffff"], density: 1.24,
          diameter: "1.75", costPerMeter: 0, weightPerMeter: 0, rank: 10000, minTemp: 190, maxTemp: 240,
          isSoluble: false, isSupport: false, shrinkageRate: 0, softeningTemp: 0, dryingTemp: 0, dryingTime: 0 },
          engineVersion: "3.0.0", printerIntName: "", nozzleDiameter: ["0.4"], kvParam: {} };
      template.base.id = generateUniqueMaterialId();
      template.base.name = "";
      editMaterialItem(-1, template);
    }
    function editMaterialItem(i, newItemTemplate) {
      currentEditIndex = i;
      var item = i >= 0 ? matDbObj.result.list[i] : newItemTemplate;
      var b = item.base;
      var f = "<tr><th colspan=\"2\"><center>" + (i >= 0 ? "Edit Material" : "New Material") + "</center></th></tr>";
      f += mmField("base_id", "Material ID", b.id);
      f += mmField("base_brand", "Brand", b.brand);
      f += mmField("base_name", "Name", b.name);
      f += mmField("base_meterialType", "Material Type", b.meterialType);
      f += mmField("base_colors0", "Color", (b.colors && b.colors[0]) ? b.colors[0] : "#ffffff", "color");
      f += mmField("base_density", "Density (g/cm3)", b.density);
      f += mmField("base_diameter", "Diameter (mm)", b.diameter);
      f += mmField("base_minTemp", "Min Nozzle Temp", b.minTemp);
      f += mmField("base_maxTemp", "Max Nozzle Temp", b.maxTemp);
      f += mmField("base_costPerMeter", "Cost Per Meter", b.costPerMeter);
      f += mmField("base_weightPerMeter", "Weight Per Meter", b.weightPerMeter);
      f += mmField("base_rank", "Rank", b.rank);
      f += mmField("base_isSoluble", "Soluble", b.isSoluble, "checkbox");
      f += mmField("base_isSupport", "Support Material", b.isSupport, "checkbox");
      f += mmField("base_shrinkageRate", "Shrinkage Rate", b.shrinkageRate);
      f += mmField("base_softeningTemp", "Softening Temp", b.softeningTemp);
      f += mmField("base_dryingTemp", "Drying Temp", b.dryingTemp);
      f += mmField("base_dryingTime", "Drying Time", b.dryingTime);
      f += "<tr><th colspan=\"2\"><center>Slicer Parameters (kvParam)</center></th></tr>";
      var keys = Object.keys(item.kvParam || {}).sort();
      for (var k = 0; k < keys.length; k++) {
        f += mmField("kv_" + keys[k], keys[k], item.kvParam[keys[k]]);
      }
      document.getElementById("mmEditForm").innerHTML = f;
      document.getElementById("mmListView").style.display = "none";
      document.getElementById("mmEditView").style.display = "block";
    }
    function mmField(id, label, value, type) {
      type = type || "text";
      if (type == "checkbox") {
        return "<tr><td>" + label + "</td><td><input type=\"checkbox\" id=\"" + id + "\"" +
          (value ? " checked" : "") + "></td></tr>";
      }
      if (type == "color") {
        return "<tr><td>" + label + "</td><td><input type=\"color\" id=\"" + id + "\" value=\"" + value + "\"></td></tr>";
      }
      return "<tr><td>" + label + "</td><td><input type=\"text\" id=\"" + id + "\" value=\"" +
        String(value).replace(/"/g, "&quot;") + "\"></td></tr>";
    }
    function cancelMaterialEdit() {
      document.getElementById("mmEditView").style.display = "none";
      document.getElementById("mmListView").style.display = "block";
    }
    function saveMaterialEdit() {
      var item = currentEditIndex >= 0 ? matDbObj.result.list[currentEditIndex] :
        JSON.parse(JSON.stringify(matDbObj.result.list.length > 0 ? matDbObj.result.list[0] : { base: {}, kvParam: {} }));
      var b = item.base;
      b.id = document.getElementById("base_id").value;
      b.brand = document.getElementById("base_brand").value;
      b.name = document.getElementById("base_name").value;
      b.meterialType = document.getElementById("base_meterialType").value;
      b.colors = [document.getElementById("base_colors0").value];
      b.density = parseFloat(document.getElementById("base_density").value) || 0;
      b.diameter = document.getElementById("base_diameter").value;
      b.minTemp = parseInt(document.getElementById("base_minTemp").value) || 0;
      b.maxTemp = parseInt(document.getElementById("base_maxTemp").value) || 0;
      b.costPerMeter = parseFloat(document.getElementById("base_costPerMeter").value) || 0;
      b.weightPerMeter = parseFloat(document.getElementById("base_weightPerMeter").value) || 0;
      b.rank = parseInt(document.getElementById("base_rank").value) || 0;
      b.isSoluble = document.getElementById("base_isSoluble").checked;
      b.isSupport = document.getElementById("base_isSupport").checked;
      b.shrinkageRate = parseFloat(document.getElementById("base_shrinkageRate").value) || 0;
      b.softeningTemp = parseFloat(document.getElementById("base_softeningTemp").value) || 0;
      b.dryingTemp = parseFloat(document.getElementById("base_dryingTemp").value) || 0;
      b.dryingTime = parseFloat(document.getElementById("base_dryingTime").value) || 0;
      if (!b.id) {
        mmMsg("Material ID is required", true);
        return;
      }
      if (b.id.length != 5) {
        mmMsg("Material ID must be exactly 5 digits", true);
        return;
      }
      if (!/^[0-9]{5}$/.test(b.id)) {
        mmMsg("Material ID must be numeric (0-9 only)", true);
        return;
      }
      for (var d = 0; d < matDbObj.result.list.length; d++) {
        if (d != currentEditIndex && matDbObj.result.list[d].base.id == b.id) {
          mmMsg("Material ID " + b.id + " already exists", true);
          return;
        }
      }
      if (!b.brand) {
        mmMsg("Brand is required", true);
        return;
      }
      if (!b.name) {
        mmMsg("Name is required", true);
        return;
      }
      if (!b.meterialType) {
        mmMsg("Material Type is required", true);
        return;
      }
      var keys = Object.keys(item.kvParam || {});
      for (var k = 0; k < keys.length; k++) {
        var el = document.getElementById("kv_" + keys[k]);
        if (el) item.kvParam[keys[k]] = el.value;
      }
      if (currentEditIndex >= 0) {
        matDbObj.result.list[currentEditIndex] = item;
      } else {
        matDbObj.result.list.push(item);
      }
      matDbObj.result.count = matDbObj.result.list.length;
      // The Android app's own "prevent DB updates" option hardcodes this
      // exact value instead of a normal version number, specifically to
      // stop whatever mechanism regenerates this file on the printer's
      // own boot sequence from overwriting local edits. A plain Unix
      // timestamp here is apparently not enough to be treated as
      // authoritative - this sentinel is.
      matDbObj.result.version = "9876543210";
      jsonDb = JSON.stringify(matDbObj, null, 2);
      persistLocal();
      loadVendors();
      cancelMaterialEdit();
      renderMaterialList();
      mmMsg("Saved locally. Use Push to Printer to make this live.", false);
    }
    function pushAgentBase() {
      return window.location.protocol + "//" + window.location.hostname + ":8081";
    }
    function syncLiveFromPrinter() {
      if (!document.getElementById("printer_host").value) { mmMsg("Set the printer host in settings first", true); return; }
      mmMsg("Syncing live database from printer...", false);
      fetchRetry(pushAgentBase() + "/getdb", {},
        function (res, text) {
          if (text.indexOf('"kvParam":') === -1) { mmMsg("Sync failed: unexpected response", true); return; }
          jsonDb = text;
          matDbObj = JSON.parse(jsonDb);
          persistLocal();
          loadVendors();
          renderMaterialList();
          mmMsg("Synced live database from printer.", false);
        },
        function (res, reason) { mmMsg("Sync failed: " + reason, true); },
        { timeoutMs: 30000, maxRetries: 2 }
      );
    }
    function pushToPrinter() {
      if (!document.getElementById("printer_host").value) { mmMsg("Set the printer host in settings first", true); return; }
      var reboot = document.getElementById("mmReboot").checked;
      if (!confirm("Push the edited database to the printer" + (reboot ? " and reboot it" : "") + "?")) return;

      // Never push whatever's sitting in the browser's local copy as-is -
      // it may be stale relative to the printer if entries were added or
      // changed there since the last sync (e.g. via the Android app).
      // Always re-fetch the live database right before pushing and merge
      // this session's edits onto it, so nothing that exists live but
      // isn't in the local copy ever gets silently dropped.
      mmMsg("Re-checking live database before push...", false);
      var localEdits = matDbObj.result.list;
      fetchRetry(pushAgentBase() + "/getdb", {},
        function (res, text) {
          if (text.indexOf('"kvParam":') === -1) { mmMsg("Push aborted: unexpected response re-fetching live database", true); return; }
          var freshObj = JSON.parse(text);
          var freshList = freshObj.result.list;
          var freshIndexById = {};
          for (var i = 0; i < freshList.length; i++) {
            freshIndexById[freshList[i].base.id] = i;
          }
          var added = 0, updated = 0;
          for (var j = 0; j < localEdits.length; j++) {
            var id = localEdits[j].base.id;
            if (freshIndexById.hasOwnProperty(id)) {
              freshList[freshIndexById[id]] = localEdits[j];
              updated++;
            } else {
              freshList.push(localEdits[j]);
              added++;
            }
          }
          freshObj.result.count = freshList.length;
          // Same sentinel as the direct-save path above - see that
          // comment for why a plain timestamp isn't enough here.
          freshObj.result.version = "9876543210";
          jsonDb = JSON.stringify(freshObj, null, 2);
          matDbObj = freshObj;
          persistLocal();
          renderMaterialList();

          mmMsg("Pushing merged database (" + added + " added, " + updated + " updated, " + freshList.length + " total)...", false);
          fetchRetry(pushAgentBase() + "/pushdb" + (reboot ? "?reboot=1" : ""),
            { method: "POST", headers: { "Content-Type": "text/plain" }, body: jsonDb },
            function (res2, text2) { mmMsg("Pushed to printer: " + text2, false); },
            function (res2, reason2) { mmMsg("Push failed: " + reason2, true); },
            { timeoutMs: 30000, maxRetries: 2 }
          );
        },
        function (res, reason) { mmMsg("Push aborted: could not re-fetch live database first (" + reason + ")", true); },
        { timeoutMs: 30000, maxRetries: 2 }
      );
    }
    function loadDbUpdate() {
      document.getElementById("dDialog").showModal();
      document.activeElement.blur();
      if (document.getElementById("printer_host").value.length > 0) {
        document.getElementById("pupload").style.display = "block";
      } else {
        document.getElementById("pupload").style.display = "none";
      }
      document.getElementById("dupload").style.display = "none";
      document.getElementById("dselfile").innerHTML = "";
      document.getElementById("dstatus").innerHTML = "";
      document.getElementById("dbtnsel").style.display = "block";
      document.getElementById('dbfile').value = "";
    }
    function formatBytes(bytes) {
      if (bytes == 0) return '0 Bytes'; var k = 1024, dm = 2, sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'], i = Math.floor(Math.log(bytes) / Math.log(k)); return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
    }
    function FileSelected(e) {
      file = document.getElementById('fwfile').files[document.getElementById('fwfile').files.length - 1];
      if (file.name.toLowerCase().endsWith(".bin")) {
        var b = file.slice(0, 1);
        var r = new FileReader(); r.onloadend = function (e) {
          if (e.target.readyState === FileReader.DONE) {
            var mb = new Uint8Array(e.target.result);
            if (parseInt(mb[0]) == 233) {
              document.getElementById("selfile").innerHTML = "File: " + file.name + "<br>Size: " + formatBytes(file.size) + "<br>Magic byte: 0x" + parseInt(mb[0]).toString(16).toUpperCase();
              document.getElementById("upload").style.display = "block";
            } else {
              document.getElementById("selfile").innerHTML = "<font color='#df3840'>Invalid firmware file</font><br><br>Magic byte is wrong<br>Expected: 0xE9<br>Found: 0x" + parseInt(mb[0]).toString(16).toUpperCase();
              document.getElementById("upload").style.display = "none";
            }
          }
        }; r.readAsArrayBuffer(b);
      } else {
        document.getElementById("selfile").innerHTML = "<font color='#df3840'>Invalid firmware file</font><br><br>File should be a .bin file";
        document.getElementById("upload").style.display = "none";
      }
    }
    function DbFileSelected(e) {
      file = document.getElementById('dbfile').files[document.getElementById('dbfile').files.length - 1];
      if (file.name.toLowerCase() == "material_database.json") {
        document.getElementById("dselfile").innerHTML = "File: " + file.name + "<br>Size: " + formatBytes(file.size);
        document.getElementById("dupload").style.display = "block";
        document.getElementById("pupload").style.display = "none";
      } else {
        document.getElementById("dselfile").innerHTML = "<font color='#df3840'>Invalid database file</font><br><br>File should be material_database.json";
        document.getElementById("dupload").style.display = "none";
      }
    }
    function uploadFile() {
      document.getElementById("upload").style.display = "none";
      document.getElementById("btnsel").style.display = "none";
      document.getElementById("status").innerHTML = "<br><progress max='100' value='0'>0%</progress><br><br>Uploading firmware file";
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState === XMLHttpRequest.DONE) {
          if (this.status === 200) {
            document.getElementById("status").innerHTML = this.responseText;
          } else {
            document.getElementById("status").innerHTML = "<font color='#df3840'>HTTP Error</font>";
          }
          setTimeout(redirect, 8000);
        }
      };
      xhr.upload.onprogress = function (e) {
        var percentComplete = Math.ceil((e.loaded / e.total) * 100);
        document.getElementById("status").innerHTML = "<br><progress max='100' value='" + percentComplete + "'>" + percentComplete + "%</progress><br><br>Uploading firmware file";
      };
      xhr.upload.onloadend = function (e) {
        document.getElementById("status").innerHTML = "<div id='loader'></div><br>Updating firmware, Please wait.";
      };
      xhr.open("POST", "/update.html", true);
      var formData = new FormData();
      formData.append("fwfile", file);
      xhr.send(formData);
    }
    function uploadDb() {
      document.getElementById("dupload").style.display = "none";
      document.getElementById("dbtnsel").style.display = "none";
      document.getElementById("dstatus").innerHTML = "<br><progress max='100' value='0'>0%</progress><br><br>Uploading database file";
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function () {
        if (this.readyState === XMLHttpRequest.DONE) {
          if (this.status === 200) {
            document.getElementById("dstatus").innerHTML = this.responseText;
          } else {
            document.getElementById("dstatus").innerHTML = "<font color='#df3840'>HTTP Error</font>";
          }
          setTimeout(redirect, 8000);
        }
      };
      xhr.upload.onprogress = function (e) {
        var percentComplete = Math.ceil((e.loaded / e.total) * 100);
        document.getElementById("dstatus").innerHTML = "<br><progress max='100' value='" + percentComplete + "'>" + percentComplete + "%</progress><br><br>Uploading database file";
      };
      xhr.upload.onloadend = function (e) {
        document.getElementById("dstatus").innerHTML = "<div id='loader'></div><br>Updating database, Please wait.";
      };
      xhr.open("POST", "/updatedb.html", true);
      let read = new FileReader();
      read.readAsArrayBuffer(file);
      read.onloadend = function () {
        Promise.resolve(read.result)
          .then(v => compress(v))
          .then(v => {
            var formData = new FormData();
            const bytes = new Uint8Array(v);
            const blob = new Blob([bytes], { type: 'application/octet-stream' });
            formData.append('dbfile', blob, file.name);
            xhr.send(formData);
          });
      }
    }
    function downloadDb() {
      var xhr = new XMLHttpRequest();
      return new Promise((resolve, reject) => {
        xhr.onreadystatechange = function () {
          if (this.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
              resolve(xhr.responseText);
            } else {
              resolve("");
            }
          }
        };
        xhr.open('GET', 'http://' + document.getElementById("printer_host").value + '/downloads/defData/material_database.json');
        xhr.timeout = 5000;
        xhr.send();
      });
    }
    const uploadDbFromPrinter = async () => {
      const res = await downloadDb();
      if (res.length > 0) {
        document.getElementById("dupload").style.display = "none";
        document.getElementById("dbtnsel").style.display = "none";
        document.getElementById("pupload").style.display = "none";
        document.getElementById("dstatus").innerHTML = "<br><progress max='100' value='0'>0%</progress><br><br>Uploading database file";
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function () {
          if (this.readyState === XMLHttpRequest.DONE) {
            if (this.status === 200) {
              document.getElementById("dstatus").innerHTML = this.responseText;
            } else {
              document.getElementById("pupload").style.display = "block";
              document.getElementById("dstatus").innerHTML = "<font color='#df3840'>HTTP Error</font>";
            }
            setTimeout(redirect, 8000);
          }
        };
        xhr.upload.onprogress = function (e) {
          var percentComplete = Math.ceil((e.loaded / e.total) * 100);
          document.getElementById("dstatus").innerHTML = "<br><progress max='100' value='" + percentComplete + "'>" + percentComplete + "%</progress><br><br>Uploading database file";
        };
        xhr.upload.onloadend = function (e) {
          document.getElementById("dstatus").innerHTML = "<div id='loader'></div><br>Updating database, Please wait.";
        };
        xhr.open("POST", "/updatedb.html", true);
        Promise.resolve(new TextEncoder().encode(res))
          .then(v => compress(v))
          .then(v => {
            var formData = new FormData();
            const bytes = new Uint8Array(v);
            const blob = new Blob([bytes], { type: 'application/octet-stream' });
            formData.append('dbfile', blob, 'material_database.json');
            xhr.send(formData);
          });
      }
      else {
        document.getElementById("dstatus").innerHTML = "<font color='#df3840'>Error downloading file from printer</font>";
      }
    }
    function redirect() {
      window.location.href = "/";
    }
    window.addEventListener("DOMContentLoaded", function () {
      loadDb();
    });
  </script>
</head>
<body>
  <center>
    <div class="main">
      <header class="mv-head">
        <div class="mv-head-title">
          <span class="mv-logo" aria-label="Creality logo">
            <img src="https://commons.wikimedia.org/wiki/Special:Redirect/file/CREALITY_LOGO.png?width=330" alt="Creality" onerror="this.style.display='none';this.nextElementSibling.style.display='block';">
            <span class="mv-logo-fallback" style="display:none">CREALITY</span>
          </span>
          <div><strong>CFS RFID</strong><span class="mv-sub">Filament manager - XIAO ESP32-C6</span></div>
        </div>
      </header>
      <p class="mv-intro">Select and apply filament settings, or read an existing CFS tag. Manage templates and printer synchronisation under Settings &gt; Material Database.</p>
      <table>
        <tr>
          <td>
            <br>
            <label style="display: block;text-align: left;">
              <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                onClick="loadParam();">
                <path d="M0 0h24v24H0V0z" fill="none" />
                <path
                  d="M11 7h2v2h-2zm0 4h2v6h-2zm1-9C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z" />
              </svg>
            </label>
          </td>
          <td>
            <br>
            <label style="display: block;text-align: right;">
              <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                onClick="openConf();">
                <path d="M0,0h24v24H0V0z" fill="none" />
                <path
                  d="M19.14,12.94c0.04-0.3,0.06-0.61,0.06-0.94c0-0.32-0.02-0.64-0.07-0.94l2.03-1.58c0.18-0.14,0.23-0.41,0.12-0.61 l-1.92-3.32c-0.12-0.22-0.37-0.29-0.59-0.22l-2.39,0.96c-0.5-0.38-1.03-0.7-1.62-0.94L14.4,2.81c-0.04-0.24-0.24-0.41-0.48-0.41 h-3.84c-0.24,0-0.43,0.17-0.47,0.41L9.25,5.35C8.66,5.59,8.12,5.92,7.63,6.29L5.24,5.33c-0.22-0.08-0.47,0-0.59,0.22L2.74,8.87 C2.62,9.08,2.66,9.34,2.86,9.48l2.03,1.58C4.84,11.36,4.8,11.69,4.8,12s0.02,0.64,0.07,0.94l-2.03,1.58 c-0.18,0.14-0.23,0.41-0.12,0.61l1.92,3.32c0.12,0.22,0.37,0.29,0.59,0.22l2.39-0.96c0.5,0.38,1.03,0.7,1.62,0.94l0.36,2.54 c0.05,0.24,0.24,0.41,0.48,0.41h3.84c0.24,0,0.44-0.17,0.47-0.41l0.36-2.54c0.59-0.24,1.13-0.56,1.62-0.94l2.39,0.96 c0.22,0.08,0.47,0,0.59-0.22l1.92-3.32c0.12-0.22,0.07-0.47-0.12-0.61L19.14,12.94z M12,15.6c-1.98,0-3.6-1.62-3.6-3.6 s1.62-3.6,3.6-3.6s3.6,1.62,3.6,3.6S13.98,15.6,12,15.6z" />
              </svg>
            </label>
          </td>
        </tr>
        <tr>
          <th colspan="2" style="background-color:#E0E0E0">
            <center>&nbsp;</center>
          </th>
        </tr>
        <tr>
          <td>
            <b>Filament Brand:</b>
          </td>
          <td>
            <div class="brands" id="brands">
              <select name="materialBrand" id="materialBrand"></select>
            </div>
          </td>
        </tr>
        <tr>
          <td>
            <b>Filament Type:</b>
          </td>
          <td>
            <div class="types" id="types">
              <select name=\"materialType\" id=\"materialType\"></select>
            </div>
          </td>
        </tr>
        <tr>
          <td>
            <b>Spool Size:</b>
          </td>
          <td>
            <select name="materialWeight" id="materialWeight">
              <option value="1 KG">1 KG</option>
              <option value="750 G">750 G</option>
              <option value="600 G">600 G</option>
              <option value="500 G">500 G</option>
              <option value="250 G">250 G</option>
            </select>
          </td>
        </tr>
        <tr>
          <td>
            <b>Filament Color:</b>
          </td>
          <td>
            <input name="materialColor" id="materialColor" type="color" value="#0000FF" list="commonColors" />
            <datalist id="commonColors">
              <option value="#1200F6" />
              <option value="#3894E1" />
              <option value="#FEFF01" />
              <option value="#F8D531" />
              <option value="#F38E24" />
              <option value="#52D048" />
              <option value="#00FEBE" />
              <option value="#B700F3" />
              <option value="#EE301A" />
              <option value="#FA5959" />
              <option value="#FFFFFF" />
              <option value="#D8D8D8" />
              <option value="#4C4C4C" />
              <option value="#782543" />
              <option value="#000000" />
            </datalist>
          </td>
        </tr>
      </table>
      <br>
      <div id="message"><label class="msg">&nbsp;</label></div>
      <br>
      <div class="mv-action-row">
        <input type="submit" class="btn" value="Reset" onClick="clearCache()">
        <input type="submit" class="btn" value="Apply" onClick="sendData()">
        <input type="button" class="btn" value="Read Tag" onClick="armReadTag()">
        <input type="button" class="btn" value="Write Tag" onClick="armWriteTag()">
        <input type="button" class="btn" value="Cancel" onClick="cancelTagAction()">
      </div>
    </div>
  </center>
  <dialog id="cDialog">
    <center>
      <div class="main">
        <table>
          <tr>
            <td></td>
            <td>
              <br>
              <label style="display: block;text-align: right;">
                <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                  onClick="loadDbUpdate();">
                  <path
                    d="M4,7v2c0,0.55-0.45,1-1,1H2v4h1c0.55,0,1,0.45,1,1v2c0,1.65,1.35,3,3,3h3v-2H7c-0.55,0-1-0.45-1-1v-2 c0-1.3-0.84-2.42-2-2.83v-0.34C5.16,11.42,6,10.3,6,9V7c0-0.55,0.45-1,1-1h3V4H7C5.35,4,4,5.35,4,7z" />
                  <path
                    d="M21,10c-0.55,0-1-0.45-1-1V7c0-1.65-1.35-3-3-3h-3v2h3c0.55,0,1,0.45,1,1v2c0,1.3,0.84,2.42,2,2.83v0.34 c-1.16,0.41-2,1.52-2,2.83v2c0,0.55-0.45,1-1,1h-3v2h3c1.65,0,3-1.35,3-3v-2c0-0.55,0.45-1,1-1h1v-4H21z" />
                </svg>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

                <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                  onClick="openMaterialManager();">
                  <path d="M0 0h24v24H0V0z" fill="none" />
                  <path
                    d="M3 17.25V21h3.75L17.81 9.94l-3.75-3.75L3 17.25zM20.71 7.04c.39-.39.39-1.02 0-1.41l-2.34-2.34a.9959.9959 0 0 0-1.41 0l-1.83 1.83 3.75 3.75 1.83-1.83z" />
                </svg>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

                <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                  onClick="loadUpdate();">
                  <path
                    d="M14,2H6C4.9,2,4.01,2.9,4.01,4L4,20c0,1.1,0.89,2,1.99,2H18c1.1,0,2-0.9,2-2V8L14,2z M18,20H6V4h7v5h5V20z M8,15.01 l1.41,1.41L11,14.84V19h2v-4.16l1.59,1.59L16,15.01L12.01,11L8,15.01z" />
                </svg>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                  onClick="dialog.close();">
                  <path d="M0 0h24v24H0z" fill="none" />
                  <path d="M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z" />
                </svg>
              </label>
            </td>
          </tr>
          <tr>
            <th colspan="2">
              <center>Access Point</center>
            </th>
          </tr>
          <tr>
            <td>ENABLE AP:</td>
            <td>
              <input type="checkbox" id="ap_enabled" checked>
            </td>
          </tr>
          <tr>
            <td>AP SSID:</td>
            <td>
              <input size="10" maxlength="32" type="text" id="ap_ssid" value="" autocomplete="off">
            </td>
          </tr>
          <tr>
            <td>AP PASSWORD:</td>
            <td>
              <input size="10" maxlength="63" minlength="8" type="text" id="ap_pass" value="********"
                autocomplete="off">
            </td>
          </tr>
          <tr>
            <th colspan="2">
              <center>Wifi Connection</center>
            </th>
          </tr>
          <tr>
            <td>WIFI SSID:</td>
            <td>
              <input size="10" maxlength="32" type="text" id="wifi_ssid" value="" autocomplete="off">
            </td>
          </tr>
          <tr>
            <td>WIFI PASSWORD:</td>
            <td>
              <input size="10" maxlength="63" minlength="8" type="text" id="wifi_pass" value="********"
                autocomplete="off">
            </td>
          </tr>
          <tr>
            <td>WIFI HOSTNAME:</td>
            <td>
              <input size="10" maxlength="16" type="text" id="wifi_host" value="" autocomplete="off">
            </td>
          </tr>
          <tr>
            <th colspan="2">
              <center>Printer</center>
            </th>
          </tr>
          <tr>
            <td>PRINTER HOST/IP:</td>
            <td>
              <input size="10" maxlength="64" type="text" id="printer_host" value="" autocomplete="off">
            </td>
          </tr>
        </table>
        <div class="mv-action-row mv-settings-actions">
          <input id="savecfg" type="button" class="btn" value="Save Settings" onClick="saveConf();">
        </div>
      </div>
    </center>
  </dialog>
  <dialog id="pDialog">
    <center>
      <table id="param"></table>
    </center>
  </dialog>
  <dialog id="uDialog">
    <center>
      <div class="main">
        <table>
          <tr>
            <td>
              <br>
              <label style="display: block;text-align: right;">
                <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                  onClick="dialogu.close();">
                  <path d="M0 0h24v24H0z" fill="none" />
                  <path d="M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z" />
                </svg>
              </label>
            </td>
          </tr>
          <tr>
            <th colspan="1" width="300">
              <center>Firmware Updater</center>
            </th>
          </tr>
          <tr>
            <td>
              <form>
                <center><input id="btnsel" type="button" class="btn" onclick="document.getElementById('fwfile').click()"
                    value="Select file" style="display: block;"></center>
                <p id="selfile"></p>
                <input id="fwfile" type="file" name="fwupdate" size="0" accept=".bin" onChange="FileSelected();"
                  style="width:0; height:0;"></p>
                <div>
                  <center>
                    <p id="status"></p>
                  </center>
                  <center><input id="upload" type="button" class="btn" value="Update Firmware" onClick="uploadFile();"
                      style="display: none;"></center>
                </div>
              </form>
            </td>
          </tr>
        </table>
      </div>
    </center>
  </dialog>
  <dialog id="dDialog">
    <center>
      <div class="main">
        <table>
          <tr>
            <td>
              <br>
              <label style="display: block;text-align: right;">
                <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                  onClick="dialogd.close();">
                  <path d="M0 0h24v24H0z" fill="none" />
                  <path d="M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z" />
                </svg>
              </label>
            </td>
          </tr>
          <tr>
            <th colspan="1" width="300">
              <center>Database Updater</center>
            </th>
          </tr>
          <tr>
            <td>
              <form>
                <center><input id="dbtnsel" type="button" class="btn"
                    onclick="document.getElementById('dbfile').click()" value="Select file" style="display: block;">
                </center>
                <p id="dselfile"></p>
                <input id="dbfile" type="file" name="dbupdate" size="0" accept=".json" onChange="DbFileSelected();"
                  style="width:0; height:0;"></p>
                <div>
                  <center>
                    <p id="dstatus"></p>
                  </center>
                  <center><input id="dupload" type="button" class="btn" value="Update Database" onClick="uploadDb();"
                      style="display: none;"></center>
                  <center><input id="pupload" type="button" class="btn" value="Update From Printer"
                      onClick="uploadDbFromPrinter();" style="display: none;"></center>
                </div>
              </form>
            </td>
          </tr>
        </table>
      </div>
    </center>
  </dialog>
  <dialog id="mDialog">
    <center>
      <div class="main">
        <div id="mmListView">
          <table>
            <tr>
              <td></td>
              <td>
                <br>
                <label style="display: block;text-align: right;">
                  <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 0 24 24" width="32px"
                    onClick="dialogm.close();">
                    <path d="M0 0h24v24H0z" fill="none" />
                    <path d="M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z" />
                  </svg>
                </label>
              </td>
            </tr>
            <tr>
              <th colspan="2">
                <center>Material Database</center>
              </th>
            </tr>
          </table>
          <div class="mv-table-scroll"><table id="mmList"></table></div>
          <br>
          Template: <select id="mmTemplatePick"></select><br><br>
          <div class="mv-action-row">
            <input type="submit" class="btn" value="+ New From Template" onClick="newMaterialItem();">
            <input id="mmSync" type="submit" class="btn" value="Sync Live From Printer" onClick="syncLiveFromPrinter();">
          </div>
          <div class="mv-action-row mv-reboot-row">
            <label><input type="checkbox" id="mmReboot"> Reboot printer after push</label>
          </div>
          <div class="mv-action-row"><input type="submit" class="btn" value="Push to Printer" onClick="pushToPrinter();"></div>
          <br><br>
          <div id="mmMessage"></div>
          <br><small style="opacity:0.5">Firmware build: 2026-08-20-mergepush</small>
        </div>
        <div id="mmEditView" style="display:none;">
          <table id="mmEditForm"></table>
          <br>
          <div class="mv-action-row">
            <input type="submit" class="btn" value="Cancel" onClick="cancelMaterialEdit();">
            <input type="submit" class="btn" value="Save" onClick="saveMaterialEdit();">
          </div>
        </div>
      </div>
    </center>
  </dialog>
  <script>
    const dialog = document.getElementById("cDialog");
    const dialogp = document.getElementById("pDialog");
    const dialogu = document.getElementById("uDialog");
    const dialogd = document.getElementById("dDialog");
    const dialogm = document.getElementById("mDialog");
  </script>

  <script>
  // TFT display-label mirroring only. Reads existing in-browser jsonDb;
  // never calls printer sync/push or database-save functions.
  (function(){
    let lastSignature="",busy=false;
    function form(op,rows){
      return fetch('/display/cache',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},
        body:'op='+encodeURIComponent(op)+(rows===undefined?'':'&rows='+encodeURIComponent(rows))})
        .then(r=>{if(!r.ok)throw Error('display cache '+r.status);});
    }
    function clean(s){return String(s==null?'':s).replace(/[\t\r\n]/g,' ').trim();}
    async function refreshDisplayLabels(){
      if(busy || typeof jsonDb!=='string' || !jsonDb.includes('"kvParam":'))return;
      let items;
      try {items=JSON.parse(jsonDb).result.list;}catch(_){return;}
      if(!Array.isArray(items))return;
      const records=[];
      for(const item of items){
        if(!item || !item.base)continue;
        const id=clean(item.base.id),brand=clean(item.base.brand).slice(0,40),name=clean(item.base.name).slice(0,60);
        if(/^\d{5}$/.test(id) && name)records.push(id+'\t'+brand+'\t'+name);
        if(records.length>=500)break;
      }
      if(!records.length)return;
      // Content-based signature also notices edits without any database code hooks.
      const signature=records.join('\n');
      if(signature===lastSignature)return;
      busy=true;
      try {
        await form('begin');
        let batch='';
        for(const row of records){
          if(batch && batch.length+row.length+1>1000){await form('append',batch);batch='';}
          batch+=row+'\n';
        }
        if(batch)await form('append',batch);
        await form('commit');
        lastSignature=signature;
      }catch(e){console.warn('Display label cache refresh:',e);}
      finally{busy=false;}
    }
    window.addEventListener('load',function(){
      setTimeout(refreshDisplayLabels,2500);
      setInterval(refreshDisplayLabels,15000);
    });
  })();
  </script>
<script>
// Display-only telemetry: observe existing Material Database status text.
// Does not wrap/replace fetchRetry, syncLiveFromPrinter or pushToPrinter.
(function(){
  var lastMode='';
  function notify(mode){
    if(mode===lastMode)return;
    lastMode=mode;
    var body=new URLSearchParams();body.set('mode',mode||'done');
    fetch('/display/transfer',{method:'POST',body:body,keepalive:true}).catch(function(){});
  }
  function check(){
    var el=document.getElementById('mmMessage');if(!el)return;
    var s=(el.textContent||'').trim().toLowerCase();
    if(s.indexOf('re-checking live database before push')>=0 || s.indexOf('pushing merged database')>=0){notify('push');return;}
    if(s.indexOf('syncing live database from printer')>=0){notify('pull');return;}
    if(lastMode && (/^(synced|pushed|push failed|push aborted|sync failed|set the printer host)/.test(s))){notify('');return;}
  }
  function init(){
    var el=document.getElementById('mmMessage');if(!el)return;
    new MutationObserver(check).observe(el,{childList:true,subtree:true,characterData:true});
    check();
  }
  if(document.readyState==='loading')document.addEventListener('DOMContentLoaded',init);
  else init();
})();
</script>
</body>
</html>)==";