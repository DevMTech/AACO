/* Original fonts included in library:
 *   ArialMT_Plain_10
 *   ArialMT_Plain_16
 *   ArialMT_Plain_24
 */
/* modified font created at http://oleddisplay.squix.ch/ */
/* #include "modified_font.h"*/

//#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
/*  SSD1306 */
#include "SSD1306Wire.h" 
#define SDA_PIN 5 //ESP32 //   21// GPIO21 -> SDA ESP8266
#define SCL_PIN 4 //ESP32 //   22// GPIO22 -> SCL ESP8266
#define SSD_ADDRESS 0x3c

SSD1306Wire  display(SSD_ADDRESS, SDA_PIN, SCL_PIN);

void SSD1306_OLED_setup() 
{
  display.init();

  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
//  display.setFont(Open_Sans_Condensed_Light_20); // set a font

  display.clear();   // clear the display
  display.drawString(0, 0,  "*** BUTTON  ***");
  display.drawString(0, 32, " NOT PRESSED ");
  display.display();   // write the buffer to the display
  delay(10);
}

void SSD1306_OLED_display() 
{
  display.clear();   // clear the display
  display.drawString(0, 0,  "*** BUTTON  ***");
  display.drawString(0, 32, "      PRESSED");
  display.display();   // write the buffer to the display
  delay(10);
}
