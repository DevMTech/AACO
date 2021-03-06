
// AutoConnectWithFeedbackLED

//RTC_DATA_ATTR char* ssid  = "hcdc-pc54";   RTC_DATA_ATTR char* password = "gLz7jl5s";
//RTC_DATA_ATTR char* ssid  = "cdac";   RTC_DATA_ATTR char* password = "";

/*
//#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

//for LED status
#include <Ticker.h>
Ticker ticker;

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 // ESP32 DOES NOT DEFINE LED_BUILTIN
#endif

//#define LED2 LED_BUILTIN

void tick() //toggle state
{ digitalWrite(LED2, !digitalRead(LED2)); // set pin to the opposite state
//  digitalWrite(REDLED, !digitalRead(REDLED)); // set pin to the opposite state
#ifdef BUZZER
  digitalWrite(BUZZER, !(digitalRead(BUZZER)));
#endif
}

//gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  SOFT_AP_SSID = myWiFiManager->getConfigPortalSSID();

  //if you used auto generated SSID, print it
  Serial.print("Connect to SSID to config WiFi : ");
  //Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println(SOFT_AP_SSID);

#ifdef OLED
  // WIFI_SSID_CONFIG_OLED();
#endif

#ifdef EPAPER2in13
  WIFI_SSID_CONFIG_EPAPER2in13();
#endif

#ifdef BUZZER
  pinMode(BUZZER, OUTPUT);   // BUZ ON
  //digitalWrite(BUZZER, HIGH); delay(50);
  //digitalWrite(BUZZER, !(digitalRead(BUZZER))); delay(500);
  //pinMode(BUZZER, INPUT); // BUZ OFF
#endif

  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void WiFiManagerSetup()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  //pinMode(LED2, OUTPUT); //set led pin as output
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.5, tick);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

//  if (bootCount <= 2)
//  { Serial.println("\n ----------- RESETTING/REMOVING SAVED WiFi CREDENTIALS --------- \n");
//    wm.resetSettings(); //reset settings - wipe credentials for testing
//  }


  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);

  bool res;

  // fetches ssid and pass and tries to automatically connect to
  // WiFi using saved credentials ...

  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap

  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  //res = wm.autoConnect(); // auto generated AP name from chipid

  //String APName = "AutoConnectAP"+ChipID;
  res = wm.autoConnect("AutoConnectAP123456", "pswd2021"); // password protected AP, password filed should be 8 char. or longer
  // then goes into a blocking loop awaiting configuration and will return success result

  if (!res)
  { Serial.println("Failed to connect and hit timeout");
//    RGB_LED_OFF();
//    digitalWrite(REDLED, LOW);    delay(500);
    ESP.restart(); // reset and try again, or maybe put it to deep sleep
    delay(1000);
  }
  else
  { Serial.println("Connected to the WiFi :"); // connected to the WiFi
    ticker.detach();
    pinMode(LED2, OUTPUT);
//    RGB_LED_OFF();
//    digitalWrite(GRNLED, LOW);    delay(500);
    
#ifdef EPAPER2in13
    EPAPER2in13_BLANK_SCREEN();
#endif

#ifdef BUZZER
    digitalWrite(BUZZER, LOW);
    //pinMode(BUZZER, INPUT); // BUZ OFF
#endif

//    RGB_LED_ON(); //    RGB_LED_OFF();
    
    Serial.println("\n POWERING OFF 3V3 BUS -> TURNING OFF SPI/I2C PERIPHERALS \n");
    digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
    // pinMode(PFET_3V3_BUS, INPUT); // TURN OFF 3V3 BUS
    delay(5);
    digitalWrite (LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF
  }
}
*/

void WiFi_OFF()
{ WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();
  delay(1);

  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi_Status = "CONNECTED";
    // Serial.println("\n================= WIFI_OFF : WiFi CONNECTED ==================\n");

    //pinMode (LED_BUILTIN, OUTPUT);
    //digitalWrite (LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF
  }
  else
  { WiFiOnDuration = millis() - WiFiOnDuration;

    WiFi_Status = "DISCONCTD";
    // Serial.println("\n================= WIFI_OFF : WiFi DISCONNECTED ==================\n");

    //pinMode (LED_BUILTIN, OUTPUT);
    digitalWrite (LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF
    //pinMode (LED_BUILTIN, INPUT);
  }

//  RGB_LED_OFF();
}


void WiFi_ON()
{
  deviceMAC = WiFi.macAddress();              //macToStr(mac);  //mac;
  Serial.print("\n\nMAC : "); // 84:0D:8E:C3:60:8C ESP32S
  Serial.println(deviceMAC); // (WiFi.macAddress());

  //  WiFi.forceSleepWake();
  // delay(1);
/*
  WiFi.mode(WIFI_STA);


  // Configures static IP address
  //  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) // if (!WiFi.config(local_IP, gateway, subnet))
  //  { Serial.println("STA Failed to configure");
  //  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);




  //Serial.println("Connecting Wifi...");
  WiFiConnAttemptDuration += millis(); // Keep track of when we started our attempt to get a WiFi connection

  int WiFiConnAttemptCount = 0;
  while (WiFi.status() != WL_CONNECTED && (WiFiConnAttemptCount < 100))
  { WiFiConnAttemptCount++;
    Serial.print(".");
    delay(50);
  }

  // if (bootCount<=1 && (WiFi.status() != WL_CONNECTED))
  if ((WiFi.status() != WL_CONNECTED))
  { WiFiConnRetryAttempt++;
    WiFiManagerSetup();
  }
  else
  { WiFiConnRetryAttempt = 0;
  }
*/

  WiFiManagerSetup();
  
  Serial.println("\n WiFi Conn Retry Attempt : " + String(WiFiConnRetryAttempt));

  if (WiFi.status() != WL_CONNECTED && (WiFiConnRetryAttempt >= 2))
  { Serial.println("\n=========================================== Restarting ESP32 ... in 100ms =========================================== \n\n");
    delay(100);
    WiFiConnRetryAttempt = 0;
    ESP.restart();
  }

  WiFiConnAttemptDuration = millis() - WiFiConnAttemptDuration;

  // Print local IP address and start web server
  Serial.flush();
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnection successful ...");
    //  Serial.println("\n\n");
    Serial.println("\nWiFi connected.");
    Serial.print("\nIP address: \t"); // Local
    Serial.println(WiFi.localIP());
    Serial.print("Gateway IP: \t");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask: \t");
    Serial.println(WiFi.subnetMask());
    Serial.print("DNS 1: \t\t");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2: \t\t");
    Serial.println(WiFi.dnsIP(1));
    Serial.println("");

    //    bool success = Ping.ping("172.217.166.36", 3); // ("www.google.com", 3);
    //    if (!success)
    //    { Serial.println("\nFailed to Ping www.google.com");   // return;
    //    }
    //    else
    //    { Serial.println("\nSuccessfully Pinged www.google.com");
    //    }
  }
  else
  {
    Serial.print("\n\nFailed to connect to ");
    Serial.println(ssid);
  }

  //  WiFi.begin(ssid, password);
  //  wifiMulti.addAP(ssid, password);
  //  wifiMulti.addAP(ssid0, password0);
  //  wifiMulti.addAP(ssid1, password1);

  // WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();

  // unsigned long startAttemptTime = millis(); // Keep track of when we started our attempt to get a WiFi connection
  //  // Keep looping while we're not connected AND haven't reached the timeout
  //  while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime < WIFI_TIMEOUT))
  //  { delay(10);
  //  }

  //  int8_t c = 0;
  //  Serial.println("Connecting Wifi...");
  //  WiFiConnAttemptDuration += millis(); // Keep track of when we started our attempt to get a WiFi connection
  //  while (WiFi.status() != WL_CONNECTED && c++ < 100)
  //  //while (wifiMulti.run() != WL_CONNECTED && c++ < 10)
  //  { delay(1); Serial.print("."); //  Serial.println("Connecting..");
  //  }
  //  Serial.println("Connection Attempts .." + String(c));
  //  WiFiConnAttemptDuration = millis() - WiFiConnAttemptDuration;



  if (WiFi.status() == WL_CONNECTED)
    //if(wifiMulti.run() == WL_CONNECTED)
  { WiFiOnDuration += millis();
    WiFi_Status = "CONNECTED";
    // Serial.println("\n================= WIFI_ON : WiFi CONNECTED ==================\n");
    // Serial.println("\nWiFi Connection to " + String(ssid) + " established!");

    //    Serial.print("\nConnected to ");    Serial.println(WiFi.SSID());
    //    Serial.print("\nIP address:\t");    Serial.println(WiFi.localIP());

    deviceDescription = "Antiquity Ambient Condition Observer";
    //    deviceMAC = WiFi.macAddress();              //macToStr(mac);  //mac;
    deviceIP = WiFi.localIP().toString().c_str();
    //Serial.println("\n");

    //  if((int)batteryLevelPercent < 20)

#ifdef BUZZER
    //#ifdef LED_BUILTIN
    //pinMode(BUZZER, OUTPUT);   // BUZ ON
    //pinMode (LED_BUILTIN, OUTPUT);
    //digitalWrite (LED_BUILTIN, HIGH); //HIGH = ON //LOW = OFF
    //    int i = 5;
    //    while(i-- > 0)
    //    {
    //      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //toggle the LED
    //      digitalWrite(BUZZER, !(digitalRead(BUZZER))); delay(500);
    //    }
    //    pinMode(BUZZER, INPUT); // BUZ OFF
    //    digitalWrite (LED_BUILTIN, HIGH); //HIGH = ON //LOW = OFF
#endif

  }
  else
  { WiFi_Status = "DISCONNCTD";
    Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");
    // Serial.println("\n================= WIFI_ON : WiFi DISCONNECTED ==================\n");

    //pinMode (LED_BUILTIN, OUTPUT);
    //digitalWrite (LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF
    //pinMode (LED_BUILTIN, INPUT);

    // SAVE DATA IN EEPROM & GO TO DEEP SLEEP
  }
}

void initialize()
{
  tempC = 22.5; humidity = 52;  lux = 560;  batteryLevel = 3.32;
  CO2 = 432.8;  pm_2point5 = 3.51;  pm_10 = 8.5;    // timeStamp = "2020-01-15T10:15:34.444Z";
  found_BLE_MAC_list = "d4:36:39:c2:28:3c, 24:0a:c4:83:20:c2";
}

/*

  //  OnDemandNonBlocking.ino
  //  example of running the webportal or configportal manually and non blocking
  //  trigger pin will start a webportal for 120 seconds then turn it off.
  //  startCP = true will start both the configportal AP and webportal

  #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

  // include MDNS
  #ifdef ESP8266
  #include <ESP8266mDNS.h>
  #elif defined(ESP32)
  #include <ESPmDNS.h>
  #endif

  // select which pin will trigger the configuration portal when set to LOW
  #define TRIGGER_PIN 0

  WiFiManager wm;

  unsigned int  timeout   = 120; // seconds to run for
  unsigned int  startTime = millis();
  bool portalRunning      = false;
  bool startAP            = false; // start AP and webserver if true, else start only webserver

  void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1000);
  Serial.println("\n Starting");

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // wm.resetSettings();
  wm.setHostname("MDNSEXAMPLE");
  // wm.setEnableConfigPortal(false);
  // wm.setConfigPortalBlocking(false);
  wm.autoConnect();
  }

  void loop() {
  #ifdef ESP8266
  MDNS.update();
  #endif
  doWiFiManager();
  // put your main code here, to run repeatedly:
  }

  void doWiFiManager(){
  // is auto timeout portal running
  if(portalRunning){
    wm.process(); // do processing

    // check for timeout
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout");
      portalRunning = false;
      if(startAP){
        wm.stopConfigPortal();
      }
      else{
        wm.stopWebPortal();
      }
   }
  }

  // is configuration portal requested?
  if(digitalRead(TRIGGER_PIN) == LOW && (!portalRunning)) {
    if(startAP){
      Serial.println("Button Pressed, Starting Config Portal");
      wm.setConfigPortalBlocking(false);
      wm.startConfigPortal();
    }
    else{
      Serial.println("Button Pressed, Starting Web Portal");
      wm.startWebPortal();
    }
    portalRunning = true;
    startTime = millis();
  }
  }


*/
