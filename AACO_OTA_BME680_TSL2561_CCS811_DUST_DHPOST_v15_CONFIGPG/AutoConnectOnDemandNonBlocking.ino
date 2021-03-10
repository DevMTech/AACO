
//   AutoConnect OnDemandNonBlocking.ino
//   example of running the webportal or configportal manually and non blocking
//   trigger pin will start a webportal for 120 seconds then turn it off.
//   startCP = true will start both the configportal AP and webportal

// WIFI CONFIG OPENS ON BOOT-UP(NO SAVED WIFI) / ON BUTTON PRESS (ON DEMAND)
// WIFI CONFIG OPENS IN  http://192.168.4.1/     http://10.208.35.252/
// NO PASSWORD REQ.
// NO CONFIG PARAM
// OTA UPDATE WORKS http://192.168.4.1/update?  http://10.208.35.252/update?


//#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// include MDNS
#ifdef ESP8266
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <ESPmDNS.h>
#endif

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 12 // 34

WiFiManager wm;

unsigned int  timeout   = 120; // seconds to run for
unsigned int  startTime = millis();
bool portalRunning      = false;
bool startAP            = false; // start AP and webserver if true, else start only webserver

void WiFiManagerSetup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once
  // Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1000);
  Serial.println("\n Starting");

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  //  if (bootCount <= 2)
  //  {
  //  Serial.println("\n ----------- RESETTING/REMOVING SAVED WiFi CREDENTIALS --------- \n");
  //  wm.resetSettings(); //reset settings - wipe credentials for testing
  //  }


  wm.setHostname("MDNSEXAMPLE");
  // wm.setEnableConfigPortal(false);
  // wm.setConfigPortalBlocking(false);
  // wm.autoConnect();
  wm.autoConnect("AutoConnectAP123456", "pswd2031");
}

void WiFiManagerLoop() {
  //  #ifdef ESP8266
  //  MDNS.update();
  //  #endif
  Serial.println("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ INSIDE onDemandWiFiManager() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  \n");
  unsigned long onDemandWiFiManagerDelay = millis();
  while (millis() - onDemandWiFiManagerDelay < 30000) // 30sec
  {
    doWiFiManager();
  }
  // put your main code here, to run repeatedly:
}

void doWiFiManager() {
  // is auto timeout portal running
  if (portalRunning) {
    wm.process(); // do processing

    // check for timeout
    if ((millis() - startTime) > (timeout * 1000)) {
      Serial.println("portaltimeout");
      portalRunning = false;
      if (startAP) {
        wm.stopConfigPortal();
      }
      else {
        wm.stopWebPortal();
      }
    }
  }

  // is configuration portal requested?
  if (digitalRead(TRIGGER_PIN) == LOW && (!portalRunning)) {
    if (startAP) {
      Serial.println("Button Pressed, Starting Config Portal");
      wm.setConfigPortalBlocking(false);
      wm.startConfigPortal();
    }
    else {
      Serial.println("Button Pressed, Starting Web Portal");
      wm.startWebPortal();
    }
    portalRunning = true;
    startTime = millis();
  }
}

/**/
