/**
 * 
 * HM10 --- SOFT SERIAL COMM. --- ESP8266 ))))) DEVICE HIVE
 * 
 * HM10   ESP8266[NODEMCU 12E] 
 *  VCC     3V3
 *  GND     GND
 *  TX      RX (14 = D5)
 *  RX      TX (12 = D6)
 */

#if defined(ESP8266)              // ESP12E - ESP8266
      #include <ESP8266WiFi.h>    
      #include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
      #include <ESP8266HTTPClient.h>
      //#include <WiFiClientSecure.h>
      ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

#else                             // ESP32
      #include <WiFi.h>           
      #include <WiFiMulti.h>
      #include <HTTPClient.h>
      WiFiMulti wifiMulti;
#endif

#include <String.h>
#include <SoftwareSerial.h>
#define TX_DELAY_MS 50 //500

//SoftwareSerial HM10_Serial(14, 12, false, 256); // 14=D5(RX), 12=D6(TX) ESP8266[NODEMCU12E]
// SoftwareSerial(rxPin, txPin, inverse_logic, buffer size); 
//https://circuits4you.com/2016/12/16/esp8266-software-serial/

// [ESP01] GPIO2(RX)=HM10_TXD, GPIO0(TX)=HM10_RXD
SoftwareSerial HM10_Serial(2, 0, false, 256);  // [ESP01]

//SoftwareSerial HM10_Serial(2, 3); // RX, TX (PRO MINI, ARDUINO)

String WriteString, ReadString;

const char* ssid_STA = "cdac";          const char* password_STA = "";
//const char* ssid_STA = "ASUS_X00TD";        const char* password_STA = "4652b298f";
//const char* ssid_AP = "ESP01_AP";          const char* password_AP = "";

String tempC, humidity, lux, pressure, alti, deviceDescription, deviceMAC, deviceIP;
//String deviceName="", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;

void setup() 
{   Serial.begin(9600);       //  Serial.flush();   
    delay(100);

    Serial.println("\n\n*********************START UP CODE***********************\n\n");
    
//    WiFi_AP_setup();
    WiFi_STA_setup();
    HM10_setup();
//    WEBSERVER_setup();
    
    Serial.println("\n\n*********************START UP DONE***********************\n\n");
//    delay(500);
}

void loop() 
{   
    HM10_Serial_Read();   
//    WEB_loop();
    HTTP_POST_NOTIF();
    Serial.println("\n\n*********************NEXT LOOP***********************\n\n");
    delay(2000);
} 



void WiFi_STA_setup()
{
  Serial.print("\n\nMAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("\nConnecting to ");
  Serial.println(ssid_STA); 
  
//WiFi.begin(ssid, password);
  WiFi.begin(ssid_STA, password_STA);
//  wifiMulti.addAP(ssid_STA, password_STA);

  int8_t c = 0;
  while (WiFi.status() != WL_CONNECTED && ((c++)<100)) 
  { delay(10); Serial.print("."); //  Serial.println("Connecting..");
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi Connection to "+ String(ssid_STA)+" established!"); 
    Serial.print("\nIP address:\t");
    Serial.println(WiFi.localIP()); // Print the IP address on serial monitor 
  }  
  else
  {Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");
  }
    
//    Serial.print("Use this URL to connect: ");
//    Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
//    Serial.print(WiFi.localIP()); // Print the IP address on serial monitor
//    Serial.println("/");

//    deviceDescription="Antiquity Ambient Condition Observer";
//    deviceMAC=WiFi.macAddress();                //macToStr(mac);  //mac;
//    deviceIP=WiFi.localIP().toString().c_str();
//    Serial.println("\n");

}

/*

void WiFi_AP_setup()
{
  WiFi.softAP(ssid_AP, password_AP);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid_AP);
  Serial.println("\" started");

//  Serial.print("IP address:\t");

  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  Serial.println("/");
  
}
*/

/********************** DUMMY DATA FOR TESTING ****************************/
//     tempC="29.24";
//     pressure="941.31";
//     alti="616.92";
//     humidity="63.01";
//     lux="115.07";

//     tempC="29.24 *C";
//     pressure="941.31 mPa";
//     alti="616.92 m";
//     humidity="63.01 %";
//     lux="115.07 lux";
/********************** DUMMY DATA FOR TESTING ****************************/
