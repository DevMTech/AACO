bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);
  
  StaticJsonBuffer<400> jsonBuffer;       //Memory pool for JSON object tree = 400bytes
  // StaticJsonBuffer allocates memory on the stack, it can be replaced by DynamicJsonBuffer which allocates in the heap.
  // DynamicJsonBuffer  jsonBuffer(400);
  
  JsonObject& json = jsonBuffer.parseObject(buf.get()); // jsonObj = Root of the object tree
  // It's a reference to the JsonObject, the actual bytes are inside the JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.


  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  /*for( const auto& kv : json ) {
    root[kv.key] = json[kv.key][kv.value].as<JsonObject>();
  }
  Serial.println("loadingJSON ");
  Serial.println(json["Networking"]["SSID"].as<char*>());*/
  Serial.print("loadingROOT ");
  Serial.println(root["Networking"]["SSID"].as<char*>());
  jsonBuffer.clear();
  return true;
}

bool saveConfig(JsonObject& json) {

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  Serial.println("saving ");
  parseObjects(json);
  Serial.println(json["Networking"]["SSID"].as<char*>());
  json.printTo(configFile);
  return true;
}


void setOtaFlag(int intOta){
  otaFlag=intOta;
  //saveConfig();
  yield();
}

bool clearConfig(){
    Debugln("DEBUG: In config clear!");
    return SPIFFS.format();  
}



