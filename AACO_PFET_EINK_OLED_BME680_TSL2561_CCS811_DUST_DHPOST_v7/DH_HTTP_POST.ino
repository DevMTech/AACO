/******************************************CONFIGURE THESE PARAMETERS***************************************************/
String URL;

String DH_DEVICE_ID = "wfh-aaco01"; //  DEVICE ID

String accessTokenS = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjQ4NzUxNDAwMDAwLCJ0IjoxLCJ1IjoxMDc0LCJuIjpbIjEwNzIiXSwiZHQiOlsiKiJdfX0.fFjhKYFl4Wh9rXHCJXLQYjmzRLVl2adK2_2RX3rBYAc";


/*
Authorization : Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjQ4NzUxNDAwMDAwLCJ0IjoxLCJ1IjoxMDc0LCJuIjpbIjEwNzIiXSwiZHQiOlsiKiJdfX0.fFjhKYFl4Wh9rXHCJXLQYjmzRLVl2adK2_2RX3rBYAc
DeviceName    : wfh-aaco01
networkId     : 1072
cloud-URL     : https://playground.devicehive.com/api/rest
*/

/*
String DH_SERVER_IP = "10.42.0.1"; // "10.208.34.242"; // "192.168.122.1"; // "10.42.0.1"; //  "10.208.37.221"; // "10.208.35.79"; // ifconfig

String DH_DEVICE_ID = "device2"; //  DEVICE ID

String accessTokenExpDate31_12_2030 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTkyNDk1OTAwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.3ymgwtMP1phpRep52U_j1fJ-4KlMReqGEkLqaGpygfY";
String accessTokenS = accessTokenExpDate31_12_2030;

//String accessTokenExpDate31_3_2020 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTU4NTU5MzAwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.-tbtkjeVlI3XvFKxJjf7uu2LdxvNGqc7TpZqM8NmLik";
//String accessTokenS = accessTokenExpDate31_3_2020;
*/

/******************************************CONFIGURE THESE PARAMETERS***************************************************/




void HTTP_POST_NOTIF()
{
  HTTP_post_status = "FAILURE";
  
  Serial.println("\n\nDH_DEVICE_ID :" + DH_DEVICE_ID); //String());
  // Serial.println("DH_SERVER_IP :" + DH_SERVER_IP);
  
    //int httpTime = millis();

    HTTPClient http;

    // Serial.print("\n[HTTP] begin...\n");

    //URL = "http://" + DH_SERVER_IP + ":80/api/rest/device/" + DH_DEVICE_ID + "/notification"; //HTTP DEVICE NAME: ESP_AFMS_01
    URL = "https://playground.devicehive.com/api/rest/device/" + DH_DEVICE_ID + "/notification"; 
    
    Serial.println("DH_DEVICE_URL :" + URL) ;
    
    String TokenBearer = "Bearer" + accessTokenS;
    http.begin(URL); //HTTP USER NAME: Dev
    http.addHeader("Content-Type" , "application/json");
    http.addHeader("Accept" , "application/json");
    http.addHeader("Authorization" , TokenBearer);

    // Serial.print("[HTTP] POST...\n");

    //        tempC = 22.5; humidity = 50;  lux = 520;  CO2 = 432;  pm_2point5 = 351;  batteryLevel = 3.32; timeStamp = "2020-01-15T10:15:34.444Z";

    //        TimeNow(); // delay(500);

    // start connection and send HTTP header
    String data = "{\"notification\": \"sensordata\"";
    data += ",\"timestamp\":\""; data += timeStamp; // "\"2019-09-11T11:15:34.444Z\""
    data += "\",\"parameters\": {";
    data += "\"temp\":\"";  data += tempC;
    data += "\",\"humi\":\""; data += humidity;
    data += "\",\"lux\":\""; data += lux;
    data += "\",\"C\":\""; data += eCO2;
    data += "\",\"D\":\""; data += pm_10; // pm_2point5;
    data += "\",\"B\":\""; data += batteryLevel; // volt    
    data += "\",\"N\":\""; data += (bootCount++); // (count++);
    data += "\"}}";
    
    //        data += "\"T\":\"";  data += tempC;      //
    //        data += "\",\"H\":\""; data += humidity;
    //        data += "\",\"L\":\""; data += lux;
    //        data += "\",\"C\":\""; data += CO2;
    //        data += "\",\"G\":\""; data += GPS;
    

    //        String data = "{\"notification\": \"sensordata\",\"timestamp\": \"2020-01-14T18:25:34\",\"parameters\": {\"temp\":\"22.5\",\"humi\":\"40\",\"lux\":\"500\"}}"; // AACO

    //        String data = "{\"notification\": \"sensordata\",\"timestamp\": \"2020-01-14T18:25:34Z\",\"parameters\": {\"T\":\"22.5\",\"H\":\"40\",\"L\":\"500\",\"C\":\"400\",\"D\":\"350\",\"B\":\"3.3\"}}"; // ,\"H\":\"40\"  // AACO
    //  '{"T":'+String(tempC)+',"H":'+String(humidity)+',"L":'+String(lux)+',"C":'+String(CO2)+',"D":'+String(pm_2point5)+',"B":'+String(batteryLevel)+',"TS":'+String(timeStamp)'}'

    //        String data = "{\"notification\": \"sensordata\",\"timestamp\": \"2019-09-18T10:25:34.444Z\",\"parameters\": {\"50:8c:b1:40:16:47\":\"1\", \"98:5d:ad:23:14:bd\":\"0\"}}"; // BLE
    // {"50:8c:b1:40:16:47":"1", "98:5d:ad:23:14:bd":"0"}

    //httpTime = millis() - httpTime;
    //Serial.println("\nhttpTime: "+String(httpTime));
    
    Serial.print("\n[HTTP] POST DATA: \t");
    Serial.println(data);

  WiFi_setup();

  //if (WiFi.status() == WL_CONNECTED)
  if((wifiMulti.run() == WL_CONNECTED))  // wait for WiFi connection
  { WiFi_Status = "CONNECTED";
    // Serial.println("\n=======================CONNECTED TO WiFi==================\n");

    Serial.print("\nIP address:\t");    Serial.println(WiFi.localIP());
    Serial.print("\nConnected to ");    Serial.println(WiFi.SSID());
    
    int count = 1;
    httpCode = 0;
    
    while(httpCode != 201 && count++ <= 5)
    { httpCode =  http.POST(data);
      Serial.println("http post attempt: " + String(count));
      count++;
    }
              
    if (httpCode > 0) // httpCode will be negative on error
    { Serial.printf("[HTTP] Response Code: %d\n", httpCode); // HTTP header has been send and Server response header has been handled
      HTTP_post_status = "SUCCESS";
      
      if (httpCode == HTTP_CODE_OK) // file found at server
      { Serial.println("HTTP_CODE_OK");        
      }    
      String payload = http.getString();
      Serial.println(payload);
    }
    else
    { Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      String payload = http.getString();
      Serial.println(payload);
      HTTP_post_status = "FAILURE";
    }
    http.end();
  }
  else
  { Serial.println("\n==========NOT CONNECTED TO WiFi - TRYING TO CONNECT==================\n");
    WiFi_Status = "DISCONNECTED";
    WiFi_ON(); // WiFi_setup();
  }
}
