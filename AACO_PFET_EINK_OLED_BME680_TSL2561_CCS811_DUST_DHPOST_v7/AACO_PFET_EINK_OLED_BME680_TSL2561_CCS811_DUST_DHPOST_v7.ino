// https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas?view=all
// Use static IP & avoid hostnames (cloudmqtt.com) USE DIRECT IP ADDRESSES INSTEAD

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

#ifdef ESP32
#define LED_BUILTIN   2 // GPIO02 ESP32
#define PFET_3V3_BUS 25 // GPIO25 ESP32
#define PFET_POT_DIV 32 // GPIO32 ESP32
#define BATTERY_V_IN 35 // GPIO35 ESP32
#endif

#if defined(ESP8266)              // ESP12E - ESP8266cdac
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//#include <WiFiClientSecure.h>
//      #include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
//      ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

#else                             // ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#endif

#include "SSID.h"

#include <driver/rtc_io.h>
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>

#define WIFI_TIMEOUT 1000 // 1second in milliseconds
//#define DEEP_SLEEP_TIME 10 // seconds

// ESP32 Wake-up Source : Timer
#define uS_TO_S_FACTOR 1000000L  //Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP_S  5         //Time ESP32 sleep duration (in seconds)
#define TIME_TO_SLEEP_M  5 * 60L   //Time ESP32 sleep duration (in minutes)

#define BUZZER 12 // GPIO12 ESP32

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

//static const uint8_t TX0_PIN = 1;
//static const uint8_t RX0_PIN = 3;
static const uint8_t SCL_PIN = 22;
static const uint8_t SDA_PIN = 21;
int LED2 = 2; //GPIO2 - ESP32
// long int count = 0;
RTC_DATA_ATTR long int bootCount = 0;
//RTC_DATA_ATTR int

double VCC;

double HDC1080_temperature;
double HDC1080_humidity;

float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0, VOC = 0; // BME680
double lux = 0;
float pm_2point5 = 0, pm_10 = 0, CO2 = 0;
float batteryRaw = 0.0, batteryLevel = 0.0, batteryLevelPercent = 0.0;
uint16_t eCO2; // , e_total_VOC, errstat, raw;
String timeStamp, timeHMS, Date;

int httpCode;
String deviceName = "", deviceDescription, deviceMAC, deviceIP, deviceConfigParams;
String WiFi_Status = "DISCONNECTED";
String HTTP_post_status = "FAILURE";
static uint8_t POST_EVERY_x_mS = 2000; // TO SEND POST NOTIFICATIONS AFTER EVERY 2 SECONDS




unsigned long totalPwrOnDuration = 0, WiFiConnAttemptDuration = 0, WiFiOnDuration = 0;
//  '{"T":'+String(tempC)+',"H":'+String(humidity)+',"L":'+String(lux)+',"C":'+String(CO2)+',"D":'+String(pm_2point5)+',"B":'+String(batteryLevel)+',"TS":'+String(timeStamp)'}'


void setup()
{ // WAKE UP FROM DEEPSLEEP
  totalPwrOnDuration = millis();
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  //  setCpuFrequencyMhz(240); // 240Mhz = 63mA (WiFi, Bluetooth, SPI, I2C -> On)
  //  setCpuFrequencyMhz(160); // 160Mhz = 42mA (WiFi, Bluetooth, SPI, I2C -> On)
  setCpuFrequencyMhz(80); // 80Mhz = 30mA (WiFi, Bluetooth, SPI, I2C -> On)


  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);

  //  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON BUS
  //  pinMode(PFET_3V3_BUS, INPUT);  // TURN OFF BUS
  //  digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS

  // WiFi_OFF();  //  WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();



  Serial.begin(BAUD_RATE);

  Serial.println("\n\n*********************WAKE UP***********************\n\n");


  bootCount++;
  Serial.println("Boot Count: " + String(bootCount));

  Serial.print("ESP32 SDK: ");  Serial.println(ESP.getSdkVersion());

  //  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON BUS
  //  digitalWrite(PFET_3V3_BUS, LOW); // POWER ON 3V3 BUS -> TURN ON SPI/I2C PERIPHERALS
  //  delay(1);

  //  batteryLevelRead_setup();

  //  RTC_DS3231_setup();

  //  BME680_Simple_setup(); //  BME680_Air_Q_setup();

  //  LUX_TSL2561_setup(); //  LUX_BH1750_setup();

  //  HDC1080_setup();

  //  CO2_I2C_CCS811_setup();

  //  SDS011_DUST_MHZ19B_CO2_setup();


  /////////////////// FOR OLED : UNCOMMENT NEXT LINE &  THIS TAB : "SSD1306_OLED_128x64_I2C" /////////////////////
  //  SSD1306_128x64_setup();  // DRAW_BITMAP_LOGO(); // print_PARAMS();
  //  DRAW_BITMAP_LOGO();
  //  BLANK_SCREEN();

  Serial.println("\n\n*********************START UP DONE***********************\n\n");

  // digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
  //pinMode(PFET_3V3_BUS, INPUT); // TURN OFF BUS
  //  delay(1);

  //  WiFi_ON();  // WiFi_setup(); // 84:0D:8E:C3:60:8C ESP32S
  // 84:0D:8E:C3:93:C0

  loopOnce();
}

void loop()
{}

void loopOnce()
{

  // TEST_OLED();
  //  digitalWrite(PFET_POT_DIV, LOW); // POWER ON POT DIV
  pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON BUS
  digitalWrite(PFET_3V3_BUS, LOW); // POWER ON 3V3 BUS -> TURN ON SPI/I2C PERIPHERALS
  delay(1);

  batteryLevelRead_setup();
  batteryLevelRead(); // delay(500);

  BME680_Simple_setup();
  BME680_Simple_loop();

  LUX_TSL2561_setup();
  LUX_TSL2561_loop();

  //  BME680_Air_Q_setup(); //  BME680_Air_Q_loop();  //delay(1);
  //  LUX_BH1750_setup(); //  LUX_BH1750_loop();  //delay(1);
  // HDC1080_setup();  // HDC1080_loop();

  CO2_I2C_CCS811_setup();   //CO2_I2C_CCS811_loop();   // delay(5);

  SDS011_DUST_MHZ19B_CO2_setup(); //   SDS011_DUST_loop();  delay(5);

  // TimeNow();   delay(50);

  /////////////////// FOR EPAPER : UNCOMMENT NEXT LINE &  THIS TAB :  "PRINT_VALUES_EPAPER_ESP32" /////////////////////
  //  ePaperSetup(); // ePaperPrintValues(); delay(1000);  //  delay(500);

  /////////////////// FOR OLED : UNCOMMENT NEXT LINE &  THIS TAB : "SSD1306_OLED_128x64_I2C" /////////////////////
  //  SSD1306_128x64_setup();  //  SSD1306_128x64_loop();  // print_PARAMS();  delay(100);
  //  DRAW_BITMAP_LOGO();
  //  print_PARAMS();
  //  BLANK_SCREEN();

  digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
  // pinMode(PFET_3V3_BUS, INPUT); // TURN OFF 3V3 BUS
  delay(5);

  // TEST_OLED(); // CAUSES CRASH AS OLED HASN'T BEEN INITIALIZED YET

  //  delay(POST_EVERY_x_mS);

  // WiFi_ON();
  //  if (WiFi.status() != WL_CONNECTED)  WiFi_setup(); // 84:0D:8E:C3:60:8C ESP32S 84:0d:8e:c3:60:8c
  //WIFI_STATUS_OLED();

  HTTP_POST_NOTIF();  //  delay(1500);

  WEB_SERVER_setup();
  WEB_SERVER_loop();

  delay(5 * 60000);
  /*
    pinMode(PFET_3V3_BUS, OUTPUT); // TURN ON 3V3 BUS
    digitalWrite(PFET_3V3_BUS, LOW); // POWER ON 3V3 BUS -> TURN ON SPI/I2C PERIPHERALS
    delay(5);

    /////////////////// FOR OLED : UNCOMMENT NEXT LINE &  THIS TAB : "SSD1306_OLED_128x64_I2C" /////////////////////
    SSD1306_128x64_setup();  //  SSD1306_128x64_loop();  // print_PARAMS();  delay(100);

    WIFI_HTTP_STATUS_OLED();
    BLANK_SCREEN();

    digitalWrite(PFET_3V3_BUS, HIGH); // POWER OFF 3V3 BUS -> TURN OFF SPI/I2C PERIPHERALS
    //pinMode(PFET_3V3_BUS, INPUT); // TURN OFF 3V3 BUS
    //delay(10);
  */

  WiFi_OFF();
  //WIFI_STATUS_OLED();


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

  Serial.println("\nWiFiConnAttemptDuration: " + String(WiFiConnAttemptDuration) + "ms\n");
  Serial.println("WiFiOnDuration: " + String(WiFiOnDuration) + "ms\n");
  Serial.println("totalPwrOnDuration: " + String(totalPwrOnDuration) + "ms\n");

  esp_deep_sleep_start();
  Serial.println("This will never be printed");

}

void WiFi_OFF()
{ WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();
  delay(1);
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi_Status = "CONNECTED";
    // Serial.println("\n================= WIFI_OFF : WiFi CONNECTED ==================\n");

    pinMode (LED2, OUTPUT);
    digitalWrite (LED2, HIGH); //HIGH = ON //LOW = OFF
  }
  else
  { WiFiOnDuration = millis() - WiFiOnDuration;

    WiFi_Status = "DISCONNECTED";
    // Serial.println("\n================= WIFI_OFF : WiFi DISCONNECTED ==================\n");

    pinMode (LED2, OUTPUT);
    digitalWrite (LED2, LOW); //HIGH = ON //LOW = OFF
    pinMode (LED2, INPUT);
  }
}

void WiFi_ON()
{
  //  WiFi.forceSleepWake();
  // delay(1);
  WiFi_setup();
}

void WiFi_setup()
{
  Serial.print("\n\nMAC : "); // 84:0D:8E:C3:60:8C ESP32S
  Serial.println(WiFi.macAddress());


  WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  //  wifiMulti.addAP(ssid, password);
  //  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid0, password0);
  // WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();



  // unsigned long startAttemptTime = millis(); // Keep track of when we started our attempt to get a WiFi connection
  //  // Keep looping while we're not connected AND haven't reached the timeout
  //  while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime < WIFI_TIMEOUT))
  //  { delay(10);
  //  }

  int8_t c = 0;
  Serial.println("Connecting Wifi...");
  WiFiConnAttemptDuration += millis(); // Keep track of when we started our attempt to get a WiFi connection

  //while (WiFi.status() != WL_CONNECTED && c++ < 100)
  while (wifiMulti.run() != WL_CONNECTED && c++ < 10)
  { delay(1); Serial.print("."); //  Serial.println("Connecting..");
  }
  Serial.println("Connection Attempts .." + String(c));
  WiFiConnAttemptDuration = millis() - WiFiConnAttemptDuration;



  //  if (WiFi.status() == WL_CONNECTED)
  if (wifiMulti.run() == WL_CONNECTED)
  { WiFiOnDuration += millis();
    WiFi_Status = "CONNECTED";
    // Serial.println("\n================= WIFI_ON : WiFi CONNECTED ==================\n");
    // Serial.println("\nWiFi Connection to " + String(ssid) + " established!");

    //    Serial.print("\nConnected to ");    Serial.println(WiFi.SSID());
    //    Serial.print("\nIP address:\t");    Serial.println(WiFi.localIP());

    deviceDescription = "Antiquity Ambient Condition Observer";
    deviceMAC = WiFi.macAddress();              //macToStr(mac);  //mac;
    deviceIP = WiFi.localIP().toString().c_str();
    //Serial.println("\n");

    //  if((int)batteryLevelPercent < 20)

    //pinMode(BUZZER, OUTPUT);   // BUZ ON
    pinMode (LED2, OUTPUT);
    digitalWrite (LED2, HIGH); //HIGH = ON //LOW = OFF
    //    int i = 5;
    //    while(i-- > 0)
    //    {
    //      digitalWrite(LED2, !digitalRead(LED2)); //toggle the LED
    //      digitalWrite(BUZZER, !(digitalRead(BUZZER))); delay(500);
    //    }
    //    pinMode(BUZZER, INPUT); // BUZ OFF
    //    digitalWrite (LED2, HIGH); //HIGH = ON //LOW = OFF
  }
  else
  { WiFi_Status = "DISCONNECTED";
    Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");
    // Serial.println("\n================= WIFI_ON : WiFi DISCONNECTED ==================\n");

    //pinMode (LED2, OUTPUT);
    //digitalWrite (LED2, LOW); //HIGH = ON //LOW = OFF
    //pinMode (LED2, INPUT);

    // SAVE DATA IN EEPROM & GO TO DEEP SLEEP
  }
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
