
//const size_t bufferSize = 2*JSON_ARRAY_SIZE(1) + 3*JSON_ARRAY_SIZE(3) + 2*JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(2) + 4*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5);


//char JSONmessageBuffer[JSON_BUFFER_SIZE_BYTES];
//char JSONmessageBuffer[bufferSize];


//void createJson() //do error checking and return TRUE/FLASE(bool) on success/failure in creating JSON
//{     
//
//    //const size_t bufferSize = 2*JSON_ARRAY_SIZE(1) + 3*JSON_ARRAY_SIZE(3) + 2*JSON_ARRAY_SIZE(5) + JSON_OBJECT_SIZE(2) + 4*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5);
////    DynamicJsonBuffer jsonBuffer(bufferSize);
//
//    StaticJsonDocument<JSON_BUFFER_SIZE_BYTES> jsonBuffer; 
//    // Memory pool for JSON object tree. JSON_BUFFER_SIZE_BYTES is the size of the pool in bytes.
//    
//    jsonBuffer root = jsonBuffer.createObject();
//    
//    jsonBuffer device = root.createNestedObject("device");
//    device["mac"] = WiFi.macAddress();    // "2:2:4:4:4"; //"11:22:33:44:98";
//    device["ip"] = WiFi.localIP().toString(); //WiFi.localIP().toString().c_str();   //"127.0.0.1";
//    device["name"] = "GigaPix";
//    device["description"] = "Touch Panel";
//    
//    jsonBuffer firmware = root.createNestedObject("firmware");
//    firmware["TargetedApplication"] = "";
//    firmware["Version_Number"] = "1.0";
//    
//    JsonArray& actions = root.createNestedArray("actions");
//    
//    jsonBuffer actions_0 = actions.createNestedObject();
//    actions_0["actionid"] = 1;
////    
//    JsonArray& actions_0_key = actions_0.createNestedArray("key");
//    actions_0_key.add(1);
////    actions_0_key.add(2);
////    actions_0_key.add(3);
////    actions_0_key.add(4);
////    actions_0_key.add(5);
////    
//    JsonArray& actions_0_values = actions_0.createNestedArray("values");
//    actions_0_values.add("1");
////    actions_0_values.add("2");
////    actions_0_values.add("3");
////    actions_0_values.add("4");
////    actions_0_values.add("5");
//    actions_0["description"] = "Change Key value";
////    
//    JsonArray& events = root.createNestedArray("events");
////    
//    jsonBuffer events_0 = events.createNestedObject();
//    events_0["eventid"] = 1;
//    
//    JsonArray& events_0_key = events_0.createNestedArray("key");
//    events_0_key.add("");
//    events_0_key.add("ButtonID");
////    events_0_key.add("Value");
//    events_0_key.add("PressType");
////    
//    JsonArray& events_0_values = events_0.createNestedArray("values");
//    events_0_values.add("1");
////    events_0_values.add("");
////    
////    JsonArray& events_0_values_2 = events_0_values.createNestedArray();
////    events_0_values_2.add(1);
////    events_0_values_2.add(2);
////    events_0_values_2.add(3);
//
//    events_0["description"] = "Triggers";
////    
//    jsonBuffer configuration = root.createNestedObject("configuration");
//    configuration["ssid"] = esid; //"cdac";
//    configuration["pwd"] = epass; //"";
//    configuration["buttonNos"] = buttonNos; //"";
//
////    DEV_REG_host = "http://"+platformUrl; // +":"+ restPort;    //10.208.35.169 //3300
////    configuration["PlatformUrl"] = DEV_REG_host;    //"http://10.208.35.169:3300";
//    
////    configuration["ServerUrl"] = serverUrl;  "http://10.208.26.254:81"; //  MAKE THIS USER DEPENDENT
//    
//    //root.printTo(Serial);
//
//    root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
//   //JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
//   Serial.println("\n\n\n\t# pretty-JSON output:\t***************************\n\n");
//   Serial.println(JSONmessageBuffer);
//
//   
////   if (!jsonBuffer.success()) { // Test if parsing succeeds.
////    Serial.println("Failed to parse config file");
////    return false;
////  }
//   
//  
//}

//https://arduinojson.org/v5/assistant/
//https://arduinojson.org/assistant/
/*{
 "device":{
           "mac":"11:22:33:44:98",
           "ip":"127.0.0.1",
           "name":"GigaPix",
           "description":"Integrate into IoTplatform"
 },

 "firmware":{
           "TargetedApplication":"Touch Panel",
           "Version_Number":"1.0.0"
 },

 "actions":[
               {
                 "actionid":1,
                  "key":[1,2,3,4,5],
                 "values":["1", "2", "3", "4", "5"],               
                 "description":"Change the Key values"
               },    
             ],

  "events":[
               {
                 "eventid": 1,
                 "key": ["ButtonID", "Value", "PressType"],
                 "values": ["1", "Single Click", [1, 2, 3]],
                 "description": "Triggers on event of button press"
               },
             ],

  "configuration": { "ssid": "cdac", "pwd": "", "PlatformUrl": "http://10.208.34.54:3300", "ServerUrl":"http://10.208.35.169:3301" }
}
*/
