/*
 *  https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/
 *  https://lastminuteengineers.com/esp32-sleep-modes-power-consumption/
 *  https://lastminuteengineers.com/ds3231-rtc-arduino-tutorial/
  Deep Sleep with External Wake Up
  =====================================
  This code displays how to use deep sleep with
  an external trigger as a wake up source and how
  to store data in RTC memory to use it over reboots

  This code is under Public Domain License.

  Hardware Connections
  ======================
  Push Button to GPIO 34 pulled down with a 10K Ohm resistor

  NOTE:
  ======
  Only RTC IO can be used as a source for external wake source.
  They are pins: 0,2,4,12-15,25-27,32-36, 39.

  Author:
  Pranav Cherukupalli <cherukupallip@gmail.com>
*/

/*
#include "WiFi.h" 
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>

const int LED_pin = 2; //GPIO2 - ESP32

// ESP32 Wake-up Source : Timer
//#define uS_TO_S_FACTOR 1000000L  //Conversion factor for micro seconds to seconds
//#define TIME_TO_SLEEP_S  5         //Time ESP32 sleep duration (in seconds)
//#define TIME_TO_SLEEP_M  5 * 60L   //Time ESP32 sleep duration (in minutes)

//ESP32 Wake-up Source : EXT WAKE-UP
// BITMASK FOR GPIO32 : 01 00 00 00 00 hex
// BITMASK FOR GPIO35 : 08 00 00 00 00 hex
// BITMASK FOR GPIO36 : 10 00 00 00 00 hex
// BITMASK FOR GPIO39 : 80 00 00 00 00 hex
// GPIO39(MSBit)-GPIO32(LSBit) FORM THE 8Bits OF THE MOST SIGNIFICANT BYTE OF BUTTON_PIN_BITMASK
//#define BUTTON_PIN_BITMASK  0x100000000 // GPIO32: 2^32 in hex
//#define BUTTON_PIN_BITMASK  0x200000000 // GPIO33: 2^33 in hex // 8589934592 in decimal
#define BUTTON_PIN_BITMASK 0x400000000 // GPIO34: 2^34 in hex // 17179869184 in decimal
//#define BUTTON_PIN_BITMASK  0x800000000 // GPIO35: 2^35 in hex // 34359738368 in decimal
//#define BUTTON_PIN_BITMASK 0x1000000000 // GPIO36: 2^36 in hex
//#define BUTTON_PIN_BITMASK 0x8000000000 // GPIO39: 2^39 in hex

RTC_DATA_ATTR int bootCount = 0; // Stored in RTC memory

// ESP32 Wake-up Source : Touch Pad
//#define Threshold 40 //Define touch sensitivity. Greater the value, more the sensitivity.
//touch_pad_t touchPin; // Global variable for returning Touch Wake-Up pin
//void Touch_Pin_Wake_Up_Callback() //placeholder callback function
//{
//
//}


// Method to print the reason by which ESP32  has been awaken from sleep
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup()
{ pinMode(LED_pin, OUTPUT);
  digitalWrite (LED_pin, HIGH); //HIGH = ON //LOW = OFF

  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();


  // ESP32 Wake-up Source : Touch Pad
  //  print_wakeup_touchpad();  // Print wake-up due to which Touch Pin
  //  touchAttachInterrupt(T3, Touch_Pin_Wake_Up_Callback, Threshold); //Setup interrupt on Touch Pad 3 (GPIO15)
  //  esp_sleep_enable_touchpad_wakeup(); //Configure Touchpad as wakeup source


  // ESP32 Wake-up Source : Timer
  //  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_S * uS_TO_S_FACTOR); //Set timer to 5 seconds
  //  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP_S) + " Seconds");


//   Configuring ESP32 to wake up for an external trigger:
//     RTC GPIOs 0,2,4,12-15,25-27,32-36, 39
//
//    There are two types for ESP32, ext0 and ext1 .
//      ext0 – Use it when you want to wake-up the chip by one particular pin only.
//      ext0 uses RTC_IO to wakeup thus requires RTC peripherals to be on
//      ext0 - internal pullup or pulldown resistors can also be used (RTC peripherals stay turned on).
//    (use rtc_gpio_pullup_en() and rtc_gpio_pulldown_en() functions, before calling esp_sleep_start())
//
//      ext1 – Use it when you have several buttons for the wake-up.
//      ext1 uses RTC Controller so it doesn't need RTC peripherals & RTC memory to be powered on.
//      ext1 - internal pullup or pulldown resistors cannot be used

  //Configure GPIO34 as ext0 wake up source for LOW logic level
  //  esp_sleep_enable_ext0_wakeup(GPIO_NUM_34,0); //1 = High, 0 = Low

  //If you were to use ext1, you would use it like
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  //  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW); // GPIO32-39
  
//  digitalWrite (LED_pin, LOW); // the LED
//  digitalWrite (LED_pin, !digitalRead(LED_pin)); //toggle the LED

  //Go to sleep now
  Serial.println("Going to sleep now");
  // goToDeepSleep();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop()
{
  //This is not going to be called
}
*/


/*
  //Function that prints the touchpad by which ESP32 has been awaken from sleep
void print_wakeup_touchpad()
{
  touch_pad_t pin;
  touchPin = esp_sleep_get_touchpad_wakeup_status();
  switch(touchPin)
  {
    case 0  : Serial.println("Touch detected on GPIO 4"); break;
    case 1  : Serial.println("Touch detected on GPIO 0"); break;
    case 2  : Serial.println("Touch detected on GPIO 2"); break;
    case 3  : Serial.println("Touch detected on GPIO 15"); break;
    case 4  : Serial.println("Touch detected on GPIO 13"); break;
    case 5  : Serial.println("Touch detected on GPIO 12"); break;
    case 6  : Serial.println("Touch detected on GPIO 14"); break;
    case 7  : Serial.println("Touch detected on GPIO 27"); break;
    case 8  : Serial.println("Touch detected on GPIO 33"); break;
    case 9  : Serial.println("Touch detected on GPIO 32"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
}*/

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
