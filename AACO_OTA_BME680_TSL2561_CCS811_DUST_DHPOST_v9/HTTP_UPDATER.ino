/**
   httpUpdate.ino

*/

//#include <Arduino.h>
//#include <WiFi.h>
//#include <HTTPClient.h>
//#include <HTTPUpdate.h>

void OTA_HTTP_UPDATER() 
{
  // wait for WiFi connection
//  if ((WiFiMulti.run() == WL_CONNECTED)) 
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\n\t inside OTA_HTTP_UPDATER() - WiFi Connected .. . . .");
    WiFiClient client;

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    // httpUpdate.setLedPin(LED_BUILTIN, LOW);

    Serial.println("\n\n\t inside OTA_HTTP_UPDATER() - connecting to Server .. . . .");
    
    t_httpUpdate_return ret = httpUpdate.update(client, "http://10.208.34.163:3000/updater");
    //t_httpUpdate_return ret = httpUpdate.update(client, "http://server/file.bin");
    // Or:
    //t_httpUpdate_return ret = httpUpdate.update(client, "server", 80, "file.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }

    Serial.println("\n\n\t exiting OTA_HTTP_UPDATER() .. . . .");
    
  }
}
