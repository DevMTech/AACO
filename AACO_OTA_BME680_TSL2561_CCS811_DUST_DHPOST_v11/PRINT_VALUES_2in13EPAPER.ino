/**/
// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Library: https://github.com/ZinggJM/GxEPD2

//DEFAULT VSPI PINS
//MOSI 23
//MISO 19
//SCK  18
//SS    5

//HELTEC SPI 2.13" EPD PIN CONNECTIONS
//VCC   3V3 (NOT 5V!)
//GND   GND
//D/C   2 // 12 -> ANY DIGITAL IO PIN WILL DO!
//SDI   23 (MOSI/DIN)
//CS    5(SS)/27 -> ANY DIGITAL IO PIN WILL DO!
//CLK   18 (SCK)
//BUSY  4 // 14 -> ANY DIGITAL IO PIN WILL DO!
//RST   N.C. - NOT PRESENT ON HELTEC 2.13" EPD

// mapping suggestion for ESP32, e.g. TTGO T8 ESP32-WROVER
// BUSY -> 4, RST -> 0, DC -> 2, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V
// for use with Board: "ESP32 Dev Module":



#ifdef EPAPER2in13

int EPAPER2in13_DELAY_1 = 100;

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

// https://github.com/ZinggJM/GxEPD2
#include <GxEPD2_3C.h> // 3-color e-papers //#include <GxEPD2_BW.h>


#if defined(ESP32)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)

GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(SS, 2, 0, 4)); // (SS/CS=5, DC=2, RST=0, BUSY=4)

//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(27, 12, 0, 14));
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c( SS, 17, 16, 4));
#endif

//#if defined(ESP32) && defined(ARDUINO_ESP32_DEV)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
// 3-color e-papers
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(27, 12, 0, 14)); // (SS/CS, DC, RST, BUSY)
//GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c( 5, 2, 0, 4));
//#endif
//#if defined(ESP8266) || defined(ESP32)
//#include "bitmaps/Bitmaps3c104x212.h" // 2.13" b/w/r
//#endif

#include <Fonts/FreeMonoBold9pt7b.h>
//Each filename starts with the face name (“FreeMono”, “FreeSerif”, etc.)
//followed by the style (“Bold”, “Oblique”, none, etc.),
//font size in points (currently 9, 12, 18 and 24 point sizes are provided) and
//“7b” to indicate that these contain 7-bit characters (ASCII codes “ ” through “~”);
//8-bit fonts (supporting symbols and/or international characters) are not yet provided but may come later.
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
// https://forum.arduino.cc/index.php?topic=578874.0


// #include <RTClib.h> // https://github.com/adafruit/RTClib


void EPAPER2in13_BLANK_SCREEN()
{
  display.init(115200);

  Serial.println("\n\t EPAPER2in13_BLANK_SCREEN()\n");
  display.setFullWindow();
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE); //(GxEPD_BLACK); //
  display.setTextColor(GxEPD_BLACK);
  //display.setFont(&FreeMonoBold9pt7b); //  display.setFont(f);
  display.setCursor(0, 0);

  display.firstPage();

do
{ 
//    display.println();  
//    display.println("");
//    display.println("");
//    display.println("");
//    display.println("");
//    display.println("");
} while (display.nextPage());

  delay(EPAPER2in13_DELAY_1);
}

void WIFI_HTTP_STATUS_EPAPER2in13()
{
  display.init(115200);

  Serial.println("\n\t WIFI_HTTP_STATUS_EPAPER2in13()\n");
  display.setFullWindow();
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE); // (GxEPD_BLACK); //
  display.setTextColor(GxEPD_BLACK); // (GxEPD_WHITE);//
  display.setFont(&FreeMonoBold9pt7b); //  display.setFont(f);
  display.setCursor(0, 0);

  //  display.println(text); // "customTextToPrint"

  display.firstPage();

  do
  { display.println();

    //    //    display.setTextColor(GxEPD_BLACK);
    //    display.print("WiFi   : ");
    //    //    display.setTextColor(GxEPD_RED);
    //    display.print(WiFi_Status);
    //    display.println("");

    //if (WiFi.status() == WL_CONNECTED)
    {
      //    display.setTextColor(GxEPD_BLACK);
      display.println("Local IP: ");
      //    display.setTextColor(GxEPD_RED);
      display.println(deviceIP);
      display.println("");
    }

    //    display.setTextColor(GxEPD_BLACK);
    display.print("HTTP POST: ");
    //    display.setTextColor(GxEPD_RED);
    display.print(HTTP_post_status);
    display.println("");

    //    display.setTextColor(GxEPD_BLACK);
    display.print("HTTP Respns: ");
    //    display.setTextColor(GxEPD_RED);
    display.print(httpCode);
    display.println("");


  } while (display.nextPage());

  delay(EPAPER2in13_DELAY_1); // delay(2000);
}

void WIFI_SSID_CONFIG_EPAPER2in13()
{ Serial.println("\n  ENTERING ->  WIFI_SSID_CONFIG_AP_EPAPER2in13 \n");

  display.init(115200);

  display.setFullWindow();
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b); //  display.setFont(f);
  display.setCursor(0, 0);

  //  display.println(text); // "customTextToPrint"

  display.firstPage();

  do
  { display.println();

    //    display.setTextColor(GxEPD_BLACK);
    display.println("To set WiFi");
    display.println("Connect to AP :");
    //    display.setTextColor(GxEPD_RED);
    //display.println("");

    //    display.setTextColor(GxEPD_BLACK);
    display.println(" ");
    display.setTextColor(GxEPD_RED);
    display.print(SOFT_AP_SSID);
    display.println("");

  } while (display.nextPage());

  delay(EPAPER2in13_DELAY_1); // delay(2000);
}

void EPAPER2in13PrintValues1()
{ display.init(115200);

  Serial.println("\n\t EPAPER2in13PrintValues1()\n");
  display.setFullWindow();
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b); //  display.setFont(f);
  display.setCursor(0, 0);

  //  display.println(text); // "customTextToPrint"

  display.firstPage();

  do
  {
    // Serial.println("\n\tEPAPER2in13PrintValues()\n");

    display.println();
    display.print("Temperature ");
    //display.setTextColor(GxEPD_RED);
    //display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
    //double val = 22.05;    String valString = String(val);
    display.print(tempC);
    display.println("*C");


    //display.setTextColor(GxEPD_BLACK);
    display.print("RelHumidity ");
    //display.setTextColor(GxEPD_RED);
    display.print(humidity); // (int)
    display.println("%");


    //display.setTextColor(GxEPD_BLACK);
    display.print("Light Int. ");
    //display.setTextColor(GxEPD_RED);
    //display.print(lux);
    display.print((int)lux);
    //floatToString =  String(lux, 2);
    //display.print(floatToString);
    display.println("Lux");


    // display.setTextColor(GxEPD_BLACK);
    display.print("CO2 Conc.  ");
    // display.setTextColor(GxEPD_RED);
    display.print(eCO2);
    display.println("PPM");

    //    display.setTextColor(GxEPD_BLACK);
    //    display.print("CO2 Conc. ");
    //    display.setTextColor(GxEPD_RED);
    //    display.print(CO2);
    //    display.println("PPM");

    // display.setTextColor(GxEPD_BLACK);
    display.print("PM2.5Dust  ");
    // display.print("PM2.5 "); // PM2.5
    // display.setTextColor(GxEPD_RED);
    display.print((int)pm_2point5);
    display.println("PPM ");

    //  display.setTextColor(GxEPD_BLACK);
    //  display.print(" PM10");
    //  display.setTextColor(GxEPD_RED);
    //  display.print(pm_10);
    //  display.println("PPM");

  } while (display.nextPage());

  delay(EPAPER2in13_DELAY_1); // delay(1000);
}


// EPAPER2in13PrintValues("customTextToPrint", &FreeMonoBold9pt7b); // function call
// void EPAPER2in13PrintValues(const char text[], const GFXfont* f) // function definition
void EPAPER2in13PrintValues2()
{ display.init(115200);

  Serial.println("\n\t EPAPER2in13PrintValues2()\n");
  display.setFullWindow();
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b); //  display.setFont(f);
  display.setCursor(0, 0);

  //  display.println(text); // "customTextToPrint"

  display.firstPage();

  do
  {
    // Serial.println("\n\tEPAPER2in13PrintValues()\n");
    display.println();

    //    display.setTextColor(GxEPD_BLACK);
    display.print("Pressure ");
    //    display.setTextColor(GxEPD_RED);
    display.print(pressure);
    display.println("hPa");

    //    display.setTextColor(GxEPD_BLACK);
    display.print("Altitude ");
    //    display.setTextColor(GxEPD_RED);
    display.print(alti);
    display.println("m");

    //    display.setTextColor(GxEPD_BLACK);
    display.print("Gas/VOC  ");
    //    display.setTextColor(GxEPD_RED);
    display.print(VOC);
    display.println("KOhms");

    //    display.setTextColor(GxEPD_BLACK);
    display.print("PM10Dust ");
    //    display.setTextColor(GxEPD_RED);
    display.print((int)pm_10);
    display.println("PPM");

    //    display.setTextColor(GxEPD_BLACK);
    //    display.print("Battery R ");
    //    display.setTextColor(GxEPD_RED);
    //    display.print(batteryRaw);
    //    display.println(" ");

    //    display.setTextColor(GxEPD_BLACK);
    display.print("Battery  ");
    //    display.setTextColor(GxEPD_RED);
    display.print(batteryLevel);
    display.println("V");

    //    display.setTextColor(GxEPD_BLACK);
    //    display.print("Battery % ");
    //    display.setTextColor(GxEPD_RED);
    //    display.print(batteryLevelPercent); // (int)
    //    display.println("%");

  } while (display.nextPage());

  delay(EPAPER2in13_DELAY_1); // delay(1000);
}



//void EPAPER2in13Setup()
//{
//  //  Serial.begin(115200); delay(100);
//
//  Serial.println("\n EPAPER2in13Setup()");
//
//  display.init(115200);
//  // first update should be full refresh
//
//  // EPAPER2in13PrintRTCtime(); delay(1000);
//  // EPAPER2in13PrintValues1(); // delay(1000);
//  // EPAPER2in13PrintValues2(); // delay(1000);
//
//  // helloWorldForDummies();     delay(1000);
//  // partial refresh mode can be used to full screen, if display panel hasFastPartialUpdate
//  // helloFullScreenPartialMode();   delay(1000);
//  // deepSleepTest();
//
//    Serial.print("MOSI ");  Serial.println(MOSI);
//    Serial.print("MISO ");  Serial.println(MISO);
//    Serial.print("SCK ");  Serial.println(SCK);
//    Serial.print("SS ");  Serial.println(SS);
//
//  Serial.println("EPAPER2in13 setup done");
//}

//void EPAPER2in13PrintRTCtime()
//{ Serial.println("\n\t EPAPER2in13PrintRTCtime()\n");
//  display.setFullWindow();
//  display.setRotation(1);
//
//  display.fillScreen(GxEPD_WHITE);
//  display.setTextColor(GxEPD_BLACK);
//
////  display.fillScreen(GxEPD_BLACK);
////  display.setTextColor(GxEPD_WHITE);
//
//  display.setFont(&FreeMonoBold9pt7b); //  display.setFont(f);
//  display.setCursor(0, 0);
//
//  //  display.println(text); // "customTextToPrint"
//
////  TimeNow();
//
//  display.firstPage();
//
//  do
//  {
//    // Serial.println("\n\tEPAPER2in13PrintValues()\n");
//
//    display.println();
//
//    DateTime now = rtc.now(); // https://github.com/adafruit/RTClib
//
////    timeStamp = String(now.hour(), DEC);
////    timeStamp = timeStamp + String(".") + String(now.minute(), DEC); // String(":") +
////    timeStamp = timeStamp + String(".") + String(now.second(), DEC); // ":" +
////    timeStamp = timeStamp + String("-") + String(now.day(), DEC);
////    timeStamp = timeStamp + String("-") + String(now.month(), DEC);
////    timeStamp = timeStamp + String("-") + String(now.year(), DEC);
////    Serial.println(String("\n\tTime Stamp : ")+timeStamp+String("\n"));
//
//    // RTC TIME
////    display.setTextColor(GxEPD_BLACK); //    display.setTextColor(GxEPD_WHITE);
//    display.print(" Time  ");
//    display.println(timeHMS);
////    display.print(now.hour(), DEC);
////    display.print(":");
////    display.print(now.minute(), DEC);
////    display.print(":");
////    display.println(now.second(), DEC);
//
//    display.print(" Date  ");
//    display.println(Date);
////    display.print(now.day(), DEC);
////    display.print("-");
////    display.print(now.month(), DEC);
////    display.print("-");
////    display.println(now.year(), DEC);
//
//    display.print(" Day   ");
//    display.println(daysOfTheWeek[now.dayOfTheWeek()]);
//  }
//  while (display.nextPage());
//}

#endif

/**/
