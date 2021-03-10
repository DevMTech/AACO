/**
   httpUpdateSecure.ino

*/

//#include <Arduino.h>
//#include <WiFi.h>
//#include <HTTPClient.h>
//#include <HTTPUpdate.h>


/**
 * This is lets-encrypt-x3-cross-signed.pem
 */
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIID4TCCAsmgAwIBAgIJAIEIY3L5h83wMA0GCSqGSIb3DQEBCwUAMIGGMQswCQYD\n" \
"VQQGEwJJTjEUMBIGA1UECAwLTWFoYXJhc2h0cmExDTALBgNVBAcMBFB1bmUxDTAL\n" \
"BgNVBAoMBENEQUMxDTALBgNVBAsMBEhDREMxEjAQBgNVBAMMCWNocHJhZGVlcDEg\n" \
"MB4GCSqGSIb3DQEJARYRY2hwcmFkZWVwQGNkYWMuaW4wHhcNMjAxMjAyMDk0MzUy\n" \
"WhcNMjExMjAyMDk0MzUyWjCBhjELMAkGA1UEBhMCSU4xFDASBgNVBAgMC01haGFy\n" \
"YXNodHJhMQ0wCwYDVQQHDARQdW5lMQ0wCwYDVQQKDARDREFDMQ0wCwYDVQQLDARI\n" \
"Q0RDMRIwEAYDVQQDDAljaHByYWRlZXAxIDAeBgkqhkiG9w0BCQEWEWNocHJhZGVl\n" \
"cEBjZGFjLmluMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2rGo4Vky\n" \
"/hMsIiEHXXTNy/3MUjksXfMNTE6gfzlHV+THwiq8h031qSr87r5x2uICIwzBtagC\n" \
"gIwWRfYXDH8KgLZVMEuzH4Wkl+OuqrS1S3ncj8ep1ZMa42pyMB1Fq3xTo7tsbIw7\n" \
"oRJXldAmG2V4IWEF1xLbStWBrniLfUGaW8EY1gJVsjhk4Z15TZZLwyy8jokflL7C\n" \
"NUvoMQkajpzuWJWPjkh+cgVnsretfWLPwnoXV+gO/UrmHF7ObG3uIwAZ0R4/M0Lj\n" \
"dBEYCE/sb758qZVwJ3WXmPQggezlN7nqlSnVXDmaB1QTe/AkjypBr0jUFthOBIkK\n" \
"KhzT/S26kH3y6wIDAQABo1AwTjAdBgNVHQ4EFgQU/MXnJ++MTmtzLC4q0dhAvA86\n" \
"8icwHwYDVR0jBBgwFoAU/MXnJ++MTmtzLC4q0dhAvA868icwDAYDVR0TBAUwAwEB\n" \
"/zANBgkqhkiG9w0BAQsFAAOCAQEAHr6VWZo+VVAaDPi7jW3Ka1ktdBDwSSugMXrc\n" \
"gT5RHsdjbF2k3tUSprI0ukXzSB1n3xcp2TmxGL552j7w/f8ZB4Vg3xbHFO8TmTK0\n" \
"1NsugZPeRPlmOTPIEaiPIaNt+3TF8+bwbx1pjEBFMz3KUEoGznory9UIkhSOJLyZ\n" \
"wkdQgSxioXeOhRhkv9puLJJHcig8ESOg9AMJzgEY2U0r6uIU4Uy2PJHpM3QsJDFY\n" \
"NTMHJn4PJr9SzGQeFoQHczGfP38eTXMXsaUiqBH/DrHVMu7sg3fqFryOaPxq/9dl\n" \
"SYl8/6Fj6b0qzia0O8Y2sV4I+Z63gbq5m1XMIgtTN9mtF1mUOg==\n" \
"-----END CERTIFICATE-----\n"; 

void OTA_HTTPS_UPDATER() 
{
  // wait for WiFi connection
//  if ((WiFiMulti.run() == WL_CONNECTED)) 
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\n\t inside OTA_HTTP_UPDATER() - WiFi Connected .. . . .");
    
    //WiFiClient client;
    WiFiClientSecure client;
    client.setCACert(rootCACertificate);

    // Reading data over SSL may be slow, use an adequate timeout
    client.setTimeout(12000 / 1000); // timeout argument is defined in seconds for setTimeout

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    // httpUpdate.setLedPin(LED_BUILTIN, HIGH);

    Serial.println("\n\n\t inside OTA_HTTP_UPDATER() - connecting to Server .. . . .");

    // https://10.208.34.200:3000/api/AACO_FIRMWARE_v11_esp32.bin
    // https://10.208.34.200:3000/api/updater
    
    //t_httpUpdate_return ret = httpUpdate.update(client, "https://10.208.34.200:3000/api/updater");
    t_httpUpdate_return ret = httpUpdate.update(client, "https://10.208.34.200:3000/api/updater/AACO_FIRMWARE_v11_esp32.bin");
    // t_httpUpdate_return ret = httpUpdate.update(client, "http://10.208.34.163:3000/updater");
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
