

void initWiFi(){
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  
  // test esid 
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi ");
  Serial.println(esid);
  Debugln(epass);
  WiFi.begin((char*)esid.c_str(), (char*)epass.c_str());
  if ( testWifi() == 20 ) { 
      launchWeb(0);
      return;
  }
  Serial.println("Opening AP");
  setupAP();   
}

int testWifi(void) {
  int c = 0;
  Debugln("Wifi test...");  
  while ( c < 30 ) {
    if (WiFi.status() == WL_CONNECTED) { Serial.println(WiFi.localIP()); return(20); } 
    delay(500);
    Serial.print(".");    
    c++;
  }
  Serial.println("\n\nWiFi Connect timed out!");
  return(10);
} 


void setupAP(void) {
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0){
    Serial.println("no networks found");
    st ="<b>No networks found:</b>";
  } else {
    Serial.print(n);
    Serial.println(" Networks found");
    st = "\r\n\r\n<form><fieldset><legend>Available Wireless Networks</legend><ul class=form-fields>"; //"<ul>";
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" (OPEN)":"*");
      
      // Print to web SSID and RSSI for each network found
      st += "<li>";
      st +=i + 1;
      st += ": ";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" (OPEN)":"*";
      st += "</li>";
      delay(10);
     }
    st += "</ul></fieldset></form>\r\n\r\n";  //"</ul>";
  }
  Serial.println(""); 
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_AP);

  
  WiFi.softAP(host);
  WiFi.begin(host); // not sure if need but works
  Serial.print("Access point started with name ");
  Serial.println(host);
  softAPflag = false;
  inApMode=1;
  launchWeb(1);
}

void launchWeb(int webtype) 
{
    Serial.println("");
    if(softAPflag)
    {
    Serial.println("WiFi connected to :");
    Serial.println(esid); 
    Serial.println(WiFi.localIP());
    } 
       
    //Start the web server or MQTT
    if(otaFlag==1 && !inApMode)
    {
      Serial.println("Starting OTA mode.");    
      Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
      Serial.printf("Free size: %u\n", ESP.getFreeSketchSpace());
      MDNS.begin(host);
      
      server.on("/", HTTP_GET, [](){
        server.sendHeader("Connection", "close");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/html", otaServerIndex);
      });
      
      server.on("/update", HTTP_POST, [](){
        server.sendHeader("Connection", "close");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
        setOtaFlag(0); 
        ESP.restart();
      },[](){
        HTTPUpload& upload = server.upload();
        if(upload.status == UPLOAD_FILE_START)
        {
          //Serial.setDebugOutput(true);
          WiFiUDP::stopAll();
          Serial.printf("Update: %s\n", upload.filename.c_str());
          otaCount=300;
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if(!Update.begin(maxSketchSpace)){//start with max available size
            Update.printError(Serial);
          }
        } 
        else if(upload.status == UPLOAD_FILE_WRITE)
        {
          if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
            Update.printError(Serial);
          }
        } 
        else if(upload.status == UPLOAD_FILE_END)
        {
          if(Update.end(true)){ //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          } else {
            Update.printError(Serial);
          }
          Serial.setDebugOutput(false);
        }
        yield();
      });
      
      server.begin();
      Serial.printf("Ready! Open http://%s.local in your browser\n", host);
      MDNS.addService("http", "tcp", 80);
      otaTickLoop.attach(1, otaCountown);
    } 
    else 
    { 
      //setOtaFlag(1); 
      if (webtype==1 || iotMode==0)
      { //in config mode or WebControl
          if (webtype==1) 
          {           
            webtypeGlob == 1;
            Serial.println(WiFi.softAPIP());
            server.on("/", webHandleConfig);
            server.on("/a", webHandleConfigSave);
            server.on("/gpio", webHandleGpio);
            server.on("/configParams", handleConfigParamsRoot);
            server.on("/b", handleConfigSave);
            server.on("/configParamsWebPage", handleConfigParamsWebPage);
            server.on("/timer",HTTP_POST, handleConfigParamsWithPassKey);         
          } 
          else 
          {
            //setup DNS since we are a client in WiFi net
            if (!MDNS.begin(host)) 
            {
              Serial.println("Error setting up MDNS responder!");
            } 
            else 
            {
              Serial.println("mDNS responder started");
              MDNS.addService("http", "tcp", 80);
            }          
            Serial.println(WiFi.localIP());
            server.on("/", webHandleRoot);  
            server.on("/cleareeprom", webHandleClearRom);
            server.on("/gpio", webHandleGpio);
            server.on("/data",HTTP_POST, handleFOTArequest);               //FOTA
            server.on("/configParams", handleConfigParamsRoot);
            server.on("/b", handleConfigSave);
            String UUIDpath = "/"+euuid+"/config";
            const char * cUUIDpath = (char*) UUIDpath.c_str();
            server.on(cUUIDpath, handleConfigSave);
//            server.on("/"+euuid+"/config", handleConfigSave);
            server.on("/configParamsWebPage", handleConfigParamsWebPage);
            server.on("/timer",HTTP_POST, handleConfigParamsWithPassKey);
          }
          //server.onNotFound(webHandleRoot);
          server.begin();
          Serial.println("Web server started");   
          webtypeGlob=webtype; //Store global to use in loop()
        } else if(webtype!=1 && iotMode==1){ // in MQTT and not in config mode     
          //mqttClient.setBrokerDomain((char*) mqttServer.c_str());//naren
          //mqttClient.setPort(1883);//naren
          mqttClient.setServer((char*) mqttServer.c_str(),1883);
          mqttClient.setCallback(mqtt_arrived);
          mqttClient.setClient(wifiClient);
          if (WiFi.status() == WL_CONNECTED){
            if (!connectMQTT()){
                delay(2000);
                if (!connectMQTT()){                            
                  Serial.println("Could not connect MQTT.");
                  Serial.println("Starting web server instead.");
                  iotMode=0;
                  launchWeb(0);
                  webtypeGlob=webtype;
                }
              }                    
            }
      }
    }
}

void webHandleConfig(){
  IPAddress ip = WiFi.softAPIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  String s;
 
  s = "Configuration of " + hostName + " at ";
  s += ipStr;
  //s += "\r\n\r\n<p><a href=\"/gpio\">Control GPIO</a><br />";
  s += "\r\n\r\n<p><a href=\"/configParams\">Edit Config Parameters</a><br />";
  //s += "<a href=\"/configParamsWebPage\">Edit Config Parameters [Passkey req.]</a><br />";
  s += st;                                        // st -> List of available WiFi Hotspots
//  s += "<form method='get' action='a'>";
  s += "<form method='get' action='a'><fieldset><legend>Configuration Parameters</legend><ul class=form-fields>";
  s += "<label>SSID: </label><input name='ssid' length=32></br><label> Pass: </label><input name='pass' type='password' length=64></br>";
  s += "The following is not ready yet!</br>";
  s += "<label>IOT mode: </label><input type='radio' name='iot' value='0'> HTTP<input type='radio' name='iot' value='1' checked> MQTT</br>";
  
  s += "<label>PLATFORM URL [http://10.208.35.169:3300]: </label><input name='platformUrl' length=64></br>";
//  s += "<label>PORT [3300]: </label><input name='restPort' length=8></br>";
  s += "<label>APPLICATION SERVER URL [http://10.208.26.254:81/gigapan/index.php]: </label><input name='serverUrl' length=64></br>";
  s += "<label>BUTTON Nos.: </label><input name='buttonNos' length=8></br>";
//  s += "<label>BUTTON ID: </label><input name='buttonID' length=8></br>";
//  s += "<label>BUTTON VALUE: </label><input name='buttonValue' length=64></br>";
  
//  s += "<label>MQTT Broker IP/DNS: </label><input name='host' length=15></br>";
//  s += "<label>MQTT Publish topic: </label><input name='pubtop' length=64></br>";
//  s += "<label>MQTT Subscribe topic: </label><input name='subtop' length=64></br>";
  
  s += "</ul></fieldset><input type='submit'></form></p>";
//  s += "<input type='submit'></form></p>";
  s += "\r\n\r\n";
  Serial.println("Sending 200");  
  server.send(200, "text/html", s); 
}

void webHandleConfigSave(){
  // /a?ssid=blahhhh&pass=poooo
  String s;
  s = "<p>Settings saved to eeprom and reset to boot into new settings</p>\r\n\r\n";
  server.send(200, "text/html", s); 
  Serial.println("clearing EEPROM.");
  clearConfig();
  
  String qsid; 
  qsid = server.arg("ssid");
  qsid.replace("%2F","/");
  Serial.println("Got SSID: " + qsid);
  esid = (char*) qsid.c_str();
  
  String qpass;
  qpass = server.arg("pwd");
  qpass.replace("%2F","/");
  Serial.println("Got pwd: " + qpass);
  epass = (char*) qpass.c_str();

  String qiot;
  qiot= server.arg("iot");
  Serial.println("Got iot mode: " + qiot);
  qiot=="0"? iotMode = 0 : iotMode = 1 ;

  String qUrl;
  qUrl = server.arg("platformUrl");
  qUrl.replace("%2F","/");
  Serial.println("Got platformUrl: " + qUrl);
  platformUrl = (char*) qUrl.c_str();

//  platformUrl = (char*) server.arg("platformUrl").c_str();
//  Serial.print("Got platformUrl: ");
//  Serial.println(platformUrl);

//  String qPort;
//  qPort = server.arg("restPort");
//  qPort.replace("%2F","/");
//  Serial.println("Got restPort: " + qPort);
//  restPort = (char*) qPort.c_str();

  String qserverUrl;
  qserverUrl = server.arg("serverUrl");
  qserverUrl.replace("%2F","/");
//  if(qserverUrl.length())
//  { configChange = true;
    Serial.println("Got serverUrl: " + qserverUrl);
    serverUrl = (char*) qserverUrl.c_str();
//  }
//  else {Serial.println(" serverUrl: No value entered.");}
   
  String qbuttonNos;
  qbuttonNos = server.arg("buttonNos");
  qbuttonNos.replace("%2F","/");
  Serial.println("Got buttonNos: " + qbuttonNos);
  //String sbuttonNos = (char*) qUrl.c_str();
  buttonNos = qbuttonNos.toInt();
  //

   String qbuttonID;
  qbuttonID = server.arg("buttonID");
  qbuttonID.replace("%2F","/");
  Serial.println("Got buttonID: " + qbuttonID);
  //String sbuttonNos = (char*) qUrl.c_str();
  buttonID = qbuttonID.toInt();

  String qbuttonValue;
  qbuttonValue = server.arg("buttonValue");
  qbuttonValue.replace("%2F","/");
  Serial.println("Got subtop: " + qbuttonValue);
  buttonValue = (char*) qbuttonValue.c_str();

  String qsubTop;
  qsubTop = server.arg("subtop");
  qsubTop.replace("%2F","/");
  Serial.println("Got subtop: " + qsubTop);
  subTopic = (char*) qsubTop.c_str();
  
  String qpubTop;
  qpubTop = server.arg("pubtop");
  qpubTop.replace("%2F","/");
  Serial.println("Got pubtop: " + qpubTop);
  pubTopic = (char*) qpubTop.c_str();

  mqttServer = (char*) server.arg("host").c_str();
  Serial.print("Got mqtt Server: ");
  Serial.println(mqttServer);
  
  Serial.print("Settings written ");
  saveConfig()? Serial.println("sucessfully.") : Serial.println("not succesfully!");;
  Serial.println("Restarting!"); 
  delay(1000);
  ESP.reset();
}

void webHandleRoot(){
  String s;
  s = "<p>ESP8266: Device-side Configuration Portal";
  s += "</p>";
  //s += "<a href=\"/gpio\">Control GPIO</a><br />";
  s += "<a href=\"/configParams\">Edit Config Parameters</a><br />";
  //s += "<a href=\"/configParamsWebPage\">Edit Config Parameters [Passkey req.]</a><br />";
  s += "<a href=\"/cleareeprom\">Clear settings an boot into Config mode</a><br />";
  s += "\r\n\r\n";
  Serial.println("Sending 200");  
  server.send(200, "text/html", s); 
}

void webHandleClearRom(){
  String s;
  s = "<p>Clearing the config and reset to configure new wifi<p>";
  s += "</html>\r\n\r\n";
  Serial.println("Sending 200"); 
  server.send(200, "text/html", s); 
  Serial.println("clearing config");
  clearConfig();
  delay(10);
  Serial.println("Done, restarting!");
  ESP.reset();
}

void webHandleGpio(){
  String s;
   // Set GPIO according to the request
    if (server.arg("state_sw")=="1" || server.arg("state_sw")=="0" ) {
      int state_sw = server.arg("state_sw").toInt();
      digitalWrite(OUTPIN, state_sw);
      Serial.print("Light switched via web request to  ");      
      Serial.println(state_sw);      
    }
     if (server.arg("state_led")=="1" || server.arg("state_led")=="0" ) {
      int state_led = server.arg("state_led").toInt();
      digitalWrite(OUTLED, state_led);
      Serial.print("Light switched via web request to  ");      
      Serial.println(state_led);      
    }
    
    s = "Light is now ";
    s += (digitalRead(OUTPIN))?"on":"off";
    s += "<p>Change to <form action='gpio'><input type='radio' name='state_sw' value='1' ";
    s += (digitalRead(OUTPIN))?"checked":"";
    s += ">LIGHT_On<input type='radio' name='state_sw' value='0' ";
    s += (digitalRead(OUTPIN))?"":"checked";
    s += ">LIGHT_Off <input type='submit' value='Submit'></form></p>";   

    s += "LED is now ";
    s += (digitalRead(OUTLED))?"ON":"OFF";
    s += "<p>Change to <form action='gpio'><input type='radio' name='state_led' value='1' ";
    s += (digitalRead(OUTLED))?"checked":"";
    s += ">LED_ON <input type='radio' name='state_led' value='0' ";
    s += (digitalRead(OUTLED))?"":"checked";
    s += ">LED_OFF <input type='submit' value='Submit'></form></p>"; 
    server.send(200, "text/html", s);    
}
                                                  /*---------------------
                                                   * To Make FOTA update
                                                   *--------------------*/

void handleFOTArequest()
{
  server.sendHeader("Location","/");
  if(!server.hasArg("token") || server.arg("token") !="123456789")
      server.send(400,"text/html","<h1>HTTP 400:</h1><h3> Bad Request</h3>");
  else
  {    
        String location = server.arg("binary_location");
        Serial.println(location);
        if(location != NULL)
        {
          Serial.print("New location received : ");         
          Serial.println(location);
          server.send(200,"{'status':'ok'}");
          
          /*---------------------
           * To Make OTA update
           *--------------------*/
         t_httpUpdate_return ret = ESPhttpUpdate.update(location);
         //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

         switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                server.send(404,"{'status':'Failed'}");
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                server.send(400,"{'status':'No Update'}");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                server.send(200,"{'status':'ok'}");
                break;
          }
        }
        else
          server.send(400,"text/html","<h1>HTTP 400:</h1><h3> Bad Request</h3>");    
  }
}
