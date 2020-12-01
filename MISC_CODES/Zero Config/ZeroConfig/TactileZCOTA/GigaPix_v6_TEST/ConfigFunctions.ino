//bool loadConfig() {
//  File configFile = SPIFFS.open("/config.json", "r");
//  if (!configFile) 
//  { Serial.println("Failed to open config file for reading");
//    return false;
//  }
//
//  size_t size = configFile.size();
//  if (size > 1024) 
//  { Serial.println("Config file size is too large");
//    return false;
//  }
//
//  std::unique_ptr<char[]> buf(new char[size]);  // Allocate a buffer to store contents of the file.
//
//  configFile.readBytes(buf.get(), size);  // We don't use String here because ArduinoJson library requires the input buffer to be mutable. 
//                                          //If you don't use ArduinoJson, you may as well use configFile.readString instead.
//                                          
//  StaticJsonDocument<400> jsonBuffer;       //Memory pool for JSON object tree = 400bytes
//  // StaticJsonDocument allocates memory on the stack, it can be replaced by DynamicJsonBuffer which allocates in the heap.
//  // DynamicJsonBuffer  jsonBuffer(400);
//
//  
//  //jsonBufferect jsonBuffer = jsonBuffer.parseObject(buf.get()); // jsonBuffer = Root of the object tree
//  // It's a reference to the jsonBufferect, the actual bytes are inside the JsonBuffer with all the other nodes of the object tree.
//  // Memory is freed when jsonBuffer goes out of scope.
//
//
//  auto error = deserializeJson(jsonBuffer, buf.get());
//  if (error) {
//      Serial.print(F("deserializeJson() failed with code "));
//      Serial.println(error.c_str());
//      return false;
//  }
//
////  if (!jsonBuffer.success()) { // Test if parsing succeeds.
////    Serial.println("Failed to parse config file");
////    return false;
////  }
//
//  int otaFlagC = jsonBuffer["otaFlag"];  // Fetch values.
//  String esidC = jsonBuffer["esid"];
//  String epassC = jsonBuffer["epass"];
////  const char* euuidC = jsonBuffer["euuid"];
//  String euuidC = jsonBuffer["euuid"];
//  int iotModeC = jsonBuffer["iotMode"];
//  String platformUrlC = jsonBuffer["platformUrl"];
//  String restPortC = jsonBuffer["restPort"];
//  String serverUrlC = jsonBuffer["serverUrl"];
//  int buttonNosC = jsonBuffer["buttonNos"];
//  int buttonIDC = jsonBuffer["buttonID"];  
//  String buttonValueC = jsonBuffer["buttonValue"];
//  String pubTopicC = jsonBuffer["pubTopic"];
//  String subTopicC = jsonBuffer["subTopic"];
//  String mqttServerC = jsonBuffer["mqttServer"];
//  
//
//  // Real world application would store these values in some variables for later use.
//  otaFlag   = otaFlagC;
//  esid      = esidC;
//  euuid      = euuidC;
//  epass     = epassC;
//  iotMode   = iotModeC;
//  platformUrl   = platformUrlC;
//  restPort  = restPortC;
//  serverUrl   = serverUrlC;
//  buttonNos = buttonNosC;
//  buttonID = buttonIDC;
//  buttonValue = buttonValueC;
//  pubTopic  = pubTopicC;
//  subTopic  = subTopicC;
//  mqttServer = mqttServerC;
//  
//  
//  Serial.print("otaFlag: "); 
//  Serial.println(otaFlag);
//  Serial.print("esid: ");
//  Serial.println(esid);
//  Serial.print("epass: ");
//  Serial.println(epass);
//  Serial.print("euuid: ");
//  Serial.println(euuid);
//  Serial.print("iotMode: ");
//  Serial.println(iotMode);
//  Serial.print("platformUrl: ");
//  Serial.println(platformUrl);
////  Serial.print("restPort: ");
////  Serial.println(restPort);
//  Serial.print("serverUrl: ");
//  Serial.println(serverUrl);
//  Serial.print("buttonNos: ");
//  Serial.println(buttonNos);
//  Serial.print("buttonID: ");
//  Serial.println(buttonID);
//  Serial.print("buttonValue: ");
//  Serial.println(buttonValue);
//  Serial.print("pubTopic: ");
//  Serial.println(pubTopic);
//  Serial.print("subTopic: ");
//  Serial.println(subTopic);
//  Serial.print("mqttServer: ");
//  Serial.println(mqttServer);
// 
//  return true;
//}

//bool saveConfig() {
//  StaticJsonDocument<400> jsonBuffer; //Memory pool for JSON object tree = 400bytes
//  jsonBuffer jsonBuffer = jsonBuffer.createObject();
//  jsonBuffer["otaFlag"] = otaFlag;
//  jsonBuffer["esid"] = esid;
//  jsonBuffer["epass"] = epass;
//  jsonBuffer["euuid"] = euuid;
//  jsonBuffer["iotMode"] = iotMode;
//  jsonBuffer["platformUrl"] = platformUrl;
////  jsonBuffer["restPort"] = restPort;
//  jsonBuffer["serverUrl"] = serverUrl;
//  jsonBuffer["buttonNos"] = buttonNos;
//  jsonBuffer["buttonID"] = buttonID;
//  jsonBuffer["buttonValue"] = buttonValue;
//  jsonBuffer["pubTopic"] = pubTopic;
//  jsonBuffer["subTopic"] = subTopic;
//  jsonBuffer["mqttServer"] = mqttServer;
//
//  
//  
//
//  File configFile = SPIFFS.open("/config.json", "w");
//  if (!configFile) {
//    Serial.println("Failed to open config file for writing");
//    return false;
//  }
//
//  jsonBuffer.printTo(configFile);
//  return true;
//}


void setOtaFlag(int intOta){
  otaFlag=intOta;
  
  // saveConfig();
  
  yield();
}

bool clearConfig(){
    Debugln("DEBUG: In config clear!");
    return SPIFFS.format();  
}
