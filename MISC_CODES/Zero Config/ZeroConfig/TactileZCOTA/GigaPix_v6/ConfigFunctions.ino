bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) 
  { Serial.println("Failed to open config file for reading");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) 
  { Serial.println("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);  // Allocate a buffer to store contents of the file.

  configFile.readBytes(buf.get(), size);  // We don't use String here because ArduinoJson library requires the input buffer to be mutable. 
                                          //If you don't use ArduinoJson, you may as well use configFile.readString instead.
                                          
  StaticJsonBuffer<400> jsonBuffer;       //Memory pool for JSON object tree = 400bytes
  // StaticJsonBuffer allocates memory on the stack, it can be replaced by DynamicJsonBuffer which allocates in the heap.
  // DynamicJsonBuffer  jsonBuffer(400);
  
  JsonObject& jsonObj = jsonBuffer.parseObject(buf.get()); // jsonObj = Root of the object tree
  // It's a reference to the JsonObject, the actual bytes are inside the JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.

  if (!jsonObj.success()) { // Test if parsing succeeds.
    Serial.println("Failed to parse config file");
    return false;
  }

  int otaFlagC = jsonObj["otaFlag"];  // Fetch values.
  String esidC = jsonObj["esid"];
  String epassC = jsonObj["epass"];
//  const char* euuidC = jsonObj["euuid"];
  String euuidC = jsonObj["euuid"];
  int iotModeC = jsonObj["iotMode"];
  String platformUrlC = jsonObj["platformUrl"];
  String restPortC = jsonObj["restPort"];
  String serverUrlC = jsonObj["serverUrl"];
  int buttonNosC = jsonObj["buttonNos"];
  int buttonIDC = jsonObj["buttonID"];  
  String buttonValueC = jsonObj["buttonValue"];
  String pubTopicC = jsonObj["pubTopic"];
  String subTopicC = jsonObj["subTopic"];
  String mqttServerC = jsonObj["mqttServer"];
  

  // Real world application would store these values in some variables for later use.
  otaFlag   = otaFlagC;
  esid      = esidC;
  euuid      = euuidC;
  epass     = epassC;
  iotMode   = iotModeC;
  platformUrl   = platformUrlC;
  restPort  = restPortC;
  serverUrl   = serverUrlC;
  buttonNos = buttonNosC;
  buttonID = buttonIDC;
  buttonValue = buttonValueC;
  pubTopic  = pubTopicC;
  subTopic  = subTopicC;
  mqttServer = mqttServerC;
  
  
  Serial.print("otaFlag: "); 
  Serial.println(otaFlag);
  Serial.print("esid: ");
  Serial.println(esid);
  Serial.print("epass: ");
  Serial.println(epass);
  Serial.print("euuid: ");
  Serial.println(euuid);
  Serial.print("iotMode: ");
  Serial.println(iotMode);
  Serial.print("platformUrl: ");
  Serial.println(platformUrl);
//  Serial.print("restPort: ");
//  Serial.println(restPort);
  Serial.print("serverUrl: ");
  Serial.println(serverUrl);
  Serial.print("buttonNos: ");
  Serial.println(buttonNos);
  Serial.print("buttonID: ");
  Serial.println(buttonID);
  Serial.print("buttonValue: ");
  Serial.println(buttonValue);
  Serial.print("pubTopic: ");
  Serial.println(pubTopic);
  Serial.print("subTopic: ");
  Serial.println(subTopic);
  Serial.print("mqttServer: ");
  Serial.println(mqttServer);
 
  return true;
}

bool saveConfig() {
  StaticJsonBuffer<400> jsonBuffer; //Memory pool for JSON object tree = 400bytes
  JsonObject& jsonObj = jsonBuffer.createObject();
  jsonObj["otaFlag"] = otaFlag;
  jsonObj["esid"] = esid;
  jsonObj["epass"] = epass;
  jsonObj["euuid"] = euuid;
  jsonObj["iotMode"] = iotMode;
  jsonObj["platformUrl"] = platformUrl;
//  jsonObj["restPort"] = restPort;
  jsonObj["serverUrl"] = serverUrl;
  jsonObj["buttonNos"] = buttonNos;
  jsonObj["buttonID"] = buttonID;
  jsonObj["buttonValue"] = buttonValue;
  jsonObj["pubTopic"] = pubTopic;
  jsonObj["subTopic"] = subTopic;
  jsonObj["mqttServer"] = mqttServer;

  
  

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  jsonObj.printTo(configFile);
  return true;
}


void setOtaFlag(int intOta){
  otaFlag=intOta;
  saveConfig();
  yield();
}

bool clearConfig(){
    Debugln("DEBUG: In config clear!");
    return SPIFFS.format();  
}

