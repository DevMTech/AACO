//String DH_SERVER_IP = "192.168.122.1"; // "10.42.0.1"; // "10.208.35.79"; // "10.208.37.221"; //ifconfig
//String DH_DEVICE_ID = "device1"; // "123"; //  CORRECT ID 
//String accessTokenExpDate12_10_19 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTU2OTkwNDIwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.mNJP0Ajqz-lGdGaPU_QxGLAS1LRcvBIdtn-Q3LJRxuk";
//String accessTokenS = accessTokenExpDate12_10_19; 

// ESP_AFMS_01
//String DH_DEVICE_ID = "WmpZLPaEYEZruXHSBSbajvlTHstEWqPFXjFd"; // CORRECT ID
//String accessTokenExpDate30_11_19 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNTc1MDUyMjAwMDAwLCJ0IjoxLCJ1IjozLCJuIjpbIjIiXSwiZHQiOlsiOSJdfX0.g5gFdZmFKhHTuewyo-8UPLGCbDdIPDyQzDovtJAzq3A";
//String accessTokenS = accessTokenExpDate30_11_19;         
//String refreshTokenExpDate30_11_19 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNTc1MDUyMjAwMDAwLCJ0IjowLCJ1IjozLCJuIjpbIjIiXSwiZHQiOlsiOSJdfX0.HHH0k__hojNgWwBB4wFajy1gC_RZ7iEtF-d1iubwcvk"; 


// ESP32-AACO-01
// 31/12/2029
// 7WSIoIabwyDKyfCXK1q7FkbIQcBIR76coQoQ
// eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTg5MzM0OTgwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.sItE0U1EkkIKikvr2bjwnG6u09TfiE2T3R0lLSiZ2fE
// eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTg5MzM0OTgwMDAwMCwidCI6MCwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.tX0b1fBHAxSwDxV33M6JEf6RyBKA6qPfY0_0JJyK2Uo                       
///  


/*
#include <WiFi.h>           
#include <HTTPClient.h>
      
String URL; 

void WiFi_setup()
{
  Serial.print("\n\nMAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("\nConnecting to ");
  Serial.println(ssid); 
  
  WiFi.begin(ssid, password);
//  wifiMulti.addAP(ssid, password);
  
  int8_t c=0;
  while (WiFi.status() != WL_CONNECTED && c++<100) 
  { delay(50); //Serial.print("."); //  Serial.println("Connecting..");
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi Connection to "+ String(ssid)+" established!");  
    Serial.print("\nIP address:\t");
    Serial.println(WiFi.localIP());  

    deviceDescription="Antiquity Ambient Condition Observer";
    deviceMAC=WiFi.macAddress();                //macToStr(mac);  //mac;
    deviceIP=WiFi.localIP().toString().c_str();
    Serial.println("\n");
  }
  else
  {Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");
  }    
}

void HTTP_POST_NOTIF() 
{       Serial.println("DH_SERVER_IP :"+DH_SERVER_IP);
        Serial.println("DH_DEVICE_ID :"+DH_DEVICE_ID) ; //String());
//    if((wifiMulti.run() == WL_CONNECTED))  // wait for WiFi connection
    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\n=======================CONNECTED TO WiFi==================\n");

        Serial.print("\nIP address:\t");
        Serial.println(WiFi.localIP()); 
        
        HTTPClient http;

        Serial.print("\n[HTTP] begin...\n");
        
        URL = "http://"+DH_SERVER_IP+":80/api/rest/device/"+DH_DEVICE_ID+"/notification"; //HTTP DEVICE NAME: ESP_AFMS_01
        String TokenBearer = "Bearer"+ accessTokenS;
        http.begin(URL); //HTTP USER NAME: Dev
        http.addHeader("Content-Type" , "application/json");
        http.addHeader("Accept" , "application/json");        
        http.addHeader("Authorization" , TokenBearer);
        
        Serial.print("[HTTP] POST...\n");
        
        // start connection and send HTTP header        
        String data = "{\"notification\": \"sensordata\",\"timestamp\": \"2019-09-11T11:15:34.444Z\",\"parameters\": {\"temp\":";  //+ tempC+ ",\"humi\":"+ humidity+ ",\"lux\":"+ lux+ "}}";
        data += tempC;      // 
        data += ",\"humi\":"; data += humidity;  
        data += ",\"volt\":"; data += volt;
        data += ",\"lux\":"; data += lux; data += "}}";

//        String data = "{\"notification\": \"sensordata\",\"timestamp\": \"2019-09-18T10:25:34.444Z\",\"parameters\": {\"temp\":\"50:8c:b1:40:16:47\",\"humi\":\"1\",\"lux\":\"500\"}}";

//        String data = "{\"notification\": \"sensordata\",\"timestamp\": \"2019-09-18T10:25:34.444Z\",\"parameters\": {\"50:8c:b1:40:16:47\":\"1\", \"98:5d:ad:23:14:bd\":\"0\"}}";
        // {"50:8c:b1:40:16:47":"1", "98:5d:ad:23:14:bd":"0"}
        
        Serial.print("\n[HTTP] POST DATA: \t");
        Serial.println(data);
        int httpCode =  http.POST(data);    
                
        if(httpCode > 0) // httpCode will be negative on error
        {            
            Serial.printf("[HTTP] Response Code: %d\n", httpCode); // HTTP header has been send and Server response header has been handled
            
            if(httpCode == HTTP_CODE_OK)  // file found at server
            {
                Serial.println("HTTP_CODE_OK");
            }

            String payload = http.getString();
            Serial.println(payload);
        } 
        else 
        {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
            String payload = http.getString();
            Serial.println(payload);
        }
        http.end();
    }
    else
    {Serial.println("\n==========NOT CONNECTED TO WiFi - TRYING TO CONNECT==================\n");
      WiFi_setup();
    }    
} 
*/
// ESP32-AACO-01
// 31/12/2029
// 7WSIoIabwyDKyfCXK1q7FkbIQcBIR76coQoQ
// eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTg5MzM0OTgwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.sItE0U1EkkIKikvr2bjwnG6u09TfiE2T3R0lLSiZ2fE
// eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTg5MzM0OTgwMDAwMCwidCI6MCwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.tX0b1fBHAxSwDxV33M6JEf6RyBKA6qPfY0_0JJyK2Uo                       


//        http.begin("http://10.208.43.120:80/api/rest/device/WmpZLPaEYEZruXHSBSbajvlTHstEWqPFXjFd/notification"); //HTTP
//        http.addHeader("Accept" , "application/json");
//        http.addHeader("Content-Type" , "application/json");
//        http.addHeader("Authorization" , "Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNTYxODMzMDAwMDAwLCJ0IjoxLCJ1IjozLCJuIjpbIjIiXSwiZHQiOlsiOSJdfX0.bdjxON3Z2UXdS-bshp4luLLG6gDkLBbK5AxiQf_KR68");


        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS // configure traged server and url       
//        http.begin("https://playground.devicehive.com:443/api/rest/device/0hnFrTEntuF4WPyDuu3bIQEmrWqOmcowu18F/notification"); //HTTP
//http.POST("{\"notification\": \"sensordata\",\"timestamp\": \"2019-06-19T05:59:34.444Z\",\"parameters\": {\"temp\":50,\"humi\":20}}");
