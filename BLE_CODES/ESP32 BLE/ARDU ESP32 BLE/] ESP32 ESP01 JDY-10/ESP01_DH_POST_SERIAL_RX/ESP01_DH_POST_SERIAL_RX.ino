
/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
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

const char* ssid = "cdac";          const char* password = "";

String tempC, pressure, alti, humidity, lux, deviceDescription, deviceMAC, deviceIP;
//String deviceName="", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;

void setup() 
{   Serial.begin(9600);       //  Serial.flush();   delay(1000);
//    while (!Serial) 
//    { ; // wait for serial port to connect. Needed for native USB port only
//    }
    Serial.println("\n\n*********************START UP CODE***********************\n\n");
    
    WiFi_setup();
    Serial.println("\n\n*********************START UP DONE***********************\n\n");
}

void loop() 
{   
    Serial_Read();   
    HTTP_POST_NOTIF();
    Serial.println("\n\n*********************NEXT LOOP***********************\n\n");
    delay(100); // (2500);
} 

void Serial_Read()
{   
  while (!Serial) 
    { ; // wait for serial port to connect. Needed for native USB port only
    }  
  
    String ReadString = Serial.readStringUntil('\n');  // C1589T29.24P941.31A616.92H63.01L115.07
//  String ReadString = "C1589T29.24P941.31A616.92H63.01L115.07"; /*DUMMY STRING FOR TESTING PURPOSE*/
//  String ReadString = "T24.94H53.01L515.07"; /*DUMMY STRING FOR TESTING PURPOSE*/
//  String ReadString = "Tc8:fd:19:4a:be:1dH0L515.07"; /*DUMMY STRING FOR TESTING PURPOSE*/
  
  //  ReadString=Serial.readString();
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

void WiFi_setup()
{
  Serial.print("\n\nMAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("\nConnecting to ");
  Serial.println(ssid); 
  
WiFi.begin(ssid, password);
//  wifiMulti.addAP(ssid, password);
  int c = 0;
  while (WiFi.status() != WL_CONNECTED && ((c++)<100)) 
  { delay(20); Serial.print("."); //  Serial.println("Connecting..");
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
  {Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");}  
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
