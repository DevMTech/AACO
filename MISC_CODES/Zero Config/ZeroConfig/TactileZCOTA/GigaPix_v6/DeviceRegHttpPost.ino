/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

bool uuid_flag = false;

void registerDevice()
{
/**/ //Device registration - HTTP POST - WOULD HAPPEN ONLY SUCCESSFUL CONECTION TO HOTSPOT
  if (WiFi.status() == WL_CONNECTED) //CHECK CONDITION - WOULD EXECUTE AFTER SUCCESSFUL CONECTION TO HOTSPOT
  { softAPflag = true;   
    //deviceName="GigaPix";
    
    deviceDescription="Tactile Touch Panel";
    deviceMAC=WiFi.macAddress();                //macToStr(mac);  //mac;
    deviceIP=WiFi.localIP().toString().c_str();

    deviceConfigParams ='{"buttonNos":'+buttonNos+'}';
    
    //deviceConfigParams ="{\"buttonNos\": 12}";  //"";
    //deviceConfigParams ="{\"buttonNos\":" + buttonNos + "}";  //"";
    
    //deviceConfigParams ='{"buttonNos":'+buttonNos+',"threshold":'+1000+'}';
    //data_string = '{"ble_id":   '+count+', "mac": '+"\""+a[0]+"\""+', "rssi": '+a[5]+', "distance": \"'+str(rounded)+'\", "time-stamp": '+"\""+str(t)+'\", "ble_master": "RPi_1"}'

    DEV_REG_host = platformUrl; // "http://"+platformUrl+":"+ restPort;    //http://10.208.35.169:3300
    Debug("\n\n\t Host Url : ");
    Debugln(DEV_REG_host);
    
    Serial.println("\n\n\t\t***************DEVICE REGISTRATION***************");

    createJson();
    devRegPostData();
    manageSaveConfig();
    //devRegPostData(devRegPostPath,deviceName,deviceDescription,deviceMAC,deviceIP,deviceConfigParams); // CHANGE PARAMETERS AS REQ.

    Serial.printf("\n\nWeb server started. \n\nOpen %s in a web browser \nto enter configuration parameters \n\n\n", WiFi.localIP().toString().c_str());
    //FOTAsetup();
  }
/**/
} 

//deviceName,deviceDescription,deviceMAC,deviceIP,deviceConfigParams
//POST data to Server
//bool devRegPostData(String path, String devName, String devDescription, String devMAC, String devIP, String devConfigParams)
bool devRegPostData()
{
  HTTPClient httpPost;                    //Declare object of class HTTPClient
  
  Debug("\n\n\tHost Path : ");         Debugln(DEV_REG_host + devRegPostPath);
  
  httpPost.begin(DEV_REG_host + devRegPostPath);    //Specify request destination  
  httpPost.addHeader("Content-Type", "application/json");   //Specify content-type header

  int httpCode = httpPost.POST(JSONmessageBuffer);            //Send the request
  Serial.print("\n\n\thttpCode:\t");
  Serial.println(httpCode);   //Print HTTP return code
  

//    String responsePayload = httpPost.getString();              //Get the response payload  
//    Serial.println("\n\tResponse Payload:\t");
//    Serial.println(responsePayload);    //Print request response payload


//  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");  
//  int httpCode = httpPost.POST("deviceName="+deviceName+"&"+"deviceDescription="+ deviceDescription +"&"+
//                 "deviceMAC="+ devMAC +"&"+"deviceIP="+ devIP +"&"+"deviceConfigParams="+devConfigParams);

  String payload = httpPost.getString();
//  Serial.print("\n\n\tPayload: ");
//  Serial.println(payload);
         
//  httpPost.writeToStream(&Serial);
  httpPost.end();
  
//  Serial.println(httpCode);  

//  String payload = httpPost.getString();
  Serial.print("\n\n\tDeviceRegHttpPost Payload: ");
  Serial.println(payload);

  const char* jsonPayload = (char *)payload.c_str();
//  Serial.print("\n\nGot jsonPayload: ");
//  Serial.println(jsonPayload);
  
//  const char* ccuuid=payload.c_str();

  const size_t jsonBufferSize = JSON_OBJECT_SIZE(2) + 30; // approx 70-100 bytes
  StaticJsonBuffer<jsonBufferSize> jsonBuffer;
  JsonObject& uuid_parse = jsonBuffer.parseObject(jsonPayload); //cuuid);

    // Test if parsing succeeds.
  if (!uuid_parse.success()) {
    Serial.println("\nNew UUID not assigned. UUID parseObject() failed");
    return false;
  }
  
//  const char* cuuid = uuid_parse["uuid"];   
//  Serial.print("\n\nGot UUID: ");
//  Serial.println(cuuid);
//  euuid =  (char*) cuuid.c_str();
//  euuid = cuuid;

  
  String quuid = uuid_parse["uuid"];      // .as<String>();
  if(quuid.length())
  { Serial.println("\n[devRegPostData] New UUID received");
    Serial.print("\n\nGot UUID - quuid : ");
    Serial.println(quuid); 
    euuid=quuid;
    uuid_flag=true;
  }
           
  if(httpCode>0)  {Serial.println(); return true;  }
  else  {  return false;  }     
}

void manageSaveConfig()
{
      if(uuid_flag)
      {   
        Serial.println("clearing EEPROM.");
        clearConfig();
      
        Serial.print("Settings written ");
      
//        saveConfig()? uuid_flag=false; Serial.println("sucessfully.") : Serial.println("not succesfully!");;
        if(saveConfig()){ uuid_flag=false; Serial.println("sucessfully.");}
        else  {Serial.println("not succesfully!");}

        Serial.println("Restarting!"); 
        
        
        delay(1000);
        ESP.reset();
      }
}

//Make GET request
bool devRegGetData(String path)
{
  HTTPClient http;
  http.begin( DEV_REG_host + path);

  int httpCode = http.GET();

  delay(100);
  http.end();

  
  if(httpCode == HTTP_CODE_OK)
  {
    String payload = http.getString();
    Serial.print("\n\n\tPayload: ");
    Serial.println(payload);
    return true;
  }
  else
    return false;
}
