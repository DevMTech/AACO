
/******************************************CONFIGURE THESE PARAMETERS***************************************************/

String accessTokenS = "Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTkyNDg4NTgwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.uNqldVfNzxl3aL_kb9UvxmUHjLMsYpbRt7w3so0O3ig";
String DH_DEVICE_ID = "b-9-2"; // "AACO163"; // "device2"; //  DEVICE ID

String DH_SERVER_IP = "10.208.34.163"; // "10.42.0.1"; // "10.208.34.242"; // "192.168.122.1"; // "10.42.0.1"; //  "10.208.37.221"; // "10.208.35.79"; // ifconfig
String URL = "http://" + DH_SERVER_IP + ":80/api/rest/device/" + DH_DEVICE_ID + "/notification"; //HTTP DEVICE NAME: ESP_AFMS_01

/******************************************CONFIGURE THESE PARAMETERS***************************************************/




void HTTP_POST_NOTIF()
{
  Serial.println("\n*************************************************************************************************");
  
  HTTP_post_status = "FAILURE";

  Serial.println("\n\nDH_DEVICE_ID :" + DH_DEVICE_ID); //String());
  // Serial.println("DH_SERVER_IP :" + DH_SERVER_IP);

  //int httpTime = millis();

  HTTPClient http;

  // Serial.print("\n[HTTP] begin...\n");

  // URL = "http://" + DH_SERVER_IP + ":80/api/rest/device/" + DH_DEVICE_ID + "/notification"; //HTTP DEVICE NAME: ESP_AFMS_01
  // URL = "https://playground.devicehive.com/api/rest/device/" + DH_DEVICE_ID + "/notification";

  Serial.println("DH_DEVICE_URL :" + URL) ;

  String TokenBearer = accessTokenS; // "Bearer " +
  http.begin(URL); //HTTP USER NAME: Dev
  http.addHeader("Content-Type" , "application/json");
  http.addHeader("Accept" , "application/json");
  http.addHeader("Authorization" , TokenBearer);

  // Serial.print("[HTTP] POST...\n");

  // TimeNow(); // delay(500);

  // tempC = 22.5; humidity = 52;  lux = 560;  batteryLevel = 3.32;
  // CO2 = 432;  pm_2point5 = 3.51;  pm_10 = 8.5;    // timeStamp = "2020-01-15T10:15:34.444Z";

  // start connection and send HTTP header
//  String data = "{\"notification\": \"sensordata\"";
//  data += ",\"timestamp\":\""; // data += timeStamp; // "\"2019-09-11T11:15:34.444Z\""
//  data += "\",\"parameters\": {";
//  data += "\"temp\":\"";  data += tempC;
//  data += "\",\"humi\":\""; data += humidity;
//  data += "\",\"lux\":\""; data += lux;
//  data += "\",\"C\":\""; data += CO2;
//  data += "\",\"D\":\""; data += pm_2point5; // pm_10; //
//  data += "\",\"D10\":\""; data += pm_10; //
//  data += "\",\"B\":\""; data += batteryLevel; // volt
//  data += "\",\"N\":\""; data += (bootCount++); // (count++);
//  data += "\"}}";

  //        data += "\"T\":\"";  data += tempC;      //
  //        data += "\",\"H\":\""; data += humidity;
  //        data += "\",\"L\":\""; data += lux;
  //        data += "\",\"C\":\""; data += CO2;
  //        data += "\",\"G\":\""; data += GPS;

  String data = "{\"notification\": \"sensordata\"";
  data += ",\"timestamp\":\""; // data += timeStamp; // "\"2019-09-11T11:15:34.444Z\""
  data += "\",\"parameters\": {";
  data += "\"BLE_MAC\":\"";  data += found_BLE_MAC_list;
  data += "\"}}";
  
  // 2020-08-03T18:25:34
  //String data = "{\"notification\": \"sensordata\",\"timestamp\": \"\",\"parameters\": {\"temp\":\"22.5\",\"humi\":\"40\",\"lux\":\"500\",\"C\":\"400\",\"D\":\"6.10\",\"D10\":\"10.40\",\"B\":\"4.3\",\"N\":\"5\"}}"; // AACO



  // String data = {"notification": "sensordata","timestamp":"","parameters": {"temp":"30.22","humi":"67.11","lux":"15.08","C":"401","D":"6.00","D10":"15.60","B":"4.36","N":"51"}};

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

  //  if((wifiMulti.run() == WL_CONNECTED))  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED)
  { WiFi_Status = "CONNECTED";
    // Serial.println("\n=======================CONNECTED TO WiFi==================\n");

    Serial.print("\n[HTTP-POST] IP address:\t");    Serial.println(WiFi.localIP());
    Serial.print("[HTTP-POST] Connected to ");    Serial.println(WiFi.SSID());

    int count = 1;    httpCode = 0;

    while (httpCode != 201 && count <= 5)
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

   Serial.println("\n*************************************************************************************************");
}

//String DH_DEVICE_ID = "wfh-aaco01"; //  DEVICE ID
//String accessTokenS = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjQ4NzUxNDAwMDAwLCJ0IjoxLCJ1IjoxMDc0LCJuIjpbIjEwNzIiXSwiZHQiOlsiKiJdfX0.fFjhKYFl4Wh9rXHCJXLQYjmzRLVl2adK2_2RX3rBYAc";
//String URL = "https://playground.devicehive.com/api/rest/device/" + DH_DEVICE_ID + "/notification";
/*
  Authorization : Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTIsMTUsMTYsMTddLCJlIjoxNjQ4NzUxNDAwMDAwLCJ0IjoxLCJ1IjoxMDc0LCJuIjpbIjEwNzIiXSwiZHQiOlsiKiJdfX0.fFjhKYFl4Wh9rXHCJXLQYjmzRLVl2adK2_2RX3rBYAc
  DeviceName    : wfh-aaco01
  networkId     : 1072
  cloud-URL     : https://playground.devicehive.com/api/rest
*/

//String accessTokenS = "Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTYwOTM1MzAwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.J5ivLupuJ8ZggldOmoprFuN6z_SKOgY4m-jYzWxHQeQ";
//String DH_DEVICE_ID = "a-9-3"; // "AACO163"; // "device2"; //  DEVICE ID



//{
//  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNTk2NDQ2NjkyODMyLCJ0IjoxLCJ1IjozLCJuIjpbIjEiLCIyIl0sImR0IjpbIioiXX19.ZdHiO2oxoOLZn_H4d4_01piHTr5VHEJd9vh4KnulJ2M",
//  "refreshToken": "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNjEyMTY5NjkyODMyLCJ0IjowLCJ1IjozLCJuIjpbIjEiLCIyIl0sImR0IjpbIioiXX19.JHL0nHh4DCl4w6e49_BO80haa4HbGcFh7vl599BsLHs"
//}

//String accessTokenS = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTYyNzY2OTgwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.4WGrTGvrCN_mCaZ_appkBsHw_uPfzpFqYHUlica-lMs";
// eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNjMwNDIxNDAwMDAwLCJ0IjoxLCJ1IjozLCJuIjpbIjEiLCIyIl0sImR0IjpbIioiXX19.biLhMhe8vUE9Vro_J_zRKoxUTLOLsTLCX6sMZr9S5UE
// eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNjMwNDIxNDAwMDAwLCJ0IjowLCJ1IjozLCJuIjpbIjEiLCIyIl0sImR0IjpbIioiXX19.vrCISIgkhMwcZQyim35LjtJdjpoofDHrCt58QmcyHRY

// expiration: 31-8-2021 20:20
//String accessTokenS = "Bearer eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxNjMwNDIxNDAwMDAwLCJ0IjoxLCJ1IjozLCJuIjpbIjEiLCIyIl0sImR0IjpbIioiXX19.biLhMhe8vUE9Vro_J_zRKoxUTLOLsTLCX6sMZr9S5UE";
//String DH_DEVICE_ID = "8CM74F79oVNGuLRzvrGFTo5MFdEK3FUsBz36"; // "AACO163"; // "device2"; //  DEVICE ID

// expiration: 31-12-2030 20:20
// String accessTokenS = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTU5NTkyNzQ5MDQ3NywidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.Uz6Q3KxitRmas0xiHFN3qvb4Phnvg4OLhELgx9OrFfs";

//String ACCESSTokenExpDate31_12_2030 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxOTI0OTU3ODAwMDAwLCJ0IjoxLCJ1IjozLCJuIjpbIjEiXSwiZHQiOlsiKiJdfX0.N5xQW_W3ZSkgdHEOCKXCNVftDtLQg4QWGdjtG6UW3LI";
//String REFRESHTokenExpDate31_12_2030 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlsyLDMsNCw1LDYsNyw4LDksMTAsMTEsMTUsMTYsMTddLCJlIjoxOTI0OTU3ODAwMDAwLCJ0IjowLCJ1IjozLCJuIjpbIjEiXSwiZHQiOlsiKiJdfX0.ywuU_vFiNRTXng9WuvLbEd-AWI3ZiUIxYJtwuZPCTrA";
//String accessTokenS = ACCESSTokenExpDate31_12_2030;

//String accessTokenExpDate31_12_2030 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTkyNDk1OTAwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.3ymgwtMP1phpRep52U_j1fJ-4KlMReqGEkLqaGpygfY";
//String accessTokenS = accessTokenExpDate31_12_2030;

//String accessTokenExpDate31_3_2020 = "eyJhbGciOiJIUzI1NiJ9.eyJwYXlsb2FkIjp7ImEiOlswXSwiZSI6MTU4NTU5MzAwMDAwMCwidCI6MSwidSI6MSwibiI6WyIqIl0sImR0IjpbIioiXX19.-tbtkjeVlI3XvFKxJjf7uu2LdxvNGqc7TpZqM8NmLik";
//String accessTokenS = accessTokenExpDate31_3_2020;
