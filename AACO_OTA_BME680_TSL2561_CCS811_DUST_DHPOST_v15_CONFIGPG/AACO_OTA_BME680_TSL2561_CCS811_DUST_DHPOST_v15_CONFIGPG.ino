// https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas?view=all
//   https://randomnerdtutorials.com/esp32-static-fixed-ip-address-arduino-ide/
//   https://microcontrollerslab.com/esp32-static-fix-ip-address/
//   https://techtutorialsx.com/2020/04/21/esp32-arduino-set-static-ip-address/
//   https://stevessmarthomeguide.com/understanding-dhcp-home-networks/
// Use static IP & avoid hostnames (cloudmqtt.com) USE DIRECT IP ADDRESSES INSTEAD

// http://10.208.34.79:8383/ne-iot/layouts.html
// http://10.208.34.79:3000/graphArtifact/5e5de820d10c9d2548e97793
// http://10.208.34.79:3000/editArtifactDetails/5e5de820d10c9d2548e97793
// http://10.208.34.79:3000/graphArtifact/5e61f56b42a34006542350da
// http://10.208.34.79:3000/editArtifactDetails/5e61f19f42a34006542350d7
// http://10.208.34.79:3000/registerArtifact
// http://10.208.34.79:3000/registerArtifactJatan

/* ESP32 BROWNOUT DETECTION
  https://github.com/nkolban/esp32-snippets/issues/168
  https://github.com/espressif/arduino-esp32/issues/863
  https://www.esp32.com/viewtopic.php?t=2462
*/

/*
  // How many readings we want to store before sending them over WiFi
  // -> 20 readings with 15min between each reading = only connecting to WiFi every 300 minutes (5hours) instead of every 15min
  #define MAX_OFFLINE_READINGS 20

  // Place in the RTC memory to store the offline readings (and how many we have so far)
  RTC_DATA_ATTR unsigned char offlineReadingCount = 0;
  RTC_DATA_ATTR unsigned int readings_temp[MAX_OFFLINE_READINGS + 1];

  // Write the temperature (20.9°C) to the RTC memory. You might want to replace this with your actual measurement code ;)
  readings_temp[offlineReadingCount] = 209;
  offlineReadingCount++;

  // If we collected less then the maximum amount of readings, go back to deep sleep!
  if(offlineReadingCount <= MAX_OFFLINE_READINGS){
    goToDeepSleep();
    return;
  }

  // If we get here we have to send the readings over WiFi
  sendReadings();

  // Don't forget to reset the counters!
  offlineReadingCount = 0;

    Temperature [°C]          GREEN 20C < T < 22.5C RED
  Relative Humidity [%]         GREEN 30% < H <50% RED
  Light Intensity [Lux]         GREEN 50 < Lux < 200 RED
  Battery Voltage [V]         RED 3V < bat. < 5V
  CO₂2 [parts per million (ppm)]    GREEN CO2 < 1000 RED
  Dust PM10 (<10µm) [(ppm)]   GREEN DUST < 100 RED

    (μg/m3) NOT= ppb


*/



//#include "soc/soc.h"
//#include "soc/rtc_cntl_reg.h"
// WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

// #ifdef ESP32
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ESP32Ping.h> // https://github.com/marian-craciunescu/ESP32Ping
//#include <WiFiMulti.h>
//WiFiMulti wifiMulti;

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#define LED_BUILTIN   2 // GPIO02 ESP32
#define LED2 LED_BUILTIN
//#define BUZZER 12 // GPIO12 ESP32
//#define REDLED 12
//#define GRNLED 13
//#define BLULED 14

//#define DUST_SHARP
#define DUST_SDS011
#define OLED  // UNCOMMENT TO ENABLE
//#define EPAPER2in13   // UNCOMMENT TO ENABLE

#define PFET_3V3_BUS 25 // GPIO25 ESP32
#define BATTERY_V_IN 35 // GPIO35 ESP32

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!ALERT!!!!!!!!!!!!!!!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// 'PFET_POT_DIV' PIN NON-EXISTENT ON LASTEST AACO_PCBv2 PCB (FEB 2021)
//#define AACO_PCBv1 // OLD PCB (JAN 2020)
#ifdef AACO_PCBv1
#define PFET_POT_DIV 32 // GPIO32 ESP32 // FOR AACO_PCBv1 : OLD PCB (JAN 2020)
#endif
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!ALERT!!!!!!!!!!!!!!!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

 
// #if defined(ESP8266)              // ESP12E - ESP8266cdac
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// //#include <WiFiClientSecure.h>
// //#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
// //ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
// //#else // ESP32
// #endif

#include "SSID.h"

#include <driver/rtc_io.h>
#include "driver/adc.h"
#include <esp_wifi.h>
//#include <esp_bt.h>



#define WIFI_TIMEOUT 1000 // 1second in milliseconds
//#define DEEP_SLEEP_TIME 10 // seconds

// ESP32 Wake-up Source : Timer
#define uS_TO_S_FACTOR 1000000L  //Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP_S  5         //Time ESP32 sleep duration (in seconds)
#define TIME_TO_SLEEP_M  5 * 60L   //Time ESP32 sleep duration (in minutes)



//#include <Adafruit_Sensor.h>
//#include "Adafruit_BME680.h"
//#define SEALEVELPRESSURE_HPA (1013.25)
//Adafruit_BME680 bme; // I2C

// UNCOMMENT TO ENABLE SERIAL PORT DEBUG MESSAGES
#define DEBUG_Sprintln(a) (Serial.println(a))
#define DEBUG_Sprint(a) (Serial.print(a))
#define DEBUG_Sprint2ln(a, b) (Serial.println(a, b)) // DEBUG_Sprint2ln((int) number, DEC);
#define DEBUG_Sprint2(a, b) (Serial.print(a, b))
#define BAUD_RATE 115200 //static const uint8_t

// UNCOMMENT TO DISABLE SERIAL PORT DEBUG MESSAGES
//#define DEBUG_Sprintln(a)
//#define DEBUG_Sprint(a)
//#define DEBUG_Sprint2ln(a, b)
//#define DEBUG_Sprint2(a, b)
//#define BAUD_RATE 9600 // USED BY SENSOR ON UART0 //static const uint8_t

String SOFT_AP_SSID;

// IP             // GATEWAY/BROADCAST  // NETMASK/SUB-NET
//10.208.35.169   // 10.208.35.255      // 255.255.254.0      // 10.208.0.11  // 10.208.11.16  // CENTOS
//10.208.34.163   // 10.208.35.255      // 255.255.254.0      // 10.208.0.11  // 10.208.11.16  // UBUNTU
//10.208.22.151   // 10.208.22.1        // 255.255.254.0      // 10.208.0.11  // 10.208.11.16  // HP WIFI

// wifi-AACO_STATIC_2
//10.208.34.23    // 10.208.34.255      // 255.255.254.0      // 10.208.0.11  // 10.208.11.16
// STA IP: 10.208.35.227, MASK: 255.255.254.0, GW: 10.208.34.1
IPAddress local_IP(10, 208, 34, 23); // Set your Static IP address
IPAddress gateway(10, 208, 34, 255); // Set your Gateway IP address
IPAddress subnet(255, 255, 254, 0);
IPAddress primaryDNS(10, 208, 0, 11);   //optional
IPAddress secondaryDNS(10, 208, 11, 16); //optional

//IPAddress local_IP(192, 168, 0, 184); // Set your Static IP address
//IPAddress gateway(192, 168, 0, 1); // Set your Gateway IP address
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8);   //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional
//IPAddress primaryDNS(192, 168, 0, 1); //optional
//IPAddress secondaryDNS(0, 0, 0, 0); //optional


//static const uint8_t TX0_PIN = 1;
//static const uint8_t RX0_PIN = 3;
static const uint8_t SCL_PIN = 22;
static const uint8_t SDA_PIN = 21;
//int LED2 = 2; //GPIO2 - ESP32
//const char* ssid = "CDACIoT";       const char* password = "hcdc_IoT";
//const char* ssid = "cdac";          const char* password = "";
//RTC_DATA_ATTR char* ssid = "cdac";  RTC_DATA_ATTR char* password = "";
//RTC_DATA_ATTR char* ssid = SECRET_SSID;
//RTC_DATA_ATTR char* password = SECRET_OPTIONAL_PWD;
RTC_DATA_ATTR long int bootCount = 0;
RTC_DATA_ATTR long int WiFiConnRetryAttempt = 0;
//RTC_DATA_ATTR int

//#define NEW_FIRMWARE_MAJOR_VERSION 8
//#define NEW_FIRMWARE_MINOR_VERSION 2
const int NEW_FIRMWARE_MAJOR_VERSION = 10;
const int NEW_FIRMWARE_MINOR_VERSION = 2;
RTC_DATA_ATTR static int FIRMWARE_MAJOR_VERSION; // = 0; // = NEW_FIRMWARE_MAJOR_VERSION;
RTC_DATA_ATTR static int FIRMWARE_MINOR_VERSION; // = 0; // = NEW_FIRMWARE_MINOR_VERSION;

//double VCC;

//double HDC1080_temperature;
//double HDC1080_humidity;

double lux = 0;
float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0; // BME680
float pm_2point5 = 0, pm_10 = 0, CO2 = 0, VOC = 0;
uint16_t eCO2, tVOC; // estimated total Volatile Organic Compunds // errstat, raw;
float batteryRaw = 0.0, batteryLevel = 0.0, batteryLevelPercent = 0.0;
String timeStamp, timeHMS, Date;
String found_BLE_MAC_list;
//String floatToString;

int httpCode;
String deviceName = "", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;
String WiFi_Status = "DISCONNECTED";
String HTTP_post_status = "FAILURE";
static uint8_t POST_EVERY_x_mS = 2000; // TO SEND POST NOTIFICATIONS AFTER EVERY 2 SECONDS

unsigned long totalPwrOnDuration = 0, WiFiConnAttemptDuration = 0, WiFiOnDuration = 0, WiFiOffDuration = 0, OTADuration = 0;
//  '{"T":'+String(tempC)+',"H":'+String(humidity)+',"L":'+String(lux)+',"C":'+String(CO2)+',"D":'+String(pm_2point5)+',"B":'+String(batteryLevel)+',"TS":'+String(timeStamp)'}'

 
//void RGB_LED_OFF()
//{
//  digitalWrite(REDLED, HIGH);
//  digitalWrite(GRNLED, HIGH);
//  digitalWrite(BLULED, HIGH);
//}
//void RGB_LED_ON()
//{
//  digitalWrite(REDLED, LOW);
//  digitalWrite(GRNLED, LOW);
//  digitalWrite(BLULED, LOW);
//}


// 24:0A:C4:83:20:C0 38PIN
// CC:50:E3:A9:4C:D8 32PIN

String ChipID; // 6 BYTE ESP32 CHIP ID

void readChipID()
{
  uint64_t chipid = ESP.getEfuseMac(); // uint32_t //The chip ID is essentially its MAC address(length: 6 bytes).

  unsigned long long1 = (unsigned long)((chipid >> 24) & 0xFFFFFF); // >> 16
  unsigned long long2 = (unsigned long)((chipid & 0xFFFFFF));

  //Serial.println(String(long1, HEX));
  //Serial.println(String(long2, HEX));

  ChipID = String((unsigned long)((chipid >> 24) & 0xFFFFFF), HEX) + String((unsigned long)(chipid & 0xFFFFFF), HEX); // six octets
  //ChipID = String(long1, HEX) + String(long2, HEX); // six octets
  Serial.println(String("ESP32 CHIP ID : ") + ChipID); // D84CA9E350CC

  //Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  //Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  // http://www.cplusplus.com/reference/cstdio/printf/
  // https://stackoverflow.com/questions/15108932/c-the-x-format-specifier

}


// Method to print the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}



void setup()
{ // WAKE UP FROM DEEPSLEEP
  totalPwrOnDuration = millis();
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  //  setCpuFrequencyMhz(240); // 240Mhz = 63mA (WiFi, Bluetooth, SPI, I2C -> On)
  //  setCpuFrequencyMhz(160); // 160Mhz = 42mA (WiFi, Bluetooth, SPI, I2C -> On)
  setCpuFrequencyMhz(80); // 80Mhz = 30mA (WiFi, Bluetooth, SPI, I2C -> On)


  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  pinMode(LED2, OUTPUT);
//  pinMode(REDLED, OUTPUT); // GPIO12
//  pinMode(GRNLED, OUTPUT); // GPIO13
//  pinMode(BLULED, OUTPUT); // GPIO14
//  RGB_LED_ON();
//  RGB_LED_OFF();
  //pinMode(BUZZER, INPUT); // BUZ OFF

  //  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON BUS
  //  pinMode(PFET_3V3_BUS, INPUT);  // TURN OFF BUS
  //  digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS

  // WiFi_OFF();  //  WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();

  Serial.begin(BAUD_RATE); delay(100);

  Serial.println("\n\n*********************WAKE UP***********************\n\n");


  readChipID(); // initialize the global variable 'ChipID'

  //  if (!bootCount)
  //  { WiFiManagerSetup();
  //  }
  //  WiFi_OFF();

  bootCount++; //Increment boot number and print it every reboot
  Serial.println("Boot Count: " + String(bootCount));

  print_wakeup_reason(); //Print the wakeup reason for ESP32

  Serial.print("ESP32 SDK: ");  Serial.println(ESP.getSdkVersion());

  //Serial.println("\nFIRMWARE_MAJOR_VERSION: " + String(NEW_FIRMWARE_MAJOR_VERSION));

  //if(NEW_FIRMWARE_MAJOR_VERSION > FIRMWARE_MAJOR_VERSION)
  { FIRMWARE_MAJOR_VERSION = NEW_FIRMWARE_MAJOR_VERSION;
    Serial.println("\nFIRMWARE_MAJOR_VERSION: " + String(FIRMWARE_MAJOR_VERSION));
    Serial.println("\nNEW_FIRMWARE_MAJOR_VERSION: " + String(NEW_FIRMWARE_MAJOR_VERSION));
  }

  //if(NEW_FIRMWARE_MINOR_VERSION > FIRMWARE_MINOR_VERSION)
  { FIRMWARE_MINOR_VERSION = NEW_FIRMWARE_MINOR_VERSION;
    Serial.println("\nFIRMWARE_MINOR_VERSION: " + String(FIRMWARE_MINOR_VERSION));
    Serial.println("\nNEW_FIRMWARE_MINOR_VERSION: " + String(NEW_FIRMWARE_MINOR_VERSION));
  }

  //  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON BUS
  //  digitalWrite(PFET_3V3_BUS, LOW); // POWER ON 3V3 BUS -> TURN ON SPI/I2C PERIPHERALS
  //  delay(1);

  //  batteryLevelRead_setup();
  //  RTC_DS3231_setup();
  //  BME680_Simple_setup(); //  BME680_Air_Q_setup();
  //  LUX_TSL2561_setup(); //  LUX_BH1750_setup();
  //  HDC1080_setup(); // humidity sensor
  //  CO2_I2C_CCS811_setup();
  //  SDS011_DUST_MHZ19B_CO2_setup();


  /////////////////// FOR OLED : UNCOMMENT NEXT LINE &  THIS TAB : "SSD1306_OLED_128x64_I2C" /////////////////////
  //  SSD1306_128x64_setup();  // DRAW_BITMAP_LOGO(); // print_PARAMS();
  //  DRAW_BITMAP_LOGO();
  //  OLED_BLANK_SCREEN();

  Serial.println("\n\n*********************START UP DONE***********************\n\n");

  // digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
  //pinMode(PFET_3V3_BUS, INPUT); // TURN OFF BUS
  //  delay(1);

  //  WiFi_ON(); // 84:0D:8E:C3:60:8C ESP32S
  // 84:0D:8E:C3:93:C0

  loopOnce();
}

void loop()
{}

void loopOnce()
{

  // TEST_OLED();
  initialize();

  Serial.println("\n POWERING ON 3V3 BUS -> TURNING ON SPI/I2C PERIPHERALS ");
  //  digitalWrite(PFET_POT_DIV, LOW); // POWER ON POT DIV
  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON BUS
  digitalWrite(PFET_3V3_BUS, LOW); // POWER ON 3V3 BUS -> TURN ON SPI/I2C PERIPHERALS
  delay(1);
  digitalWrite(LED_BUILTIN, HIGH); //HIGH = ON //LOW = OFF
//  RGB_LED_ON(); // digitalWrite(BLULED, LOW);
  
  batteryLevelRead_setup();
  batteryLevelRead(); // delay(500);
  BME680_Simple_setup();
  BME680_Simple_loop();
  LUX_TSL2561_setup();
  LUX_TSL2561_loop();

#ifdef DUST_SHARP
  DUST_SHARP_setup();   //DUST_SHARP_print();
#endif

#ifdef DUST_SDS011
  SDS011_DUST_MHZ19B_CO2_setup(); //   SDS011_DUST_loop();  delay(5);
#endif

  CO2_I2C_CCS811_setup();   //CO2_I2C_CCS811_loop();   // delay(5);

  //  BME680_Air_Q_setup(); //  BME680_Air_Q_loop();  //delay(1);
  //  LUX_BH1750_setup(); //  LUX_BH1750_loop();  //delay(1);
  //  HDC1080_setup();  // HDC1080_loop();

  // TimeNow();   delay(50);

  
  // RGB_LED_OFF();
  Serial.println("\n POWERING OFF 3V3 BUS -> TURNING OFF SPI/I2C PERIPHERALS \n");
  digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
  // pinMode(PFET_3V3_BUS, INPUT); // TURN OFF 3V3 BUS
  delay(5);
  digitalWrite(LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF
  // TEST_OLED(); // CAUSES CRASH AS OLED HASN'T BEEN INITIALIZED YET

  //  delay(POST_EVERY_x_mS);


  //if (WiFi.status() != WL_CONNECTED)  WiFi_ON(); // 84:0D:8E:C3:60:8C ESP32S 84:0d:8e:c3:60:8c
  // WIFI_STATUS_OLED();

  HTTP_POST_NOTIF();  //  delay(1500);

  if (WiFi.status() == WL_CONNECTED)
  {
    //if(!bootCount)
    //if (bootCount < 6) // OR USE AN EXTERNAL INTERRUPT TO TRIGGER THE OTA FEATURE
    {
      //  OTAsetup(); // OTA VIA WEB SERVER RUNNING IN ESP32
    }

    // OTA_HTTPS_UPDATER(); // OTA FROM CDAC SERVER
    //  OTA_HTTP_UPDATER(); // OTA FROM CDAC SERVER

    //  WEB_SERVER_setup();
    //  WEB_SERVER_loop();
    //  delay(5*60000);
  }

  WiFi_OFF();
  //WIFI_STATUS_OLED();



  Serial.println("\n POWERING ON 3V3 BUS -> TURNING ON SPI/I2C PERIPHERALS ");
  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON 3V3 BUS
  digitalWrite(PFET_3V3_BUS, LOW); // POWER ON 3V3 BUS -> TURN ON SPI/I2C PERIPHERALS
  delay(5);
  digitalWrite (LED_BUILTIN, HIGH); //HIGH = ON //LOW = OFF
//  RGB_LED_ON(); // digitalWrite(BLULED, LOW);
  
#ifdef EPAPER2in13
  /////////////////// FOR EPAPER2in13 : UNCOMMENT NEXT LINE &  THIS TAB :  "PRINT_VALUES_EPAPER2in13_ESP32" /////////////////////
  WIFI_HTTP_STATUS_EPAPER2in13();
  EPAPER2in13PrintValues1();
#endif


#ifdef OLED
  /////////////////// FOR OLED : UNCOMMENT NEXT LINE &  THIS TAB : "SSD1306_OLED_128x64_I2C" /////////////////////
  SSD1306_128x64_setup();  //  SSD1306_128x64_loop();  // print_PARAMS();  delay(100);
  DRAW_BITMAP_LOGO();
  print_PARAMS();
  OLED_BLANK_SCREEN();

  //  SSD1306_128x64_setup();  //  SSD1306_128x64_loop();  // print_PARAMS();  delay(100);
  WIFI_HTTP_STATUS_OLED();
  OLED_BLANK_SCREEN();
#endif

  // RGB_LED_OFF();
  Serial.println("\n POWERING OFF 3V3 BUS -> TURNING OFF SPI/I2C PERIPHERALS \n ");
  digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
  //pinMode(PFET_3V3_BUS, INPUT); // TURN OFF 3V3 BUS
  //delay(10);
  digitalWrite (LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF


  //  digitalWrite(PFET_LATCH_NPN, LOW); // ESP32 OFF
  //  pinMode(PFET_3V3_BUS, INPUT); // TURN OFF BUS
  //  digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS

  //  digitalWrite(PFET_POT_DIV, HIGH); // POWER OFF POT DIV

  //  TEST_OLED();

  // Serial.println("\n\n*********************NEXT LOOP***********************\n\n");

  // DEEPSLEEP CODE - GO TO SLEEP

  //  adc_power_off();
  //  esp_wifi_stop();
  //  btStart();
  //  btStop();
  //  esp_bt_controller_disable();

  // https://github.com/G6EJD/ESP32-Push-button-or-Pulse-Wake-up/blob/master/ESP32_Switrch_WakeUp_Example.ino
  //   rtc_gpio_pulldown_en((gpio_num_t)GPIO_NUM_34);//Enables ESP32 pullup or pulldown resistor during deep sleep
  //   esp_sleep_enable_ext0_wakeup((gpio_num_t)GPIO_NUM_34, RISING);

  // ext0 – Use it when you want to wake-up the chip by one particular pin only.
  // Configure GPIO34 as ext0 wake up source for LOW logic level
  // esp_sleep_enable_ext0_wakeup((gpio_num_t)GPIO_NUM_34,0); //1 = High, 0 = Low

  // Configure the timer to wake us up!
  // esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 60L * 1000000L);

  // ESP32 Wake-up Source : Timer
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_S * uS_TO_S_FACTOR); //Set timer to 5 seconds
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP_S) + " Seconds");
  Serial.println("Going to sleep now");
  Serial.println("\n\n*********************DEEP SLEEP***********************\n\n");

  totalPwrOnDuration = millis() - totalPwrOnDuration;
  WiFiOffDuration = totalPwrOnDuration - WiFiConnAttemptDuration - WiFiOnDuration;
  Serial.println("\nWiFiOffDuration: " + String(WiFiOffDuration) + "ms\n");
  Serial.println("WiFiConnAttemptDuration: " + String(WiFiConnAttemptDuration) + "ms\n");
  Serial.println("WiFiOnDuration: " + String(WiFiOnDuration) + "ms\n");
  Serial.println("OTADuration: " + String(OTADuration) + "ms\n");
  Serial.println("totalPwrOnDuration: " + String(totalPwrOnDuration) + "ms\n");
  //  digitalWrite (LED_BUILTIN, LOW); //HIGH = ON //LOW = OFF
  //  pinMode(LED_BUILTIN, INPUT); // LED OFF
  esp_deep_sleep_start();
  Serial.println("This will never be printed");

}


/*
  // https://www.savjee.be/2019/12/esp32-tips-to-increase-battery-life/
  void goToDeepSleep()
  {
  Serial.println("Going to sleep...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();

  // Configure the timer to wake us up!
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 60L * 1000000L);

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
  }
*/
