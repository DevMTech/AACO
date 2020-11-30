/**************************************************************************/
/*  PN532 NFC/RFID breakout board
    @file     readMifare.pde
    @author   Adafruit Industries
	@license  BSD (see license.txt)

    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.
   
    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:
   
    - Authenticate block 4 (the first block of Sector 1) using
      the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
    - If authentication succeeds, we can then read any of the
      4 blocks in that sector (though only block 4 is read here)
	 
    If the card has a 7-byte UID it is probably a Mifare
    Ultralight card, and the 4 byte pages can be read directly.
    Page 4 is read by default since this is the first 'general-
    purpose' page on the tags.

This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Introdution

PN532 NFC RFID Module is a highly integrated transmission module for 
Near Field Communication at 13.56MHz. With the mode switch on board, 
you can change easily between I2C, SPI, and UART modes. In addition, 
it supports RFID reading and writing, and NFC function with Android 
phone, which makes it quite convenient for wireless connection. This 
module is equipped with two 3mm mounting holes, of which the small 
dimension makes it easy for using in your project!

Features

    Supports I2C, SPI and HSU (High Speed UART), easy to switch between these modes.
    Works in the NFC mode and RFID reader/writer mode.
    Maximum distance for communication: 3 cm.

Hardware Preparation

1 *  Arduino UNO/Mega 2560 board
1 * PN532 NFC Module
1 * Blank NFC card supporting the ISO14443A
1 * PC with Arduino software installed
Several jumper wires

Connect the module and the Uno board as shown below - SPI
PN532 Module   SPI - Arduino Uno/Mega 2560
    VCC         5V
    GND         GND
    SCK         D2
    MISO        D5
    MOSI        D3
    SS          D4

Connect the PN532 and the Uno/Mega2560 as shown below - I2C
PN532 Module   I2C - Uno/Mega2560
        VCC       5V
        GND       GND
        SDA       A4/ pin20 Mega2560
        SCL       A5 /pin21 Mega2560
        RST0      D3
        IRQ       D2    
    
http://osoyoo.com/2017/07/pn532-nfc-rfid-module-for-arduino/
*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a software SPI connection (recommended):
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);              // SPI

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);                                 // I2C

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif

void setup(void) {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
	  
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      Serial.println("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	  
	  // Start with block 4 (the first block of sector 1) since sector 0
	  // contains the manufacturer data and it's probably better just
	  // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
	  
      if (success)
      {
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
		
        // If you want to write something to block 4 to test with, uncomment
		// the following line and this text should be read back in a minute
        //memcpy(data, (const uint8_t[]){ 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0 }, sizeof data);
        // success = nfc.mifareclassic_WriteDataBlock (4, data);

        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
		
        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          Serial.println("");
		  
          // Wait a bit before reading the card again
          delay(1000);
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
	  
      // Try to read the first general-purpose user page (#4)
      Serial.println("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        nfc.PrintHexChar(data, 4);
        Serial.println("");
		
        // Wait a bit before reading the card again
        delay(1000);
      }
      else
      {
        Serial.println("Ooops ... unable to read the requested page!?");
      }
    }
  }
}

/*
Found an ISO14443A card
  UID Length: 4 bytes
  UID Value: 0xF9 0xBE 0xD9 0x2B

Seems to be a Mifare Classic card (4 byte UID)
Trying to authenticate block 4 with default KEYA value
Sector 1 (Blocks 4..7) has been authenticated
Reading Block 4:
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

Found an ISO14443A card
  UID Length: 4 bytes
  UID Value: 0xF9 0x1A 0xAD 0x55

Seems to be a Mifare Classic card (4 byte UID)
Trying to authenticate block 4 with default KEYA value
Sector 1 (Blocks 4..7) has been authenticated
Reading Block 4:
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

Found an ISO14443A card
  UID Length: 4 bytes
  UID Value: 0x2C 0x61 0xD7 0x73

Seems to be a Mifare Classic card (4 byte UID)
Trying to authenticate block 4 with default KEYA value
Sector 1 (Blocks 4..7) has been authenticated
Reading Block 4:
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

Found an ISO14443A card
  UID Length: 4 bytes
  UID Value: 0xB4 0xF3 0xA1 0x1E

Seems to be a Mifare Classic card (4 byte UID)
Trying to authenticate block 4 with default KEYA value
Sector 1 (Blocks 4..7) has been authenticated
Reading Block 4:
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

*/
