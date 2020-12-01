void initWiFi(){
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  
  // test esid 
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi ");
  String esid = root["network"]["SSID"];
  String epass = root["network"]["Password"];
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
    if (WiFi.status() == WL_CONNECTED) { return(20); } 
    delay(500);
    Serial.print(".");    
    c++;
  }
  Serial.println("WiFi Connect timed out!");
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
    st = "<form><fieldset><legend>Available Networks</legend><ul class=form-fields>";
    for (int i = 0; i < n; ++i)
     {     
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
      st += "</ul></fieldset></form>\r\n\r\n";

  }
  Serial.println(""); 
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_AP);

  
  WiFi.softAP(host);
  //WiFi.begin(host); // not sure if need but works
  Serial.print("Access point started with name ");
  Serial.println(host);
  inApMode=1;
  launchWeb(1);
}

void launchWeb(int webtype) {
   IPAddress ip = WiFi.softAPIP();
   String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
   root["Device"]["IP"] = ipStr;
   webtypeGlob = 1;
   Serial.println(WiFi.softAPIP());
   server.on("/", webHandleConfig);
   server.on("/a", webHandleConfigSave);
   server.begin();
   Serial.println("Web server started");
}

void webHandleConfig(){
  
  IPAddress ip = WiFi.softAPIP();
  String s;
  s += st;
  s += "<form method='get' action='a'><fieldset><legend>Networking Parameters</legend><ul class=form-fields>";
  for( const auto& kv : iRoot["Networking"].as<JsonObject>() ) {  //SSID, Password
        s += "<li><label>";
        s += kv.key;
        s += " : </label><input name= ";
        s += kv.key;
        String temp = kv.value.as<char*>();
        if(temp.length() > 0 ){
        s += " value= ";
        s += kv.value.as<char*>();
        s +=  " readonly>";}
        s += "</li>";
  }
  s += "</ul></fieldset>\r\n\r\n<fieldset><legend>Configuration Parameters</legend><ul class=form-fields>";
  for( const auto& kv : iRoot["Configuration"].as<JsonObject>() ) { //PlatformURl, Button
        s += "<li>";
        s += "<label>";
        s += kv.key;
        s += " : ";
        s += "</label>";
        s += "<input name= ";
        s += kv.key;
        String temp = kv.value.as<char*>();
        if(temp.length() > 0 ){
        s += " value= ";
        s += kv.value.as<char*>();}
        s +=  " length=32>";
        s += "</li>";
  }

  s += "</ul></fieldset><input type='submit'></form>";
  
  s += "\r\n\r\n\r\n<fieldset><legend>Device Parameters</legend><ul class=form-fields>";
  for( const auto& kv : iRoot["Device"].as<JsonObject>() ) {  //MAC, IP, UUID
        s += "<li><label>";
        s += kv.key;
        s += " : </label><input name= ";
        s += kv.key;
        String temp = kv.value.as<char*>();
        if(temp.length() > 0 ){
        s += " value= ";
        s += kv.value.as<char*>();
        s +=  " readonly>";}
        s += "</li>";
  }
  s += "</ul></fieldset>\r\n\r\n<fieldset><legend>Firmware Parameters</legend><ul class=form-fields>";
  for( const auto& kv : iRoot["Firmware"].as<JsonObject>() ) {  //Application, Version
        s += "<li><label>";
        s += kv.key;
        s += " : </label><input name= ";
        s += kv.key;
        String temp = kv.value.as<char*>();
        if(temp.length() > 0 ){
          s += " value= ";
          s += kv.value.as<char*>();
          s +=  " readonly>";
        }
        s += "</li>";
  }
  s += "</ul></fieldset>\r\n\r\n";
  
  Serial.println("Sending 200");  
  server.send(200, "text/html", s); 
  jB.clear();
}

void webHandleConfigSave(){
  String s;
  s = "<p>Settings saved to eeprom and reset to boot into new settings</p>\r\n\r\n";
  server.send(200, "text/html", s); 
  Serial.println("clearing EEPROM.");
  //clearConfig();
  /*for (uint8_t i = 0; i < server.args(); i++) {
    String t = server.argName(i);
    t.replace("%3C%2Fli","");
    Serial.println(" " + t + ": " + server.arg(i));
  }*/
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonObject& network = json.createNestedObject("Networking");
  for( const auto& kv : iRoot["Networking"].as<JsonObject>() ) {
    String k = kv.key;
    k += "%3C%2Fli";
    String res  = server.arg(k);
    String kk = kv.key;
    //Serial.println(kk+" : "+res);
    network[kk]=res;
  }
   JsonObject& conf = json.createNestedObject("Config");
  //json = jsonBuffer.createObject();
  for( const auto& kv : iRoot["Configuration"].as<JsonObject>() ) {
    String res  = server.arg(kv.key);
    res.replace("%2F","/");
    String s = res;
    String k = kv.key;
    //Serial.println(k+" : "+s);
    conf[k]=s;
  }
  //root=json;
  //json = jsonBuffer.createObject();
  
  Serial.print("Settings written ");
  parseObjects(json);
  saveConfig(json)? Serial.println("sucessfully.") : Serial.println("not succesfully!");;
  jsonBuffer.clear();
  Serial.println("Restarting!"); 
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  ESP.reset();
}

void webHandleClearRom(){
  String s;
  s = "<p>Clearing the config and reset to configure new wifi<p>";
  s += "</html>\r\n\r\n";
  Serial.println("Sending 200"); 
  server.send(200, "text/html", s); 
  Serial.println("clearing config");
  //clearConfig();
  delay(10);
  Serial.println("Done, restarting!");
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  ESP.reset();
}


