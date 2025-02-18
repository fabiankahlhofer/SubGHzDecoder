#include <Arduino.h>
#include <WiFiServer.h>
#include <sstream>
#include <stdio.h>
#include <cJSON.h>
#include <time.h>
#include "ntp/NTPClient.h"
#include "logger/logger.h"

#include "webserver/Webserver.h"

#define API_RECEIVED_DATA_DELIMETER ";"

WiFiServer instance(80);
String receivedData;
std::stringstream sentData;

void Webserver_init(char *ssid, char *password) {
  Logger::log(LogSeverity::DEBUG, "Setting up Access Point\n", true);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  int ssidLength = strlen(ssid) + strlen("| SSID: ") + strlen(" |");
  int passwordLength = strlen(password) + strlen("| Password: ") + strlen(" |");
  int ipLength = strlen("| IP Adress: ") + strlen(IP.toString().c_str()) + strlen(" |");
  int maxLength = ssidLength > passwordLength ? (ssidLength > ipLength ? ssidLength : ipLength) : (passwordLength > ipLength ? passwordLength : ipLength);

  Logger::log(LogSeverity::INFO, "-", true);
  for (int i = 1; i < maxLength; i++) {
    Logger::log(LogSeverity::INFO, "-");
  }
  Logger::log(LogSeverity::INFO, "\n");
  Logger::log(LogSeverity::INFO, "| SSID: ", true);
  Logger::log(LogSeverity::INFO, ssid);
  for (int i = 0; i < maxLength - ssidLength; i++) {
    Logger::log(LogSeverity::INFO, " ");
  }
  Logger::log(LogSeverity::INFO, " |\n");
  Logger::log(LogSeverity::INFO, "| Password: ", true);
  Logger::log(LogSeverity::INFO, password);
  for (int i = 0; i < maxLength - passwordLength; i++) {
    Logger::log(LogSeverity::INFO, " ");
  }
  Logger::log(LogSeverity::INFO, " |\n");
  Logger::log(LogSeverity::INFO, "| IP Address: ", true);
  Logger::log(LogSeverity::INFO, IP.toString().c_str());
  for (int i = 0; i < maxLength - ipLength; i++) {
    Logger::log(LogSeverity::INFO, " ");
  }
  Logger::log(LogSeverity::INFO, "|\n");
  Logger::log(LogSeverity::INFO, "-", true);
  for (int i = 1; i < maxLength; i++) {
    Logger::log(LogSeverity::INFO, "-");
  }
  Logger::log(LogSeverity::INFO, "\n");

  instance.begin();

  Logger::log(LogSeverity::DEBUG, "Webserver started. Ready for connection.\n", true);
}

unsigned int receivedCounter = 0;
void Webserver_addReceivedValue(String json) {
  receivedData += receivedCounter == 0 ? "" : API_RECEIVED_DATA_DELIMETER;
  receivedData += json;
  receivedCounter++;
}

String Webserver_getReceivedValues() {
  return (char *)(receivedData.c_str());
}

String Webserver_shortenJSON(String json) {
  json.replace("\t", "");
  json.replace("\n", "");
  return json;
}

String toLower(String str) {
  String result = "";

  for (int i = 0; i < str.length(); i++) {
    result += tolower(str[i]);
  }

  return result;
}

String formatTime() {
  char *timeStr = NTPClient_getTimeStr(NTPClient_getTime());
  String result(timeStr);
  free(timeStr);
  return result;
}

void Webserver_appendToListWithTime(std::list<String> *list, std::list<String> *dateList, String value) {
  list->push_back(value);
  dateList->push_back(formatTime());
}

void Webserver_handleClient(Queue<String> *sendDataQueue_443MHz, 
                            Queue<String> *sendDataQueue_IR, 
                            std::list<String> *receivedDataList_443MHz, 
                            std::list <String> *receivedDataList_IR,
                            std::list <String> *receivedDataDateList_443MHz,
                            std::list <String> *receivedDataDateList_IR) {
  String header;

  WiFiClient client = instance.available(); 

  if (client) { 
    Logger::log(LogSeverity::DEBUG, "New Client.\n", true);
    String currentLine = ""; 
    while (client.connected()) { 
      if (client.available()) { 
        char c = client.read(); 
        header += c;
        if (c == '\n') { 

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Access-Control-Allow-Origin: *");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("/home") >= 0) {
              //send them the longest string ever known to mankind
              client.println("<!DOCTYPE html><html lang=\"en\" id=\"base\"><head><meta charset=\"UTF-8\"><title>SubGHzDecoder</title><style>ul,ul li{font-family:\'Roboto Mono\',monospace}.hover-pointer,.linkButton{cursor:pointer}:not(.logItem){color:#23272a;font-family:Arial}.logItem{margin-top:0;margin-bottom:0}.linkButton{padding:.75rem;background-color:#23272a;color:#eee;border-radius:.5rem}ul{list-style-type:none;font-size:14px;line-height:25px;padding-left:5px}ul li{border-bottom:1px solid #80808038;cursor:default}li{display:block;white-space:normal;word-wrap:break-word}.disabled{background-color:#aaa;cursor:not-allowed}#consoleContainer{width:100%;display:flex;gap:2.5rem}#consoleWindow{height:50vh;width:50vw}#lowerContainer>div{flex-grow:1;padding:1rem}@media (orientation:portrait){#consoleWindow{width:95vw;margin-left:2.5vw;margin-right:2.5vw}#consoleContainer{display:block}}</style><style>*{margin:0;padding:0}.svg-box{display:inline-block;position:relative;width:150px}.green-stroke{stroke:#7cb342}.red-stroke{stroke:#ff6245}.yellow-stroke{stroke:#ffc107}.alert-sign,.checkmark,.cross{stroke-width:6.25;position:absolute;stroke-linecap:round}.circular circle.path{stroke-dasharray:330;stroke-dashoffset:0;stroke-linecap:round;opacity:.4;animation:.7s ease-out draw-circle}.checkmark{top:56px;left:49px;width:52px;height:40px}.checkmark path{animation:1s ease-out draw-check}@keyframes draw-circle{0%{stroke-dasharray:0,330;stroke-dashoffset:0;opacity:1}80%{stroke-dasharray:330,330;stroke-dashoffset:0;opacity:1}100%{opacity:.4}}@keyframes draw-check{0%{stroke-dasharray:49,80;stroke-dashoffset:48;opacity:0}50%{stroke-dasharray:49,80;stroke-dashoffset:48;opacity:1}100%{stroke-dasharray:130,80;stroke-dashoffset:48}}.cross{top:54px;left:54px;width:40px;height:40px}.cross .first-line{animation:.7s ease-out draw-first-line}.cross .second-line{animation:.7s ease-out draw-second-line}@keyframes draw-first-line{0%,50%{stroke-dasharray:0,56;stroke-dashoffset:0}100%{stroke-dasharray:56,330;stroke-dashoffset:0}}@keyframes draw-second-line{0%,50%{stroke-dasharray:0,55;stroke-dashoffset:1}100%{stroke-dasharray:55,0;stroke-dashoffset:70}}.alert-sign{top:40px;left:68px;width:15px;height:70px;animation:.5s cubic-bezier(.175,.885,.32,1.275) alert-sign-bounce}.alert-sign .dot{stroke:none;fill:#ffc107}@keyframes alert-sign-bounce{0%{transform:scale(0);opacity:0}50%{transform:scale(0);opacity:1}100%{transform:scale(1)}}</style><style>#toast{position:fixed;top:0;left:50%;transform:translate(-50%);background-color:#eee;color:#000;padding:16px;border-radius:4px;text-align:center;box-shadow:0 0 20px rgba(0,0,0,.3);visibility:hidden;opacity:0;z-index:20202020202020}#toast.show{visibility:visible;animation:3s fadeInOut}@keyframes fadeInOut{5%,95%{opacity:1;top:50px}15%,85%{opacity:1;top:30px}}</style><style>#createCustomCommandCloseBtn:hover,.box{cursor:pointer}.box{background:#23272a;color:#eee;position:relative;padding:10px;box-shadow:0 0 8px rgba(0,0,0,.2);box-sizing:border-box}.box:hover{background:rgba(35,39,42,.9);box-shadow:0 8px 15px rgba(0,0,0,.4)}</style></head><body style=\"background:#eee;overflow-x:hidden\"><div id=\"toast\"></div><h1 style=\"margin:1rem\">SubGHzDecoder</h1><div id=\"customSendButtonsContainer\" style=\"position:absolute;top:1rem;right:1rem;display:flex;gap:1rem\"><div style=\"float:left\" class=\"box\" id=\"send-custom-443-btn\"><div class=\"box__label\" style=\"color:#eee\">443MHz</div></div><div style=\"float:right\" class=\"box\" id=\"send-custom-ir-btn\"><div class=\"box__label\" style=\"color:#eee\">IR</div></div></div><div id=\"consoleContainer\"><div style=\"width:50vw;min-width:50vw;max-width:50vw;display:flex;justify-content:space-between;height:50vh\"><iframe style=\"width:49%;margin-right:1%;margin-left:1%\" frameborder=\"0\" srcdoc=\"&lt;!DOCTYPE html&gt;\n      &lt;html lang=&quot;en&quot;&gt;\n      &lt;head&gt;\n        &lt;meta charset=&quot;UTF-8&quot;&gt;\n        &lt;meta name=&quot;viewport&quot; content=&quot;width=device-width, initial-scale=1.0&quot;&gt;\n        &lt;title&gt;Console&lt;/title&gt;\n      \n        &lt;!-- style.css --&gt;\n        &lt;style&gt;ul,ul li{font-family:\'Roboto Mono\',monospace}.hover-pointer,.linkButton{cursor:pointer}:not(.logItem){color:#23272a;font-family:Arial}.logItem{margin-top:0;margin-bottom:0}.linkButton{padding:.75rem;background-color:#23272a;color:#eee;border-radius:.5rem}ul{list-style-type:none;font-size:14px;line-height:25px;padding-left:5px}ul li{border-bottom:1px solid #80808038;cursor:default}li{display:block;white-space:normal;word-wrap:break-word}.disabled{background-color:#aaa;cursor:not-allowed}#consoleContainer{width:100%;display:flex;gap:2.5rem}#consoleWindow{height:50vh;width:50vw}#lowerContainer&gt;div{flex-grow:1;padding:1rem}@media (orientation:portrait){#consoleWindow{width:95vw;margin-left:2.5vw;margin-right:2.5vw}#consoleContainer{display:block}}&lt;/style&gt;\n        \n        &lt;style&gt;li.logItem{cursor:pointer;}&lt;/style&gt;\n      &lt;/head&gt;\n      \n      &lt;body style=&quot;background:#23272A&quot;&gt;\n        &lt;ul id=&quot;console&quot; style=&quot;color:#eee;font-family:\'Roboto Mono\', monospace;font-style:bold;&quot;&gt;\n          &lt;li class=&quot;logItem&quot;&gt;Start&lt;/li&gt;\n        &lt;ul&gt;\n        &lt;script&gt;\n      function mapNumberToHexCustom(t){const e=\"23272A\",o=\"FFFFFF\";t=Math.max(0,Math.min(256,t));let r=Math.round((parseInt(e.substring(0,2),16)*(256-t)+parseInt(o.substring(0,2),16)*t)/256),n=Math.round((parseInt(e.substring(2,4),16)*(256-t)+parseInt(o.substring(2,4),16)*t)/256),l=Math.round((parseInt(e.substring(4,6),16)*(256-t)+parseInt(o.substring(4,6),16)*t)/256);return r=r.toString(16).toUpperCase(),n=n.toString(16).toUpperCase(),l=l.toString(16).toUpperCase(),\"#\"+r.padStart(2,\"0\")+n.padStart(2,\"0\")+l.padStart(2,\"0\")}function log(t){const e=localStorage.getItem(\"protocols\");if(e){let o=e.split(\";;;\");o=o.filter((t=>\"\"!==t));let r=[];o.forEach((t=>{let e=localStorage.getItem(\"!-\"+t);e&&r.push(e)})),r.includes(t)&&(t=o[r.indexOf(t)])}var o=document.createElement(\"li\");o.classList.add(\"logItem\"),o.addEventListener(\"click\",(async function(t){let e=t.target.textContent;localStorage.getItem(\"!-\"+e)&&(e=localStorage.getItem(\"!-\"+e)),localStorage.setItem(\"selectedLog\",e),localStorage.setItem(\"scheduleToInspectElement\",\":;:\"+e),\"true\"===localStorage.getItem(\"isCreatingProtocoll\")&&localStorage.setItem(\"addingProtocolValue\",t.target.textContent);for(let e=256;e>=0;e--)t.target.style.backgroundColor=mapNumberToHexCustom(e),t.target.style.color=mapNumberToHexCustom(256-e),await new Promise((t=>setTimeout(t,1)))})),o.innerHTML=\"string\"==typeof t?t:JSON.stringify(t,null,4),document.querySelector(\"#console\").appendChild(o)}function overwriteLog(t){document.querySelectorAll(\"li.logItem\").forEach((t=>{t.remove()})),t.forEach((t=>{log(t)}))}function clearLog(){document.querySelectorAll(\"li.logItem\").forEach((t=>{t.remove()}))}function scrollToBottom(){window.scrollTo({top:document.body.scrollHeight,behavior:\"smooth\"})}let lastJsonResponse=\"\";async function update(){const t=await getJSONfromAPI(),e=JSON.parse(t);if(t!==lastJsonResponse){clearLog();for(const t in e)log(e[t]);\"true\"===localStorage.getItem(\"shouldScroll\")&&scrollToBottom(),lastJsonResponse=t,console.log(\"debug\")}}async function getJSONfromAPI(){let t=\"\",e=\"\";try{const o=await fetch(\"http://192.168.4.1/api/get/443mhz\"),r=await o.json();t=JSON.stringify(r),e=r;const n={...e};return console.log(JSON.stringify(n)),JSON.stringify(n)}catch(t){return console.error(\"Error fetching data:\",t),null}}setInterval(update,1e3);\n        &lt;/script&gt;\n      &lt;/body&gt;\n      &lt;/html&gt;\"></iframe><iframe style=\"width:49%\" srcdoc=\"&lt;!DOCTYPE html&gt;\n      &lt;html lang=&quot;en&quot;&gt;\n      &lt;head&gt;\n        &lt;meta charset=&quot;UTF-8&quot;&gt;\n        &lt;meta name=&quot;viewport&quot; content=&quot;width=device-width, initial-scale=1.0&quot;&gt;\n        &lt;title&gt;Console&lt;/title&gt;\n        &lt;!-- style.css --&gt;\n        &lt;style&gt;ul,ul li{font-family:\'Roboto Mono\',monospace}.hover-pointer,.linkButton{cursor:pointer}:not(.logItem){color:#23272a;font-family:Arial}.logItem{margin-top:0;margin-bottom:0}.linkButton{padding:.75rem;background-color:#23272a;color:#eee;border-radius:.5rem}ul{list-style-type:none;font-size:14px;line-height:25px;padding-left:5px}ul li{border-bottom:1px solid #80808038;cursor:default}li{display:block;white-space:normal;word-wrap:break-word}.disabled{background-color:#aaa;cursor:not-allowed}#consoleContainer{width:100%;display:flex;gap:2.5rem}#consoleWindow{height:50vh;width:50vw}#lowerContainer&gt;div{flex-grow:1;padding:1rem}@media (orientation:portrait){#consoleWindow{width:95vw;margin-left:2.5vw;margin-right:2.5vw}#consoleContainer{display:block}}&lt;/style&gt;\n        &lt;style&gt;li.logItem{cursor:pointer;}&lt;/style&gt;\n      &lt;/head&gt;\n      \n      &lt;body style=&quot;background:#23272A&quot;&gt;\n        &lt;ul id=&quot;console&quot; style=&quot;color:#eee;font-family:\'Roboto Mono\', monospace;font-style:bold;&quot;&gt;\n          &lt;li class=&quot;logItem&quot;&gt;Start&lt;/li&gt;\n        &lt;ul&gt;\n        &lt;script&gt;\n      function mapNumberToHexCustom(t){const e=\"23272A\",o=\"FFFFFF\";t=Math.max(0,Math.min(256,t));let r=Math.round((parseInt(e.substring(0,2),16)*(256-t)+parseInt(o.substring(0,2),16)*t)/256),n=Math.round((parseInt(e.substring(2,4),16)*(256-t)+parseInt(o.substring(2,4),16)*t)/256),a=Math.round((parseInt(e.substring(4,6),16)*(256-t)+parseInt(o.substring(4,6),16)*t)/256);return r=r.toString(16).toUpperCase(),n=n.toString(16).toUpperCase(),a=a.toString(16).toUpperCase(),\"#\"+r.padStart(2,\"0\")+n.padStart(2,\"0\")+a.padStart(2,\"0\")}function log(t){const e=localStorage.getItem(\"protocols\");if(e){let o=e.split(\";;;\");o=o.filter((t=>\"\"!==t));let r=[];o.forEach((t=>{let e=localStorage.getItem(\"!-\"+t);e&&r.push(e)})),r.includes(t)&&(t=o[r.indexOf(t)])}var o=document.createElement(\"li\");o.classList.add(\"logItem\"),o.addEventListener(\"click\",(async function(t){let e=t.target.textContent;localStorage.getItem(\"!-\"+e)&&(e=localStorage.getItem(\"!-\"+e)),localStorage.setItem(\"selectedLog\",e),localStorage.setItem(\"scheduleToInspectElement\",\":;:\"+e),\"true\"===localStorage.getItem(\"isCreatingProtocoll\")&&localStorage.setItem(\"addingProtocolValue\",t.target.textContent);for(let e=256;e>=0;e--)t.target.style.backgroundColor=mapNumberToHexCustom(e),t.target.style.color=mapNumberToHexCustom(256-e),await new Promise((t=>setTimeout(t,1)))})),o.innerHTML=\"string\"==typeof t?t:JSON.stringify(t,null,4),document.querySelector(\"#console\").appendChild(o)}function overwriteLog(t){document.querySelectorAll(\"li.logItem\").forEach((t=>{t.remove()})),t.forEach((t=>{log(t)}))}function clearLog(){document.querySelectorAll(\"li.logItem\").forEach((t=>{t.remove()}))}function scrollToBottom(){window.scrollTo({top:document.body.scrollHeight,behavior:\"smooth\"})}let lastJsonResponse=\"\";async function update(){const t=await getJSONfromAPI(),e=JSON.parse(t);if(t!==lastJsonResponse){clearLog();for(const t in e)log(e[t]);\"true\"===localStorage.getItem(\"shouldScroll\")&&scrollToBottom(),lastJsonResponse=t,console.log(\"debug\")}}async function getJSONfromAPI(){let t=\"\",e=\"\",o=\"\",r=\"\";try{const n=await fetch(\"http://192.168.4.1/api/get/443mhz\"),a=await n.json();t=JSON.stringify(a),e=a;const l=await fetch(\"http://192.168.4.1/api/get/ir\"),s=await l.json();o=JSON.stringify(s),r=s;const c={...r};return console.log(JSON.stringify(c)),JSON.stringify(c)}catch(t){return console.error(\"Error fetching data:\",t),null}}setInterval(update,1e3);\n        &lt;/script&gt;\n      &lt;/body&gt;\n      &lt;/html&gt;\" frameborder=\"0\"></iframe></div><div id=\"consoleConfigContainer\"><input type=\"checkbox\" id=\"automaticScrolling\" name=\"automaticScrolling\" value=\"automaticScrolling\"><label for=\"automaticScrolling\">Enable automatic scrolling</label><div id=\"menuButtonsContainer\" style=\"display:flex;gap:1rem;align-items:center;margin-top:2.5rem;margin-bottom:2.5rem\"><a id=\"protocolManagerButton\" class=\"linkButton\" style=\"text-align:center;width:10rem\">Create New Protocol</a><div id=\"protocol-name-container\"></div></div><span id=\"isCreatingProtocolInformator\"></span><div id=\"signal-info\" style=\"margin-top:2.5rem\"></div><ul id=\"createProtocolCommandList\"><li id=\"createProtocolCommandListLi\"></li></ul></div></div><div id=\"lowerContainer\" style=\"display:flex;width:100%\"><div id=\"protocols\" style=\"width:50%\"><h2 style=\"margin-top:0;float:left\">Protocols</h2><div style=\"float:right\" class=\"box\" id=\"import-protocol-btn\"><div class=\"box__label\" style=\"color:#eee\">Import Protocol</div></div><input type=\"file\" id=\"fileInput\" accept=\".json\" style=\"display:none\" onchange=\"handleFileSelection(event)\"> <input type=\"file\" id=\"fileInput\" style=\"display:none\" onchange=\"handleFileSelection(event)\"><iframe srcdoc=\"&lt;!DOCTYPE html&gt;\n      &lt;html lang=&quot;en&quot;&gt;\n      \n      &lt;head&gt;\n        &lt;meta charset=&quot;UTF-8&quot;&gt;\n        &lt;style&gt;\n        *,.boxes{margin:0}.box,.boxes{padding:20px;box-sizing:border-box}.boxes,ul li{list-style:none}*,.context__menu{font-family:Arial}.box,.item,button{cursor:pointer}body{overflow-x:hidden}*{padding:0;border:0}.boxes{display:grid;grid-template-rows:repeat(4,1fr);grid-template-columns:repeat(4,1fr);grid-gap:20px;height:90vh}.box{background:#23272a;color:#eee;position:relative;box-shadow:0 0 8px rgba(0,0,0,.2);height:100%}.box:hover{background:rgba(35,39,42,.9);box-shadow:0 8px 15px rgba(0,0,0,.4)}.box__label{position:absolute;right:50%;top:50%;transform:translate(50%,-50%)}*,:after,:before{-moz-box-sizing:border-box;-webkit-box-sizing:border-box;box-sizing:border-box}:active,:focus,a:active,a:focus{outline:0}aside,footer,header,nav{display:block}body,html{height:100%;width:100%;line-height:1;font-size:14px;-ms-text-size-adjust:100%;-moz-text-size-adjust:100%;-webkit-text-size-adjust:100%}button,input,textarea{font-family:inherit}input::-ms-clear{display:none}input::-ms-reveal{display:none}button::-moz-focus-inner{padding:0;border:0}a,a:hover,a:visited{text-decoration:none}.context__menu{position:absolute;z-index:10000;text-align:left;display:none;opacity:0;width:150px;background-color:#202020;color:#fff;padding:10px;border-radius:10px;box-shadow:0 10px 20px rgba(0,0,0,.3);transition:.2s}.menu__list{display:flex;justify-content:space-between;flex-direction:column;letter-spacing:1px;height:100%;width:100%}.item{padding:10px 5px;font-weight:300;border-radius:5px;transition:.1s}.item:hover{background-color:#303030;letter-spacing:2px}.item:active{color:rgb(66,133,244,.3)}.active{display:block;opacity:1}\n        &lt;/style&gt;\n      \n      &lt;/head&gt;\n      \n      &lt;body&gt;\n        &lt;ul id=&quot;protocolList&quot; class=&quot;boxes&quot;&gt; &lt;/ul&gt;\n      \n        &lt;nav class=&quot;context__menu&quot;&gt;\n          &lt;ul class=&quot;menu__list&quot;&gt;\n            &lt;li class=&quot;item send&quot;&gt;\n              &lt;span class=&quot;list__item&quot;&gt;Send&lt;/span&gt;\n            &lt;/li&gt;\n            &lt;li class=&quot;item view&quot;&gt;\n              &lt;span class=&quot;list__item&quot;&gt;View info&lt;/span&gt;\n            &lt;/li&gt;\n            &lt;li class=&quot;item export&quot;&gt;\n              &lt;span class=&quot;list__item&quot;&gt;Export&lt;/span&gt;\n            &lt;/li&gt;\n            &lt;li class=&quot;item del&quot;&gt;\n              &lt;span class=&quot;list__item&quot;&gt;Delete&lt;/span&gt;\n            &lt;/li&gt;\n          &lt;/ul&gt;\n        &lt;/nav&gt;\n      \n        &lt;script&gt;\n        function sync(){const e=localStorage.getItem(&quot;protocols&quot;);document.getElementById(&quot;protocolList&quot;).innerHTML=&quot;&quot;,e&amp;&amp;e.split(&quot;;;;&quot;).forEach((e=&gt;{if(0!=e.length){const o=document.querySelector(&quot;#protocolList&quot;),t=document.createElement(&quot;li&quot;);t.classList.add(&quot;boxes__item&quot;),t.innerHTML=`&lt;div class=&quot;box&quot;&gt;&lt;div class=&quot;box__label&quot;&gt;${e}&lt;/div&gt;&lt;/div&gt;`,t.addEventListener(&quot;click&quot;,(()=&gt;{})),o.appendChild(t)}}))}sync(),setInterval((()=&gt;{const e=localStorage.getItem(&quot;scheduleToAddElement&quot;),o=localStorage.getItem(&quot;scheduleToRemoveElement&quot;);if(e){const o=document.querySelector(&quot;#protocolList&quot;),t=document.createElement(&quot;li&quot;);t.classList.add(&quot;boxes__item&quot;);const l=e.split(&quot;;;;&quot;)[0],c=e.split(&quot;;;;&quot;)[1];t.innerHTML=`&lt;div class=&quot;box&quot;&gt;&lt;div class=&quot;box__label&quot;&gt;${l}&lt;/div&gt;&lt;/div&gt;`,o.appendChild(t),localStorage.setItem(l,c),localStorage.setItem(&quot;protocols&quot;,null==localStorage.getItem(&quot;protocols&quot;)?l:localStorage.getItem(&quot;protocols&quot;)+&quot;;;;&quot;+l),localStorage.removeItem(&quot;scheduleToAddElement&quot;),sync()}if(o){const e=document.querySelector(&quot;#protocolList&quot;),t=Array.from(e.children).find((e=&gt;e.querySelector(&quot;.box__label&quot;).textContent===o));if(!e||!t)return;e.removeChild(t),localStorage.removeItem(&quot;!-&quot;+o),localStorage.removeItem(&quot;scheduleToRemoveElement&quot;),sync()}&quot;true&quot;==localStorage.getItem(&quot;scheduleSync&quot;)&amp;&amp;(localStorage.removeItem(&quot;scheduleSync&quot;),sync()),localStorage.getItem(&quot;scheduleForceSync&quot;)&amp;&amp;(window.location.reload(),localStorage.removeItem(&quot;scheduleForceSync&quot;))}));\n        &lt;/script&gt;\n      \n        &lt;script&gt;\n          const menu=document.querySelector(&quot;.context__menu&quot;),[add,view,edit,del]=document.querySelectorAll(&quot;.send, .view, .export, .del&quot;);let selectedElement;function encodeToBase64(e){return btoa(unescape(encodeURIComponent(e)))}function estimateType(e){let t=!1,o=!1;return e.includes(&quot;decimal&quot;)&amp;&amp;e.includes(&quot;hex&quot;)&amp;&amp;e.includes(&quot;length&quot;)&amp;&amp;e.includes(&quot;protocol&quot;)&amp;&amp;e.includes(&quot;bin&quot;)&amp;&amp;e.includes(&quot;raw&quot;)?(t=!0,&quot;443-raw&quot;):e.includes(&quot;address&quot;)&amp;&amp;e.includes(&quot;command&quot;)&amp;&amp;e.includes(&quot;protocol&quot;)&amp;&amp;e.includes(&quot;bin&quot;)&amp;&amp;e.includes(&quot;dec&quot;)?(o=!0,&quot;IR-raw&quot;):e.startsWith(&quot;443#&quot;)?&quot;443&quot;:e.startsWith(&quot;IR#&quot;)?&quot;IR&quot;:t&amp;&amp;o?&quot;undefined&quot;:t?&quot;443&quot;:o?&quot;IR&quot;:void 0}function getPosition(e){let t=0,o=0;return e.pageX||e.pageY?(t=e.pageX,o=e.pageY):(e.clientX||e.clientY)&amp;&amp;(t=e.clientX+document.body.scrollLeft+document.documentElement.scrollLeft,o=e.clientY+document.body.scrollTop+document.documentElement.scrollTop),{x:t,y:o}}function clickInsideElement(e,t){let o=e.srcElement||e.target;for(;o;){if(o.classList&amp;&amp;o.classList.contains(t))return o;o=o.parentNode}return!1}document.addEventListener(&quot;contextmenu&quot;,(e=&gt;{e.preventDefault();const{x:t,y:o}=getPosition(e);menu.classList.add(&quot;active&quot;),menu.style.left=t+&quot;px&quot;,menu.style.top=o+&quot;px&quot;,selectedElement=e.target})),document.addEventListener(&quot;click&quot;,(e=&gt;{clickInsideElement(e,&quot;item&quot;)||menu.classList.remove(&quot;active&quot;)})),setInterval((()=&gt;{&quot;true&quot;==localStorage.getItem(&quot;scheduleSync&quot;)&amp;&amp;(localStorage.removeItem(&quot;scheduleSync&quot;),sync())}),100),[add,view,edit,del].forEach((e=&gt;{e.addEventListener(&quot;click&quot;,(t=&gt;{const o=e.classList[1].toUpperCase();console.log(`${o} click ${selectedElement.textContent}`);const s=localStorage.getItem(&quot;!-&quot;+selectedElement.textContent.trim());if(&quot;VIEW&quot;===o)console.log(s),localStorage.setItem(&quot;scheduleToInspectElement&quot;,selectedElement.textContent+&quot;:;:&quot;+s);else{if(&quot;EDIT&quot;===o)return;if(&quot;DEL&quot;===o){localStorage.removeItem(&quot;!-&quot;+selectedElement.textContent.trim());let e=localStorage.getItem(&quot;protocols&quot;).split(&quot;;;;&quot;);e=e.filter((e=&gt;e!==selectedElement.textContent.trim())),localStorage.setItem(&quot;protocols&quot;,e.join(&quot;;;;&quot;)),setTimeout((()=&gt;{sync()}),150)}else if(&quot;SEND&quot;===o){s.split(&quot;;#;&quot;);let e=s.startsWith(&quot;443#&quot;)?&quot;443&quot;:s.startsWith(&quot;IR#&quot;)?&quot;IR&quot;:&quot;undefined&quot;;if(&quot;undefined&quot;===e&amp;&amp;(e=estimateType(s)),&quot;undefined&quot;===e)return void localStorage.setItem(&quot;scheduleToast&quot;,&quot;Error: Couldn\'t obtain Protocol type&quot;);if(e.includes(&quot;443&quot;)){const e=s.startsWith(&quot;443#&quot;)?s.split(&quot;#&quot;)[1]:JSON.parse(s).bin;localStorage.setItem(&quot;scheduleToSend443Binary&quot;,e)}else if(e.includes(&quot;IR&quot;)&amp;&amp;!e.includes(&quot;raw&quot;)){const e=s.split(&quot;#&quot;),t=e[1],o=e[2],l=e[3];if(!t||!o||!l)return void localStorage.setItem(&quot;scheduleToast&quot;,&quot;Data sent successfully.&quot;);localStorage.setItem(&quot;scheduleToSendIR&quot;,`${t}#${o}#${l}`),console.log(&quot;hello&quot;)}else if(e.includes(&quot;IR&quot;)&amp;&amp;e.includes(&quot;raw&quot;)){const e=JSON.parse(s).address,t=JSON.parse(s).command,o=JSON.parse(s).protocol;if(!e||!t||!o)return void localStorage.setItem(&quot;scheduleToast&quot;,&quot;Data sent successfully.&quot;);localStorage.setItem(&quot;scheduleToSendIR&quot;,`${e}#${t}#${o}`)}}else if(&quot;EXPORT&quot;===o){const e=encodeToBase64(s),t={[encodeToBase64(&quot;!-&quot;+selectedElement.textContent)]:e},o=new Blob([JSON.stringify(t)],{type:&quot;text/plain;charset=utf-8&quot;}),l=URL.createObjectURL(o),n=document.createElement(&quot;a&quot;);n.href=l,n.download=`${selectedElement.textContent.trim()}.json`,n.click(),URL.revokeObjectURL(l)}}menu.classList.remove(&quot;active&quot;)}))})),setInterval((()=&gt;{const e=&quot;http://192.168.4.1/&quot;,t=localStorage.getItem(&quot;scheduleToSend443Binary&quot;),o=localStorage.getItem(&quot;scheduleToSendIR&quot;);if(t){localStorage.removeItem(&quot;scheduleToSend443Binary&quot;);let o=!0;const s=e+&quot;api/send/443MHz&quot;+`?data=${t}`;console.log(&quot;I\'m sending 443 to &quot;+s),fetch(s).then((e=&gt;{if(o=!0,!e.ok||!e.text().includes(t))throw localStorage.setItem(&quot;scheduleToast&quot;,&quot;An Error occurred whilst trying to send this Data.\\n&quot;+e.statusText),new Error(&quot;Network response was not ok.&quot;);if(!e.text().includes(t))throw localStorage.setItem(&quot;scheduleToast&quot;,&quot;A malformed request was sent.\\n&quot;+e.statusText),new Error(&quot;Network response was not ok.&quot;);localStorage.setItem(&quot;scheduleToast&quot;,&quot;Command &quot;+t+&quot; sent successfully&quot;)})).then((e=&gt;{o=!0})).catch((e=&gt;{o=!0})),!1===o?localStorage.setItem(&quot;scheduleToast&quot;,&quot;Could not send the Data.&quot;):localStorage.setItem(&quot;scheduleToast&quot;,&quot;Successfully sent the Data.&quot;)}if(o){localStorage.removeItem(&quot;scheduleToSendIR&quot;);const t=o.split(&quot;#&quot;),s=t[0],l=t[1],n=t[2];let c=!1;const a=e+&quot;api/send/ir&quot;+`?address=${s}&amp;command=${l}&amp;protocol=${n}`;console.log(&quot;I\'m sending IR to &quot;+a),fetch(a).then((e=&gt;{if(c=!0,!e.ok)throw localStorage.setItem(&quot;scheduleToast&quot;,&quot;An error occurred whilst trying to send this data.\\n&quot;+e.statusText),new Error(&quot;Network response was not ok.&quot;);return e.text()})).then((e=&gt;{if(!(e.includes(s)&amp;&amp;e.includes(l)&amp;&amp;e.includes(n)))throw localStorage.setItem(&quot;scheduleToast&quot;,&quot;A malformed request was sent.\\n&quot;+e),new Error(&quot;Malformed request response.&quot;);localStorage.setItem(&quot;scheduleToast&quot;,&quot;Command &quot;+o+&quot; sent successfully&quot;)})).catch((e=&gt;{c=!0})),!1===c?localStorage.setItem(&quot;scheduleToast&quot;,&quot;Could not send the Data.&quot;):localStorage.setItem(&quot;scheduleToast&quot;,&quot;Successfully sent the Data.&quot;)}}),100);\n        &lt;/script&gt;\n      &lt;/body&gt;\n      \n      &lt;/html&gt;\" frameborder=\"0\" width=\"100%\" height=\"100%\"></iframe></div><iframe frameborder=\"0\" style=\"width:50%\" srcdoc=\"&lt;!-- style.css --&gt;\n    &lt;style&gt;ul,ul li{font-family:\'Roboto Mono\',monospace}.hover-pointer,.linkButton{cursor:pointer}:not(.logItem){color:#23272a;font-family:Arial}.logItem{margin-top:0;margin-bottom:0}.linkButton{padding:.75rem;background-color:#23272a;color:#eee;border-radius:.5rem}ul{list-style-type:none;font-size:14px;line-height:25px;padding-left:5px}ul li{border-bottom:1px solid #80808038;cursor:default}li{display:block;white-space:normal;word-wrap:break-word}.disabled{background-color:#aaa;cursor:not-allowed}#consoleContainer{width:100%;display:flex;gap:2.5rem}#consoleWindow{height:50vh;width:50vw}#lowerContainer&gt;div{flex-grow:1;padding:1rem}@media (orientation:portrait){#consoleWindow{width:95vw;margin-left:2.5vw;margin-right:2.5vw}#consoleContainer{display:block}}&lt;/style&gt;\n    &lt;div id=&quot;inspect&quot;&gt;\n      &lt;h2 id=&quot;inspectHeader&quot;&gt;Inspect&lt;/h2&gt;\n      &lt;div id=&quot;inspectContent&quot;&gt;&lt;/div&gt;\n    &lt;/div&gt;\n    &lt;script&gt;\n      function isValidJSON(e){try{return JSON.parse(e),!0}catch(e){return!1}}function estimateMethod(e){if(e.startsWith(\"443#\"))return\"443-raw\";if(e.startsWith(\"ir#\")||e.startsWith(\"IR#\"))return\"ir-raw\";if(isValidJSON(e)){if(e.includes(\"address\")&&e.includes(\"command\")&&e.includes(\"protocol\")&&e.includes(\"bin\")&&e.includes(\"dec\"))return\"ir-json\";if(e.includes(\"decimal\")&&e.includes(\"hex\")&&e.includes(\"length\")&&e.includes(\"protocol\")&&e.includes(\"tri\"))return\"443-json\"}return onlyConsistsOfBinaryCharacters(e)?\"443-raw\":void 0}function onlyConsistsOfBinaryCharacters(e){let t=0,n=0;for(let l=0;l<e.length;l++)if(\"0\"===e[l]?n++:\"1\"===e[l]&&t++,n+t!=l+1)return!1;return!0}setInterval((()=>{if(localStorage.getItem(\"scheduleToInspectElement\")){const e=document.getElementById(\"inspectContent\"),t=localStorage.getItem(\"scheduleToInspectElement\");localStorage.removeItem(\"scheduleToInspectElement\"),localStorage.removeItem(\"scheduleToInspectElement-shouldOverwrite\");const n=t.split(\":;:\");if(2!=n.length)return;n[0]?document.getElementById(\"inspectHeader\").innerHTML=\"Inspect - \"+n[0]:document.getElementById(\"inspectHeader\").innerHTML=\"Inspect\";const l=n[1].split(\";#;\");if(!l)return;let r=estimateMethod(l[0]);if(void 0===r)return void localStorage.setItem(\"scheduleToast\",\"Error: Couldn\'t obtain Protocol type from \"+l[0]);if(r.endsWith(\"-json\"))for(let t=0;t<l.length;t++){let r=`<ul>${Object.entries(JSON.parse(l[t])).map((([e,t])=>`<li>${e}: ${t}</li>`)).join(\"\")}</ul>`;\"<ul></ul>\"===r&&(r=\"<ul>\"+n[1]+\"</ul>\"),0==t?e.innerHTML=r:e.innerHTML+=r}else if(r.endsWith(\"-raw\")){let t=\"\",n=!0;if(r.startsWith(\"443\"))t=\"443MHz - \",l[0].includes(\"#\")?t+=l[0].split(\"#\")[1]:t+=l[0];else if(r.startsWith(\"ir\"))if(t=\"IR - \",l[0].includes(\"#\")){const r=l[0].split(\"#\");t=\"\",n=!1,e.innerHTML=\"<ul><li>Address: \"+r[1]+\"</li><li>Command: \"+r[2]+\"</li><li>Protocol: \"+r[3]+\"</li></ul>\"}else t+=l[0];n&&(e.innerHTML=t)}}localStorage.getItem(\"scheduleToInspectHeader\")&&(document.getElementById(\"inspectHeader\").innerHTML=localStorage.getItem(\"scheduleToInspectHeader\"),localStorage.removeItem(\"scheduleToInspectHeader\"))}),25);\n    &lt;/script&gt;\"></iframe></div><div id=\"createCustomCommandGUI\" style=\"top:50%;position:absolute;transform:translate(-50%,-50%);left:50%;z-index:9999999\"></div><script>localStorage.removeItem(\"selectedLog\");{let e=localStorage.getItem(\"protocols\");e||(localStorage.setItem(\"protocols\",\"\"),e=\"\"),e.startsWith(\";;;\")&&(e=e.substring(3)),e.endsWith(\";;;\")&&(e=e.substring(0,e.length-3)),localStorage.setItem(\"protocols\",e)}function disableButton(e){e.style.backgroundColor=\"gray\",e.style.cursor=\"not-allowed\",e.style.pointerEvents=\"none\",e.classList.add(\"disabledButton\")}function enableButton(e){e.style.backgroundColor=\"\",e.style.cursor=\"pointer\",e.style.pointerEvents=\"auto\",e.classList.remove(\"disabledButton\")}function hide(e){var t=document.createElement(\"div\");t.style.zIndex=\"1000\",t.style.backgroundColor=\"rgba(0, 0, 0, \"+e+\")\",t.style.width=\"100vw\",t.style.height=\"100vh\",t.style.position=\"absolute\",t.style.top=\"0px\",t.style.left=\"0px\",t.id=\"hide-div\",document.getElementById(\"base\").appendChild(t)}function show(){const e=document.getElementById(\"hide-div\");e&&e.remove()}function stringToBinary(e){return e.split(\"\").map((e=>e.charCodeAt(0).toString(2).padStart(8,\"0\"))).join(\"\")}function binaryToString(e){let t=\"\";for(let o=0;o<e.length;o+=8){let n=e.slice(o,o+8);8===n.length?t+=String.fromCharCode(parseInt(n,2)):t+=\"�\"}return t}function openFileSelector(){document.getElementById(\"fileInput\").click()}function handleFileSelection(e){const t=e.target.files[0];if(t)if(\"application/json\"===t.type||t.name.endsWith(\".json\")){const e=new FileReader;e.onload=function(e){const t=e.target.result;console.log(t);const o=JSON.parse(t);for(const[e,t]of Object.entries(o)){const o=decodeFromBase64(t),n=decodeFromBase64(e);console.log(n+\"\\n\\n\"+o);const l=n.startsWith(\"!-\")?n:\"!-\"+n;localStorage.setItem(l,o),localStorage.setItem(\"protocols\",null==localStorage.getItem(\"protocols\")?n:localStorage.getItem(\"protocols\")+\";;;\"+l.substring(2))}},e.readAsText(t)}else alert(\"Please select a valid JSON file.\")}function decodeFromBase64(e){return decodeURIComponent(escape(atob(e)))}setInterval((()=>{let e=localStorage.getItem(\"protocols\");e&&e.startsWith(\";;;\")&&(e=e.substring(3),localStorage.setItem(\"protocols\",e),localStorage.setItem(\"scheduleSync\",\"true\"))})),document.getElementById(\"automaticScrolling\").checked=!0,setInterval((()=>{localStorage.setItem(\"shouldScroll\",document.getElementById(\"automaticScrolling\").checked)})),disableButton(document.getElementById(\"protocolManagerButton\")),setInterval((()=>{const e=localStorage.getItem(\"selectedLog\");e&&(enableButton(document.getElementById(\"protocolManagerButton\")),document.getElementById(\"createProtocolCommandListLi\").innerHTML!=`${e}`&&(document.getElementById(\"createProtocolCommandListLi\").innerHTML=`${e}`,document.getElementById(\"protocol-name-container\").innerHTML=\'<form><input type=\"text\" id=\"protocol-name-input\" name=\"text-input\" style=\"font-size:1rem;\"></form>\'))})),document.getElementById(\"protocolManagerButton\").addEventListener(\"click\",(()=>{const e=document.getElementById(\"createProtocolCommandListLi\").innerHTML,t=document.getElementById(\"protocol-name-input\").value;t?e?localStorage.getItem(\"!-\"+t)?alert(\"Protocol with that name already exists.\"):(localStorage.setItem(\"!-\"+t,e),localStorage.setItem(\"protocols\",null==localStorage.getItem(\"protocols\")?t:localStorage.getItem(\"protocols\")+\";;;\"+t),document.getElementById(\"createProtocolCommandListLi\").innerHTML=\"\",document.getElementById(\"protocol-name-container\").innerHTML=\"\",localStorage.removeItem(\"selectedLog\"),localStorage.setItem(\"scheduleSync\",\"true\"),disableButton(document.getElementById(\"protocolManagerButton\"))):alert(\"Please select a log!\"):alert(\"Please enter a protocol name!\")})),document.getElementById(\"import-protocol-btn\").addEventListener(\"click\",openFileSelector);const createCustom443=document.getElementById(\"send-custom-443-btn\"),createCustomIR=document.getElementById(\"send-custom-ir-btn\"),createCustomCommandGUI=document.getElementById(\"createCustomCommandGUI\");function updateGUIsuccess(){document.getElementById(\"createCustomCommandGUI\").innerHTML=\'\\n<div style=\"background:white;height:300px;width:300px;border-radius:2rem\">\\n  <div class=\"svg-box\" id=\"svg-box-success\" style=\"z-index:2020202020202020220; position:absolute; top:50%; left: 50%; transform: translate(-50%, -50%);\">\\n    <svg class=\"circular green-stroke\">\\n      <circle class=\"path\" cx=\"75\" cy=\"75\" r=\"50\" fill=\"none\" stroke-width=\"5\" stroke-miterlimit=\"10\"/>\\n    </svg>\\n    <svg class=\"checkmark green-stroke\">\\n      <g transform=\"matrix(0.79961,8.65821e-32,8.39584e-32,0.79961,-489.57,-205.679)\">\\n        <path class=\"checkmark__check\" fill=\"none\" d=\"M616.306,283.025L634.087,300.805L673.361,261.53\"/>\\n      </g>\\n    </svg>\\n  </div>\\n</div>\\n      \'}function updateGUIerror(){document.getElementById(\"createCustomCommandGUI\").innerHTML=\'\\n<div style=\"background:white;height:300px;width:300px;border-radius:2rem\">\\n  <div class=\"svg-box\" id=\"svg-box-error\" style=\"z-index:2020202020202020220; position:absolute; top:50%; left: 50%; transform: translate(-50%, -50%);\">\\n    <svg class=\"circular red-stroke\">\\n      <circle class=\"path\" cx=\"75\" cy=\"75\" r=\"50\" fill=\"none\" stroke-width=\"5\" stroke-miterlimit=\"10\"/>\\n    </svg>\\n    <svg class=\"cross red-stroke\">\\n      <g transform=\"matrix(0.79961,8.65821e-32,8.39584e-32,0.79961,-502.652,-204.518)\">\\n        <path class=\"first-line\" d=\"M634.087,300.805L673.361,261.53\" fill=\"none\"/>\\n      </g>\\n      <g transform=\"matrix(-1.28587e-16,-0.79961,0.79961,-1.28587e-16,-204.752,543.031)\">\\n        <path class=\"second-line\" d=\"M634.087,300.805L673.361,261.53\"/>\\n      </g>\\n    </svg>\\n  </div>\\n</div>\\n      \'}function waitForLocalStorageItemThenShowSuccessScreen(e){let t=0;const o=setInterval((()=>{t++,console.log(e),localStorage.getItem(e)&&(clearInterval(o),updateGUIsuccess(),showToast(\"Command sent successfully.\"),setTimeout((()=>{show(),createCustomCommandGUI.innerHTML=\"\",localStorage.removeItem(e),localStorage.setItem(\"scheduleForceSync\",\"true\")}),2e3)),t>25&&(clearInterval(o),updateGUIerror(),showToast(\"An Error occurred.\"),setTimeout((()=>{show(),createCustomCommandGUI.innerHTML=\"\",localStorage.removeItem(e),localStorage.setItem(\"scheduleForceSync\",\"true\")}),2e3))}),100)}function showToast(e){var t=document.getElementById(\"toast\");t.classList.add(\"show\"),t.innerHTML=e,setTimeout((function(){t.classList.remove(\"show\")}),3e3)}function validateInput(e){const t=e.target,o=t.value.replace(/[^01]/g,\"\");t.value!==o&&(t.value=o)}createCustomIR.addEventListener(\"click\",(()=>{hide(\"60%\");createCustomCommandGUI.innerHTML+=\'\\n<div style=\"background:white;height:75vh;width:50vw;border-radius:2rem\">\\n  <div id=\"createCustomCommandCloseBtn\" style=\"position:absolute;top:2rem;right:2rem;background-color:black;border-radius:10rem;width:3rem;height:3rem;display:flex;align-items:center;justify-content:center\">\\n    <div style=\"color:white\">✖</div>\\n  </div>\\n\\n  <div style=\"padding-left: 2rem;padding-top: 1rem;\">IR Protocol Name: *</div>\\n  <div style=\"padding-left: 2rem;\">\\n    <form>\\n      <label for=\"IRNameInput\"></label>\\n      <input type=\"text\" id=\"IRNameInput\" name=\"IRNameInput\" style=\"font-size:18px; width:80%\">\\n    </form>\\n  </div>\\n\\n  <div style=\"padding-left: 2rem;padding-top: 1rem;\">IR Address Data:</div>\\n  <div style=\"padding-left: 2rem;\">\\n    <form>\\n      <label for=\"IRAddressInput\"></label>\\n      <input type=\"text\" id=\"IRAddressInput\" name=\"IRAddressInput\" style=\"font-size:18px; width:80%\">\\n    </form>\\n  </div>\\n\\n  <div style=\"padding-left: 2rem;padding-top: 1rem;\">IR Command Data:</div>\\n  <div style=\"padding-left: 2rem;\">\\n    <form>\\n      \\x3c!-- IRDataInput ~ IRCommandInput --\\x3e\\n      <label for=\"IRDataInput\"></label>\\n      <input oninput=\"validateInput(event)\" type=\"text\" id=\"IRDataInput\" name=\"IRDataInput\" style=\"font-size:18px; width:80%\">\\n    </form>\\n  </div>\\n\\n  <div style=\"padding-left: 2rem;padding-top: 1rem;\">IR Protocol:</div>\\n  <div style=\"padding-left: 2rem;\">\\n    <form>\\n      <label for=\"IRProtocolSelect\"></label>\\n      <select id=\"IRProtocolSelect\" name=\"IRProtocolSelect\" style=\"font-size:18px; width:80%\">\\n        <option value=\"PULSE_WIDTH\">PULSE_WIDTH</option>\\n        <option value=\"PULSE_DISTANCE\">PULSE_DISTANCE</option>\\n        <option value=\"APPLE\">APPLE</option>\\n        <option value=\"DENON\">DENON</option>\\n        <option value=\"JVC\">JVC</option>\\n        <option value=\"LG\">LG</option>\\n        <option value=\"LG2\">LG2</option>\\n        <option value=\"NEC\">NEC</option>\\n        <option value=\"NEC2\">NEC2</option>\\n        <option value=\"ONKYO\">ONKYO</option>\\n        <option value=\"PANASONIC\">PANASONIC</option>\\n        <option value=\"KASEIKYO\">KASEIKYO</option>\\n        <option value=\"KASEIKYO_DENON\">KASEIKYO_DENON</option>\\n        <option value=\"KASEIKYO_SHARP\">KASEIKYO_SHARP</option>\\n        <option value=\"KASEIKYO_JVC\">KASEIKYO_JVC</option>\\n        <option value=\"KASEIKYO_MITSUBISHI\">KASEIKYO_MITSUBISHI</option>\\n        <option value=\"RC5\">RC5</option>\\n        <option value=\"RC6\">RC6</option>\\n        <option value=\"SAMSUNG\">SAMSUNG</option>\\n        <option value=\"SAMSUNGLG\">SAMSUNGLG</option>\\n        <option value=\"SAMSUNG48\">SAMSUNG48</option>\\n        <option value=\"SHARP\">SHARP</option>\\n        <option value=\"SONY\">SONY</option>\\n        <option value=\"BANG_OLUFSEN\">BANG_OLUFSEN</option>\\n        <option value=\"BOSEWAVE\">BOSEWAVE</option>\\n        <option value=\"LEGO_PF\">LEGO_PF</option>\\n        <option value=\"MAGIQUEST\">MAGIQUEST</option>\\n        <option value=\"WHYNTER\">WHYNTER</option>\\n        <option value=\"FAST\">FAST</option>\\n      </select>\\n    </form>\\n  </div>\\n\\n  <div id=\"finishCreateIRSendingData\" style=\"position:absolute;bottom:2.5rem;left:2rem;display:flex;gap:1rem;\">\\n    <div style=\"\" class=\"box\" id=\"save-as-protocol-custom-ir-btn\">\\n      <div class=\"box__label\" style=\"color:#eee\">Save as Protocol</div>\\n    </div>\\n    <div style=\"\" class=\"box\" id=\"send-without-saving-custom-ir-btn\">\\n      <div class=\"box__label\" style=\"color:#eee\">Send without Saving</div>\\n    </div>\\n  </div>\\n\\n  <div style=\"position:absolute;bottom:1rem;left:2rem;\">\\n    <span style=\"font-size:10px;\">\\n      * Only required if you want to save the command as a protocol.\\n    </span>\\n  </div>\\n</div>\\n\\n\';document.getElementById(\"createCustomCommandCloseBtn\").addEventListener(\"click\",(()=>{show(),createCustomCommandGUI.innerHTML=\"\"}));const e=document.getElementById(\"save-as-protocol-custom-ir-btn\");e.addEventListener(\"click\",(()=>{const e=document.getElementById(\"IRNameInput\").value,t=document.getElementById(\"IRAddressInput\").value,o=document.getElementById(\"IRDataInput\").value,n=document.getElementById(\"IRProtocolSelect\").value;e?t?o?n?localStorage.getItem(\"!-\"+e)?alert(\"Protocol with that name already exists.\"):(localStorage.setItem(\"!-\"+e,\"IR#\"+t+\"#\"+o+\"#\"+n),localStorage.setItem(\"protocols\",null==localStorage.getItem(\"protocols\")?e:localStorage.getItem(\"protocols\")+\";;;\"+e),show(),createCustomCommandGUI.innerHTML=\"\"):alert(\"Please enter a protocol Type!\"):alert(\"Please enter a protocol data!\"):alert(\"Please enter a protocol address!\"):alert(\"Please enter a protocol name!\")}));const t=document.getElementById(\"send-without-saving-custom-ir-btn\");t.addEventListener(\"click\",(()=>{const o=document.getElementById(\"IRAddressInput\").value,n=document.getElementById(\"IRDataInput\").value,l=document.getElementById(\"IRProtocolSelect\").value;o?n?l?(localStorage.setItem(\"scheduleToSendIR\",o+\"#\"+n+\"#\"+l),disableButton(t),disableButton(e),waitForLocalStorageItemThenShowSuccessScreen(\"ir-was-sent\")):alert(\"Please enter a protocol Type!\"):alert(\"Please enter a protocol data!\"):alert(\"Please enter a protocol address!\")}))})),createCustom443.addEventListener(\"click\",(()=>{hide(\"60%\");createCustomCommandGUI.innerHTML+=\'\\n<div style=\"background:white;height:50vh;width:50vw;border-radius:2rem\">\\n  <div id=\"createCustomCommandCloseBtn\" style=\"position:absolute;top:2rem;right:2rem;background-color:black;border-radius:10rem;width:3rem;height:3rem;display:flex;align-items:center;justify-content:center\">\\n    <div style=\"color:white\">✖</div>\\n  </div>\\n\\n  <div style=\"padding-left: 2rem;padding-top: 1rem;\">443MHz Protocol Name: *</div>\\n  <div style=\"padding-left: 2rem;padding-top: 1rem\">\\n    <form>\\n      <label for=\"443MHzNameInput\"></label>\\n      <input type=\"text\" id=\"443MHzNameInput\" name=\"443MHzNameInput\" style=\"font-size:18px; width:80%\">\\n    </form>\\n  </div>\\n\\n  <div style=\"padding-left: 2rem;padding-top: 1rem;\">443MHz Command Data:</div>\\n  <div style=\"padding-left: 2rem;\">\\n    <form>\\n      <label for=\"443MHzDataInput\"></label>\\n      <input oninput=\"validateInput(event)\" type=\"text\" id=\"443MHzDataInput\" name=\"443MHzDataInput\" style=\"font-size:18px; width:80%\">\\n    </form>\\n  </div>\\n\\n  <div id=\"finishCreate443MHzSendingData\" style=\"position:absolute;bottom:2.5rem;left:2rem;display:flex;gap:1rem;\">\\n    <div style=\"\" class=\"box\" id=\"save-as-protocol-custom-443-btn\">\\n      <div class=\"box__label\" style=\"color:#eee\">Save as Protocol</div>\\n    </div>\\n    <div style=\"\" class=\"box\" id=\"send-without-saving-custom-443-btn\">\\n      <div class=\"box__label\" style=\"color:#eee\">Send without Saving</div>\\n    </div>\\n  </div>\\n\\n  <div style=\"position:absolute;bottom:1rem;left:2rem;\">\\n    <span style=\"font-size:10px;\">\\n      * Only required if you want to save the command as a protocol.\\n    </span>\\n  </div>\\n</div>\\n\';document.getElementById(\"createCustomCommandCloseBtn\").addEventListener(\"click\",(()=>{show(),createCustomCommandGUI.innerHTML=\"\"}));const e=document.getElementById(\"save-as-protocol-custom-443-btn\");e.addEventListener(\"click\",(()=>{const e=document.getElementById(\"443MHzNameInput\").value,t=document.getElementById(\"443MHzDataInput\").value;e?t?localStorage.getItem(\"!-\"+e)?alert(\"Protocol with that name already exists.\"):(localStorage.setItem(\"!-\"+e,\"443#\"+t),localStorage.setItem(\"protocols\",null==localStorage.getItem(\"protocols\")?e:localStorage.getItem(\"protocols\")+\";;;\"+e),show(),createCustomCommandGUI.innerHTML=\"\"):alert(\"Please enter a protocol data!\"):alert(\"Please enter a protocol name!\")}));const t=document.getElementById(\"send-without-saving-custom-443-btn\");t.addEventListener(\"click\",(()=>{const o=document.getElementById(\"443MHzDataInput\").value;o?(localStorage.setItem(\"scheduleToSend443Binary\",o),disableButton(t),disableButton(e),show(),createCustomCommandGUI.innerHTML=\"\",localStorage.removeItem(localStorageItem),localStorage.setItem(\"scheduleForceSync\",\"true\")):alert(\"Please enter a protocol data!\")}))})),setInterval((()=>{const e=localStorage.getItem(\"scheduleToast\");e&&(localStorage.removeItem(\"scheduleToast\"),showToast(e))}),25);</script></body></html>");
            }

            if (header.indexOf("/api") >= 0) {
              // someone is scraping us. we do not want them to receive the <!DOCTYPE> etc.
              // client.println(header);

              if (header.indexOf("/api/send") >= 0) {
                Logger::log(LogSeverity::DEBUG, "Web request to send data: ", true);
                
                if (header.indexOf("api/send/443MHz") >= 0) {
                  int apiPathIndex = header.indexOf("/api/send/443MHz?data=");
                  bool doesDataArgumentExist = apiPathIndex >= 0;
                  int binIndex = apiPathIndex + strlen("/api/send/443MHz?data=");

                  String bin;

                  while (header[binIndex] == '0' || header[binIndex] == '1') {
                    bin += header[binIndex];
                    binIndex++;
                  }

                  if (doesDataArgumentExist) {
                    Logger::log(LogSeverity::DEBUG, "'");
                    Logger::log(LogSeverity::DEBUG, (char *)(bin.c_str()));
                    Logger::log(LogSeverity::DEBUG, "'\n");
                    client.print(bin);

                    sendDataQueue_443MHz->enqueue(bin);
                  } else {
                    Logger::log(LogSeverity::DEBUG, "\n");
                    Logger::log(LogSeverity::DEBUG, "Malformed 443MHz send request: ", true);
                    Logger::log(LogSeverity::DEBUG, (char *)(header.c_str()));
                    Logger::log(LogSeverity::DEBUG, "\n");
                    client.print("Usage: /api/send/443MHz?data=<data>");
                  }
                } else if (header.indexOf("api/send/ir")) {
                  int apiPathIndex = header.indexOf("/api/send/ir?address=");
                  bool doesAdressArgumentExist = apiPathIndex >= 0;
                  int addressIndex = apiPathIndex + strlen("/api/send/ir?address=");
                  String address;
                  while ('0' <= header[addressIndex] && header[addressIndex] <= '9') {
                    address += header[addressIndex];
                    addressIndex++;
                  }

                  // 197.0.0.1/api/send/ir?address=1&command=2&protocol=NEC
                  
                  int commandIndex = header.indexOf("&command=") + strlen("&command=");
                  bool doesCommandArgumentExist = header.indexOf("&command=") >= 0;
                  String command;
                  while ('0' <= header[commandIndex] && header[commandIndex] <= '9') {
                    command += header[commandIndex];
                    commandIndex++;
                  }
                  
                  int protocolIndex = header.indexOf("&protocol=") + strlen("&protocol=");
                  bool doesProtocolArgumentExist = header.indexOf("&protocol=") >= 0;
                  String protocol;
                  while ('a' <= header[protocolIndex] && header[protocolIndex] <= 'z' || 'A' <= header[protocolIndex] && header[protocolIndex] <= 'Z') {
                    protocol += header[protocolIndex];
                    protocolIndex++;
                  }

                  if (doesAdressArgumentExist && doesCommandArgumentExist && doesProtocolArgumentExist) {
                    String entry = address + ";" + command + ";" + protocol;
                    sendDataQueue_IR->enqueue(entry);

                    Logger::log(LogSeverity::DEBUG, "'");
                    Logger::log(LogSeverity::DEBUG, (char *)(entry.c_str()));
                    Logger::log(LogSeverity::DEBUG, "'\n");
                    client.print(entry);
                  } else {
                    Logger::log(LogSeverity::DEBUG, "\n");
                    Logger::log(LogSeverity::DEBUG, "Malformed IR send request: ", true);
                    Logger::log(LogSeverity::DEBUG, (char *)(header.c_str()));
                    Logger::log(LogSeverity::DEBUG, "\n");
                    client.print("Usage: /api/send/ir?address=&lt;address&gt;&command=&lt;command&gt;&protocol=&lt;protocol&gt;");
                  }
                } else {
                  client.print("Invalid API path.");
                }
              } else if (header.indexOf("/api/get") >= 0) {
                if (header.indexOf("/api/get/443mhz") >= 0
                  || header.indexOf("/api/get/443MHz") >= 0) {
                  cJSON *root = cJSON_CreateObject();
                  std::list<String>::iterator it_443MHz = receivedDataList_443MHz->begin();
                  std::list<String>::iterator it_443MHz_date = receivedDataDateList_443MHz->begin();
                  while (it_443MHz != receivedDataList_443MHz->end() && it_443MHz_date != receivedDataDateList_443MHz->end()) {
                    cJSON_AddStringToObject(root,
                      (char *)(it_443MHz_date->c_str()),
                      (char *)(it_443MHz->c_str()));
                    it_443MHz++;
                    it_443MHz_date++;
                  }
                  char *serialized = cJSON_Print(root);
                  String result(serialized);
                  cJSON_Delete(root);
                  free(serialized);
                  #ifdef SHORTEN_WEBSERVER_RESPONSE
                  result.replace("\t", "");
                  result.replace("\n", "");
                  #endif
                  client.print(result);
                } else if (header.indexOf("api/get/ir") >= 0
                        || header.indexOf("api/get/IR") >= 0) {
                  unsigned long counter = 0;
                  cJSON *root = cJSON_CreateObject();
                  for (String data : *receivedDataList_IR) {
                    cJSON_AddStringToObject(root, (std::to_string(counter++).c_str()), (char *)(data.c_str()));
                  }
                  char *serialized = cJSON_Print(root);
                  String result(serialized);
                  cJSON_Delete(root);
                  free(serialized);
                  #ifdef SHORTEN_WEBSERVER_RESPONSE
                  result.replace("\t", "");
                  result.replace("\n", "");
                  #endif
                  client.print(result);
                } else {
                  client.print("Invalid API path. ");
                  client.print("Header lowered: ");
                  client.print(toLower(header));
                }
              } else if (header.indexOf("/debug") >= 0) {
                client.print(header);
              } else if (header.indexOf("/clear") >= 0) {
                receivedDataDateList_443MHz->clear();
                receivedDataDateList_IR->clear();
                while (!sendDataQueue_443MHz->isEmpty()) {
                  sendDataQueue_443MHz->dequeue();
                }
                while (!sendDataQueue_IR->isEmpty()) {
                  sendDataQueue_IR->dequeue();
                }
                client.print("cleared 443MHz received Data list, IR received Data list, ");
                client.print("443MHz send Data queue, IR send Data queue.");
              }
            } else {
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");

              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}</style></head>");

              client.println("<body><h1>ESP32 Web Server</h1>");

              // client.println(Webserver_getReceivedValues());

              client.println(header);

              client.println("<br><br><br><br>Click <a href=\"/home\">here</a> to go to the web interface.<br><br><br><br>");

              // if (path contains "/api/send/") {
              //   string dataToSend = das nach dem "/api/send/"
              //   queue.add(dataToSend) 
              // }

              // if (path = "<ip>/api/send/101010101010101") {
              //   queue.add(101010101010101)
              // }

              client.println("</body></html>");
            }

            

            client.println();
            client.stop();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c; 
        }
      }
    }

    header = "";

    client.stop();
    Logger::log(LogSeverity::DEBUG, "Client disconnected.\n", true);
  }
}