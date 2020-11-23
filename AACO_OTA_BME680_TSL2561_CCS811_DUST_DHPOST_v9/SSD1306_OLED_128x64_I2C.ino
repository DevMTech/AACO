/*********

  I2C SSD1306 WIRING
  -------------------------------------------
    Pin1: GND -> GND
    Pin2: VCC -> VCC (3.3V~5V)
    Pin3: D0 -> SCL (22)(Pullup to 5V)       22
    Pin4: D1 -> SDA (21)(Pullup to 5V)       21
    Pin5: RES -> RESET ->                 3V3/VCC [optionally 10K to VCC, 0.1uF to GND]
    Pin6: DC -> (DATA/COMMAND)             GND
    Pin7: CS -> (CHIP SELECT)              NC

  IIC R1, R4, R6, R7 and R8(SHORTED) need to be connected
  SPI(4-WIRE) R3, R4
  SPI(3-WIRE) R2, R3

  CONVERT 4-WIRE SPI TO I2C:
    Move the resistor from position R3 to R1.
    Connect R8(SHORT).

 ********/

 

/**/ // REMOVE THIS TO ACTIVATE/DEACTIVATE SSD1306 OLED CODE


#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h> // https://github.com/adafruit/Adafruit-GFX-Library
// ADAFRUIT GFX DEPENDANCY: ADAFRUIT BUSIO -> https://github.com/adafruit/Adafruit_BusIO
#include <Fonts/FreeSerif12pt7b.h> // https://github.com/adafruit/Adafruit-GFX-Library

#include <Adafruit_SSD1306.h> // https://github.com/adafruit/Adafruit_SSD1306


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CDAC_LOGO_HEIGHT   64
#define CDAC_LOGO_WIDTH    128

int OLED_DELAY_1 = 2000;
int OLED_DELAY_2 = 1000;

static const unsigned char PROGMEM CDAC_logo_bmp[] = //CDAC LOGO
{ // 'CDAC LOGO 1', 128x64px
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x9f, 0xff, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xef, 0xff, 0x87, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xd8, 0x77, 0xff, 0x83, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xb7, 0xff, 0xf1, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xb7, 0xff, 0xfd, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xb7, 0xff, 0xfc, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xb7, 0xff, 0xfe, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x30, 0x31, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfd, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x03, 0x80, 0x30, 0x31, 0xff, 0xfe, 0x0f, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfc, 0xbf, 0xb7, 0xb7, 0xff, 0xfe, 0x0f, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0xdf, 0xb7, 0xb7, 0xff, 0xfe, 0x0f, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0xdf, 0xb7, 0xb7, 0xfe, 0x00, 0x0f, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xb7, 0x87, 0xf8, 0x00, 0x0f, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0xdf, 0xb7, 0xff, 0xf0, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfc, 0xdf, 0xb7, 0x87, 0xf0, 0x00, 0x0f, 0xc0, 0xff, 0xe0, 0x7f, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x03, 0xb0, 0x37, 0xff, 0xf0, 0xff, 0xff, 0xc1, 0xff, 0xf8, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0xff, 0x77, 0xf7, 0x87, 0xf0, 0xff, 0xff, 0x83, 0xff, 0xf8, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x11, 0xf0, 0x37, 0xff, 0xf0, 0x00, 0x1f, 0x83, 0xe1, 0xfc, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xdf, 0xff, 0xb7, 0x87, 0xf0, 0x00, 0x03, 0x83, 0xff, 0xfc, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xdb, 0xff, 0xb7, 0xff, 0xfc, 0x00, 0x01, 0x83, 0xe1, 0xfc, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xdb, 0xff, 0xb7, 0x87, 0xff, 0xff, 0xc1, 0xc1, 0xff, 0xf8, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xed, 0xff, 0xb7, 0xb7, 0xff, 0xff, 0xc1, 0xc0, 0x00, 0xf8, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xe6, 0x0f, 0xb7, 0xb7, 0xf1, 0xff, 0xc1, 0xe0, 0x00, 0xc0, 0x7f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf3, 0xef, 0xb7, 0xb7, 0xe0, 0x1e, 0x01, 0xf0, 0x00, 0xc0, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf8, 0xef, 0xb7, 0xb7, 0xe0, 0x00, 0x03, 0xfe, 0x00, 0xc1, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x8f, 0x87, 0x87, 0xf0, 0x00, 0x0f, 0xff, 0xe0, 0xc7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x30, 0x07, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x00, 0x30, 0x00, 0x7f, 0xfe, 0x00, 0x0f, 0xf0, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xe3, 0xff, 0x30, 0x00, 0x1f, 0xf8, 0x00, 0x0f, 0xc0, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x8f, 0xff, 0x30, 0x00, 0x0f, 0xf0, 0x00, 0x0f, 0x80, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x9c, 0x00, 0x30, 0x00, 0x07, 0xe0, 0x00, 0x0f, 0x00, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x30, 0xff, 0xff, 0xfc, 0x03, 0xc0, 0x1f, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x63, 0xff, 0xff, 0xff, 0x03, 0xc0, 0xff, 0x0e, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x67, 0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xcf, 0xff, 0xf0, 0xff, 0xc1, 0x83, 0xff, 0x0c, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xdf, 0xff, 0xff, 0xff, 0xc1, 0x83, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xdf, 0xff, 0xff, 0xff, 0xc1, 0x83, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xdf, 0xff, 0xf0, 0xff, 0xc1, 0x83, 0xff, 0x0c, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xdf, 0xff, 0xff, 0xff, 0xc1, 0x83, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfc, 0xcf, 0xff, 0xf0, 0xff, 0xc1, 0x83, 0xff, 0x0c, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0xcf, 0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xfe, 0x67, 0xff, 0xff, 0xff, 0x03, 0xc0, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x71, 0xff, 0xf0, 0xfe, 0x03, 0xc0, 0x7f, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x38, 0x00, 0x30, 0x00, 0x07, 0xe0, 0x00, 0x0f, 0x00, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0x9f, 0xff, 0x30, 0x00, 0x0f, 0xf0, 0x00, 0x0f, 0x80, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc7, 0xff, 0x30, 0x00, 0x1f, 0xf8, 0x00, 0x0f, 0xc0, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x00, 0x30, 0x00, 0x3f, 0xfc, 0x00, 0x0f, 0xe0, 0x00, 0x3f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x30, 0x01, 0xff, 0xff, 0x80, 0x0f, 0xfc, 0x00, 0x3f, 0xff, 0xff
};



void SSD1306_128x64_setup()
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    DEBUG_Sprintln(F("SSD1306 allocation failed"));
    //BEWARE BLOCKING LOOP AHEAD !!!!!!
    //    for (;;); // Don't proceed, loop forever
  }
  //  else {}

  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(WHITE);
  //DRAW_BITMAP_LOGO(); // CDAC logo
  //display.display();
  //delay(1000);

  // display.display() is NOT necessary after every single drawing command, rather, you can
  // batch up a bunch of drawing operations and update the screen by calling display.display().

  // print_PARAMS();
  Serial.println("\nSSD1306_128x64_setup");
}

//void SSD1306_128x64_loop()
//{
//  //  print_BATTERY_PARAMS();
//  print_PARAMS();
//  //  DRAW_BITMAP_LOGO();  //  testScrollText(1);  //  print_DHT();
//}


void DRAW_BITMAP_LOGO() // http://javl.github.io/image2cpp/
{
  display.clearDisplay(); // clear display
  // Display bitmap
  display.drawBitmap(0, 0,  CDAC_logo_bmp, CDAC_LOGO_WIDTH, CDAC_LOGO_HEIGHT, BLACK, WHITE);
  // (x-coordinate, y-coordinate, bitmap array, width, height, color)
  display.display();
  delay(OLED_DELAY_2);
  display.clearDisplay(); // clear display
}

void WIFI_HTTP_STATUS_OLED()
{
  display.clearDisplay(); // clear display
  // delay(100);
  // display WiFi_Status
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("WiFi");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(WiFi_Status);
  //  display.print(" lx");

  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("HTTP POST");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(HTTP_post_status);

  display.display();
  delay(OLED_DELAY_1);
  display.clearDisplay(); // clear display

  if (WiFi.status() == WL_CONNECTED)
  {
    //    display.setTextSize(2);
    //    display.setCursor(0, 0);
    //    display.print(deviceIP);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Local IP");;
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(deviceIP);;

    display.display();
    delay(OLED_DELAY_1);
    display.clearDisplay(); // clear display


  }

}

void TEST_OLED()
{
  display.clearDisplay(); // clear display
  delay(100);
  // display WiFi_Status
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("TEST-WiFi");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(WiFi_Status);
  //  display.print(" lx");

  display.display();
  delay(OLED_DELAY_2);
  display.clearDisplay(); // clear display
}

void print_PARAMS()
{
  // display.clearDisplay(); // clear display
  //  DRAW_BITMAP_LOGO();  delay(1000);
  display.clearDisplay(); // clear display
  delay(100);
  // display WiFi_Status
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("BOOT COUNT");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(bootCount);
  //  display.print(" lx");

  display.display();
  delay(OLED_DELAY_2);
  display.clearDisplay(); // clear display

  //  static float t = 0.00, h = 0.00;
  //  SHOW_TIME();  delay(500);
  //  SHOW_GMT_TIME();
  //  DRAW_BITMAP_LOGO();  display.clearDisplay(); // clear display
  //------------------------------------------------------------------------------------------------------------------------------------------//


  // display temperature
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(tempC);
  display.print(" ");
  display.setTextSize(1);

  // To display the º symbol, we use the Code Page 437 font.
  // For that, you need to set the cp437 to true as follows:
  display.cp437(true);  // https://www.ascii-codes.com/

  // Use the write() method to display your chosen character.
  // The º symbol corresponds to character 167.
  display.write(167);

  display.setTextSize(2);
  display.print("C");

  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(humidity);
  display.print(" %");

  display.display();
  delay(OLED_DELAY_1);
  display.clearDisplay(); // clear display
  //------------------------------------------------------------------------------------------------------------------------------------------//

  
//    // display temperature
//    display.setTextSize(1);
//    display.setCursor(0, 0);
//    display.print("HDCTempertr: ");
//    display.setTextSize(2);
//    display.setCursor(0, 10);
//    display.print(HDC1080_temperature);
//    display.print(" ");
//    display.setTextSize(1);
//
//    // To display the º symbol, we use the Code Page 437 font.
//    // For that, you need to set the cp437 to true as follows:
//    display.cp437(true);  // https://www.ascii-codes.com/
//
//    // Use the write() method to display your chosen character.
//    // The º symbol corresponds to character 167.
//    display.write(167);
//
//    display.setTextSize(2);
//    display.print("C");
//
//    // display humidity
//    display.setTextSize(1);
//    display.setCursor(0, 35);
//    display.print("HDCHumidity: ");
//    display.setTextSize(2);
//    display.setCursor(0, 45);
//    display.print(HDC1080_humidity);
//    display.print(" %");
//
//    display.display();
//    delay(OLED_DELAY_1);
//    display.clearDisplay(); // clear display
//    //------------------------------------------------------------------------------------------------------------------------------------------//
//
//
//    // display pressure
//    display.setTextSize(1);
//    display.setCursor(0, 0);
//    display.print("Pressure:");
//    display.setTextSize(2);
//    display.setCursor(0, 10);
//    display.print(pressure);
//    display.print(" hPa");
//
//    // display Approx. Altitude
//    display.setTextSize(1);
//    display.setCursor(0, 35);
//    display.print("Altitude:");
//    display.setTextSize(2);
//    display.setCursor(0, 45);
//    display.print(alti);
//    display.print(" m");
//
//    display.display();
//    delay(OLED_DELAY_1);
//    display.clearDisplay(); // clear display
//    //------------------------------------------------------------------------------------------------------------------------------------------//
//
//    // display PPM 2.5
//    display.setTextSize(1);
//    display.setCursor(0, 0);
//    display.print("PPM 2.5:");
//    display.setTextSize(2);
//    display.setCursor(0, 10);
//    display.print(pm_2point5);
//    display.print("PPM");
//
//    // display PPM 10
//    display.setTextSize(1);
//    display.setCursor(0, 35);
//    display.print("PPM 10:");
//    display.setTextSize(2);
//    display.setCursor(0, 45);
//    display.print(pm_10);
//    display.print("PPM");
//
//    display.display();
//    delay(OLED_DELAY_1);
//    display.clearDisplay(); // clear display
//    //------------------------------------------------------------------------------------------------------------------------------------------//
//
//    // display CO2
//    display.setTextSize(1);
//    display.setCursor(0, 0);
//    display.print("CO2");
//    display.setTextSize(2);
//    display.setCursor(0, 10);
//    display.print(eCO2);
//    //  display.print(CO2);
//    display.print(" PPM");
//
//    // display VOC
//    display.setTextSize(1);
//    display.setCursor(0, 35);
//    display.print("Gas/VOC:");
//    display.setTextSize(2);
//    display.setCursor(0, 45);
//    display.print(VOC);
//    display.print(" KOhms");
//
//    display.display();
//    delay(OLED_DELAY_1);
//    display.clearDisplay(); // clear display
  

  
  //------------------------------------------------------------------------------------------------------------------------------------------//

  // display LUX
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Illminance");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(lux);
  display.print(" lux");

  //  display.display();
  //  delay(2000);
  //  display.clearDisplay(); // clear display

  //------------------------------------------------------------------------------------------------------------------------------------------//

  // display Battery Raw
  //  display.setTextSize(1);
  //  display.setCursor(0, 0);
  //  display.print("Batt. Raw");
  //  display.setTextSize(2);
  //  display.setCursor(0, 10);
  //  display.print(batteryRaw);
  //  display.setTextSize(1);
  //  display.print(" /4095");

  // display Battery V
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Batt. V");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(batteryLevel);
  display.print(" V");

  display.display();
  delay(OLED_DELAY_1);
  display.clearDisplay(); // clear display

  //  // display Latitude
  //  display.setTextSize(1);
  //  display.setCursor(0, 0); // COL, ROW
  //  display.print("Latitude:");
  //  display.setTextSize(2);
  //  display.setCursor(0, 10); // COL, ROW
  //  display.print(GPSData.GPS_lat);
  //  display.print(" ");
  //  display.setTextSize(1);
  //  display.cp437(true);
  //  display.write(167);
  //
  //
  //  // display Longitude
  //  display.setTextSize(1);
  //  display.setCursor(0, 35);
  //  display.print("Longitude:");
  //  display.setTextSize(2);
  //  display.setCursor(0, 45);
  //  display.print(GPSData.GPS_lng);
  //  display.print(" ");
  //  display.setTextSize(1);
  //  display.cp437(true);
  //  display.write(167);

  //  display.display();
  //  delay(2000);
  //  display.clearDisplay(); // clear display

  // delay(500);
  // display.clearDisplay(); // clear display
}

void BLANK_SCREEN()
{
  display.clearDisplay(); // clear display
  display.setTextSize(1);
  //display.setCursor(0, 35);
  display.print("   ");
  //display.setTextSize(2);
  display.setCursor(0, 45);
  //display.print();
  display.print("    ");

  display.display();
  delay(20);
  display.clearDisplay(); // clear display
  delay(20);
}

//void SHOW_TIME()
//{
//  DateTime now = rtc.now();
//
//  display.clearDisplay();
//  display.setTextSize(4);
//  display.setCursor(0, 0);
//  display.print(" RTC");
//  display.setTextSize(2);
//  display.setCursor(0, 40);
//  display.print("   TIME");
//  display.display();
//  delay(OLED_DELAY_2);
//
//  display.clearDisplay();
//  display.setTextSize(2);
//  display.setCursor(0, 0);
//  display.print(now.hour(), DEC);
//  display.print(":");
//  display.print(now.minute(), DEC);
//  display.print(":");
//  display.println(now.second(), DEC);
//
//  display.setTextSize(2);
//  display.setCursor(0, 20);
//  display.print(now.day(), DEC);
//  display.print("-");
//
//  //  display.setTextSize(1);
//  //  display.setCursor(40, 20);
//  display.print(now.month(), DEC);
//
//  //  display.setTextSize(1);
//  //  display.setCursor(55, 20);
//  display.print("-");
//
//  //  display.setTextSize(1);
//  //  display.setCursor(70, 20);
//  display.println(now.year(), DEC);
//
//  display.setTextSize(2);
//  display.setCursor(0, 40);
//  display.println(daysOfTheWeek[now.dayOfTheWeek()]);
//
//  display.display();
//  delay(OLED_DELAY_1);
//  display.clearDisplay();
//}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*


  void print_BATTERY_PARAMS()
  { static float t = 0.00, h = 0.00;
  display.clearDisplay(); // clear display

  // BATTERY RAW
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Bat.Raw: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(batteryRaw);
  display.print("");

  // BATTERY LEVEL
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Bat.Lev: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(batteryLevel);
  display.print("V");

  display.display();
  delay(2000);
  display.clearDisplay(); // clear display

  // BATTERY PERCENTAGE
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Bat.Per: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(batteryLevelPercent);
  display.print("%");

  display.display();
  delay(2000);
  display.clearDisplay(); // clear display
  }

  void SHOW_GMT_TIME()
  {
  display.clearDisplay();
  display.setTextSize(4);
  display.setCursor(0, 0);
  display.print(" GMT");
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("   TIME");
  display.display();
  delay(500);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(GPSData.hh);
  display.print(":");
  display.print(GPSData.mm);
  display.print(":");
  display.println(GPSData.ss);


  display.setTextSize(1);
  display.setCursor(0, 20);
  DateTime now = rtc.now(); // https://github.com/adafruit/RTClib
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);

  display.setTextSize(2);
  //  display.setCursor(25, 40);
  display.setCursor(0, 40);
  display.print(GPSData.DD);
  display.print("-");

  //  display.setTextSize(1);
  //  display.setCursor(40, 40);
  display.print(GPSData.MM);

  //  display.setTextSize(1);
  //  display.setCursor(55, 40);
  display.print("-");

  //  display.setTextSize(1);
  //  display.setCursor(70, 40);
  display.print(GPSData.YY);

  display.display();
  delay(2000);
  }
/**/
