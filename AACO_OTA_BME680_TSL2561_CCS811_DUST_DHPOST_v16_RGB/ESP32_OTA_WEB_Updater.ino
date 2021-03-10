/* https://lastminuteengineers.com/esp32-ota-web-updater-arduino-ide/
  Generate a .bin file in Arduino IDE
  To generate a .bin file from your sketch, go to Sketch > Export compiled Binary

  A new file on the folder sketch should be created. Go to Sketch > Show Sketch Folder.
  You should have two files in your Sketch folder: the .ino and the .bin file.
  You should upload the .bin file using the OTA Web Updater.

  // https://www.w3schools.com/jsref/met_win_open.asp


*/

// http://10.42.0.81/ota


// UNCOMMENT TO ENABLE SERIAL PORT DEBUG MESSAGES
//#define DEBUG_Sprintln(a) (Serial.println(a))
//#define DEBUG_Sprint(a) (Serial.print(a))
//#define DEBUG_Sprint2ln(a, b) (Serial.println(a, b)) // DEBUG_Sprint2ln((int) number, DEC);
//#define DEBUG_Sprint2(a, b) (Serial.print(a, b))

// UNCOMMENT TO DISABLE SERIAL PORT DEBUG MESSAGES
//#define DEBUG_Sprintln(a)
//#define DEBUG_Sprint(a)
//#define DEBUG_Sprint2ln(a, b)
//#define DEBUG_Sprint2(a, b)


//#include <WiFi.h>
//#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>


#define AP_SSID  "ESP32AP"
const char* hostname = "esp32"; // http://esp32.local

//const char* ssid = "NeMo";                   const char* password = "";
//String deviceName = "", deviceDescription, deviceConfigParams;
//String deviceMAC, deviceIP;

//variabls for blinking an LED with Millis
//const int led = 2; // ESP32 Pin to which onboard LED is connected
//unsigned long previousMillis = 0;  // will store last time LED was updated
//const long interval = 1000;  // interval at which to blink (milliseconds)
//int ledState = LOW;  // ledState used to set the LED

WebServer server(80);

// Style
String style =
  "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
  "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
  "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
  "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
  "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
  ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

// Login page
String loginPage1 =
  "<form name=loginForm>"
  "<h1>ESP32 Login</h1>"
  "<input name=userid placeholder='User ID'> "
  "<input name=pwd placeholder=Password type=Password> "
  "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
  "<script>"
  "function check(form) {"
  "if(form.userid.value=='admin' && form.pwd.value=='3210')"
  "{window.open('/binUpload";

String loginPage2 =
  "')}else" //,"\_self\" // ,"_parent" // ,'_self'
  "{alert('Error: Password or Username!')}"
  "}"
  "</script>" + style;

// bin File Upload Page
String binFileUploadPage =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
  "<label id='file-input' for='file'>   Choose file...</label>"
  "<input type='submit' class=btn value='Update'>"
  "<br><br>"
  "<div id='prg'></div>"
  "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
  "<script>"
  "function sub(obj){"
  "var fileName = obj.value.split('\\\\');"
  "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
  "};"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  "$.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "$('#bar').css('width',Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!') "
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>" + style;


 

String randString;
String serverIndexRandStr = "";

// setup function
void OTAsetup(void)
{
  OTADuration = millis();
  //  pinMode(led, OUTPUT);

  // readChipID(); // initialize the global variable 'ChipID' 
  
  //  deviceMAC = WiFi.macAddress();              //macToStr(mac);  //mac;
  //  Serial.print("\n\nMAC : "); // 84:0D:8E:C3:60:8C ESP32S
  //  Serial.println(deviceMAC); // (WiFi.macAddress());

  //WiFiSetup();

  WebOTA();

  Serial.println("\n\n OTA WEBSERVER RUNNING FOR NEXT 15s . . .\n");

  for (int i = 0; i < 15000; i++)
  {
    //createRandomString();
    server.handleClient(); // OTA WEBSERVER 
    delay(1);
    //randString = "";
    if (i % 5000 == 0)      Serial.println(i);
  }

  Serial.println("EXITING OTA!");
  OTADuration = millis() - OTADuration;
}

void WebOTA()
{
  Serial.println("\n\n---------------WebOTA-----------------\n");

  //  WiFi.mode(WIFI_AP);
  //  Serial.println("** Starting AP");
  //  WiFi.softAP(AP_SSID);

  //  Serial.print("Connected to ");
  //  Serial.println(WiFi.SSID());

  // deviceIP = WiFi.localIP().toString().c_str();


  if (!MDNS.begin(hostname)) //use mdns for host name resolution
  { Serial.println("Error setting up MDNS responder!"); // http://esp32.local/ota
    while (1)    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
//  Serial.print("Access the OTA webserver at http://");
//  Serial.print(hostname);
//  Serial.print(".local/");


  Serial.print("\n Visit for FOTA : http://");
  Serial.print(WiFi.localIP()); // (deviceIP); //
  Serial.println("/ota");

  // Serial.println("\n Visit for OTA: http://esp32.local/ota \n");
  // http://10.208.34.23/ota
  // http://10.208.22.164/ota

  // http://esp.local/ota
  // http://192.168.0.106/ota
  // http://192.168.0.100/binUpload
  // http://esp32.local/ota

  createRandomString();

  serverIndexRandStr = "/binUpload" + randString; // ChipID; // deviceMAC;

  //  Serial.println( serverIndexRandStr);
  //  Serial.println("\nloginPage1+randString+loginPage2");
  //  Serial.println(loginPage1+randString+loginPage2);
  //  Serial.println();

  //---------------------------------------------------------------------------------------//

  //return index page which is stored in serverIndex
  server.on("/ota", HTTP_GET, []() {
    server.sendHeader("Connection", "close"); //
    server.send(200, "text/html", loginPage1 + randString + loginPage2); // loginPage1+ChipID+loginPage2);
  });

  //---------------------------------------------------------------------------------------//

  //server.on("/serverIndexD84CA9E350CC", HTTP_GET, []() { // D84CA9E350CC
  server.on(serverIndexRandStr, HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", binFileUploadPage);
  });

  //---------------------------------------------------------------------------------------//


  // handling uploading firmware file
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []()
  {
    HTTPUpload& upload = server.upload();
    long int fileSize = sizeof(upload);
    //Serial.print("fileSize: ");    Serial.print(fileSize);    Serial.println("bytes");
    //    Serial.print("upload: ");    Serial.println(sizeof(upload));



    if (upload.status == UPLOAD_FILE_START)
    { Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN))
      {
        Update.printError(Serial); //start with max available size
      }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    { if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)// flashing firmware to ESP
      { Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_END)
    { if (Update.end(true)) //true to set the size to the current progress
      { Serial.printf(" Update Success: %u bytes??\n Rebooting...\n", upload.totalSize);
      }
      else
      { Update.printError(Serial);
      }
    }
  }); // end of server.on("/update" ...
  //---------------------------------------------------------------------------------------//

  server.begin();

  delay(5000);
}

//String loginPage =
//  "<form name=loginForm>"
//  "<h1>ESP32 Login</h1>"
//  "<input name=userid placeholder='User ID'> "
//  "<input name=pwd placeholder=Password type=Password> "
//  "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
//  "<script>"
//  "function check(form) {"
//  "if(form.userid.value=='admin' && form.pwd.value=='admin')"
//  "{window.open('/binUpload')}"
//  "else"
//  "{alert('Error: Password or Username!')}"
//  "}"
//  "</script>" + style;

//void WiFiSetup()
//{
//    // Connect to WiFi network
//  WiFi.begin(ssid, password);
//  Serial.println("");
//
//  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(WiFi.SSID());
//
//  deviceIP = WiFi.localIP().toString().c_str();
//  Serial.print("IP address: ");
//  Serial.println(deviceIP); //(WiFi.localIP());
//
//}

//void loop(void)
//{
//  //createRandomString();
//  //server.handleClient();
//  //delay(1);
//
//  //loop to blink without delay
//  unsigned long currentMillis = millis();
//  if (currentMillis - previousMillis >= interval)
//  {
//    // save the last time you blinked the LED
//    previousMillis = currentMillis;
//    // if the LED is off turn it on and vice-versa:
//    ledState = not(ledState);
//    // set the LED with the ledState of the variable:
//    digitalWrite(led, ledState);
//  }
//
//}
