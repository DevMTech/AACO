/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *  
 *  MAC : 84:0D:8E:B1:27:3A

    Connecting to ASUS_X00TD
    ..............................................
    WiFi Connection to ASUS_X00TD established!
    Use this URL to connect: http://192.168.43.211/

 *
 */

#include <ESP8266WiFi.h> 
//#include <ESP8266HTTPClient.h>
//#include <ESP8266WebServer.h>
//#include <EEPROM.h>

//#if defined(ESP8266)              // ESP12E - ESP8266
//      #include <ESP8266WiFi.h>    
//      #include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
//      #include <ESP8266HTTPClient.h>
//      //#include <WiFiClientSecure.h>
//      ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
//
//#else                             // ESP32
//      #include <WiFi.h>           
//      #include <WiFiMulti.h>
//      #include <HTTPClient.h>
//      WiFiMulti wifiMulti;
//#endif


//const char* ssid_STA = "cdac";          const char* password_STA = "";
const char* ssid_STA = "ASUS_X00TD";        const char* password_STA = "4652b298f"; // http://192.168.43.211/

const char* ssid_AP = "ESP01_AP";          const char* password_AP = "";

String tempC, humidity, lux, pressure, alti, deviceDescription, deviceMAC, deviceIP;
//String deviceName="", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;

WiFiServer server(80);

void setup() 
{   Serial.begin(9600);       //  Serial.flush();   delay(1000);
//    while (!Serial) 
//    { ; // wait for serial port to connect. Needed for native USB port only
//    }
    Serial.println("\n\n*********************START UP CODE***********************\n\n");
    Serial.println("Disconnecting previously connected WiFi");
    WiFi.disconnect();
//    EEPROM.begin(512); //Initialasing EEPROM
    delay(10);
    
    WiFi_AP_setup();
//    WiFi_STA_setup();

    WEBSERVER_setup();
    
    Serial.println("\n\n*********************START UP DONE***********************\n\n");
}

void loop() 
{   
    Serial_Read();   
    WEB_loop();
    Serial.println("\n\n*********************NEXT LOOP***********************\n\n");
    delay(5000);
} 

void Serial_Read()
{   
//  while (!Serial) 
//    { ; // wait for serial port to connect. Needed for native USB port only
//    }  
  
    static uint8_t count = 0;
    String ReadString = "T"+String(count++)+"H53.01L515.07"; /*DUMMY STRING FOR TESTING PURPOSE*/
    
//  String ReadString = "C1589T29.24P941.31A616.92H63.01L115.07"; /*DUMMY STRING FOR TESTING PURPOSE*/
//  String ReadString = "T24.94H53.01L515.07"; /*DUMMY STRING FOR TESTING PURPOSE*/

//    String ReadString = Serial.readStringUntil('\n');  // C1589T29.24P941.31A616.92H63.01L115.07
//    String  ReadString=Serial.readString();
  
  int T, P, A, H, L;
  int Str_size = ReadString.length();
  if(Str_size)
  {
    Serial.println("\nRead String : "+String(ReadString)+"\n"); 
  
    T=ReadString.indexOf('T');
//    P=ReadString.indexOf('P');
//    A=ReadString.indexOf('A');
    H=ReadString.indexOf('H');
    L=ReadString.indexOf('L');

//    tempC=ReadString.substring(T+1,P);
//    pressure=ReadString.substring(P+1,A);
//    alti=ReadString.substring(A+1,H);
//    humidity=ReadString.substring(H+1,L);
//    lux=ReadString.substring(L+1,Str_size);
    
    tempC=ReadString.substring(T+1,H);
    humidity=ReadString.substring(H+1,L);
    lux=ReadString.substring(L+1,Str_size);
  }
}

void WiFi_AP_setup()
{
  WiFi.softAP(ssid_AP, password_AP);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid_AP);
  Serial.println("\" started");

//  Serial.print("IP address:\t");

  Serial.print("\nUse this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  Serial.println("/");
  
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
  int c = 0;
  while (WiFi.status() != WL_CONNECTED && ((c++)<100)) 
  { delay(10); Serial.print("."); //  Serial.println("Connecting..");
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi Connection to "+ String(ssid_STA)+" established!");  
  }  

    // Print the IP address on serial monitor
    Serial.print("\nUse this URL to connect: ");
    Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
    Serial.print(WiFi.localIP());
    Serial.println("/");

//    deviceDescription="Antiquity Ambient Condition Observer";
//    deviceMAC=WiFi.macAddress();                //macToStr(mac);  //mac;
//    deviceIP=WiFi.localIP().toString().c_str();
    Serial.println("\n");
}

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
