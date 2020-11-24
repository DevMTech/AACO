// *****************************************************************************
// Arduino UNO Temperature and Humidity Monitor using Cypress                 **
// Solar Beacons and HM-10/HM-11 BLE board                                    **
// Software to accompany the article:                                         **
// "Build an Arduino multi-node BLE humidity and temperature sensor monitor"  **
// *****************************************************************************
// This software is offered strictly as is with no warranties whatsoever.     **
// Use it at your own risk.                                                   **
// *****************************************************************************
// SolarBeaconScannerB.ino
// Output to serial port AND LCD
//
// NOTES:
// Sensor IDs must be set for 1 (0001 - INDOOR) and (0002 - OUTDOOR)
//
// Tested with both HM10 and HM11 devices BUT...
// Software version of the HM BLE device must be 546
//
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Modify below for the pins you use
SoftwareSerial sSerial(2, 3); // RX=2, TX=3

// This configures my old LCD - yours may be much simpler
// see wiki for the New LiquidCrystal library
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// AT commands and responses for HM10/11
// add a AT+VERR? / HMSoft V546 to the list if desired
char* ATcmds[] = {"AT", "AT+RENEW", "AT+IMME1", "AT+ROLE1"};
char* ATresps[] = {"OK", "OK+RENEW", "OK+Set:1", "OK+Set:1"};

// ascii string from HM10/11
String response = "";
// fixed UUID string for solar beacon
String UUID = "0005000100001000800000805F9B0131";
// variables for string slices
String sliceP2, sID, sHUM, sTEM;
// Strings for conversion
char SsensorID[] = {"0000"};
char SsensorHUM[] = {"00"};
char SsensorTEM[] = {"00"};
// index into strings
int sliceS, sliceE, sIndex, eIndex, uIndex;
// ascii hex to ulong variables
unsigned long sensorID, sensorHUM, sensorTEM;
// final converted sensor variables
float Hum, Tem, Hum1, Tem1, Hum2, Tem2;
// strings for lcd display of sensor values
String lcd_hum1, lcd_hum2, lcd_tem1, lcd_tem2;


//-------------------------------------------

void setup()
{
  // Open serial communications
  Serial.begin(9600);
  lcd.begin(16, 2);      // initialize the lcd
  sSerial.begin(9600);   // initialize software serial
  ReadScan(3500); // This will effectively flush the sSerial buffer
  Serial.println("Solar Beacon Scanner (V1.0B)");
  print_lcd();
}
//-------------------------------------------

void loop()
{
  // Once we are sure that we have set up the HM device
  // we stay in a forever while loop

  // send the initial AT commands to the HM10
  if (SendCmd()) {
    Serial.println("Error sending AT commands");
    ReadScan(3500); // This will effectively flush the sSerial buffer
    delay(1000);
  }
  else {
    while (1) {
      // command below gets the scanned string
      sSerial.write("AT+DISI?");
      ReadScan(4000); // 4 seconds works well
      // uncomment the next line to print the entire response screen
      // Serial.println(response);
      if (sSerial.overflow()) {
        // process error here if you want - but did not occur with a 4 sec scan
        Serial.println("** SoftwareSerial overflow! **");
      }
      // SearchScan() will do all the work and return either
      // 0 - no sensor in the scan
      // 1 - sensor 1 found and values updated
      // 2 - sensor 2 found and values updated
      // 3 - sensors 1 & 2 found and values updated
      switch (SearchScan()) {
        case 0:
          // no sensor
          Serial.print(".");
          break;
        case 1:
          // print out updated values for sensor 1
          Serial.println();
          Serial.print(" Sensor ID=1 ");
          Serial.print(" Temperature (F)=");
          Serial.print(Tem1);
          Serial.print(" Humidity (RH%)=");
          Serial.println(Hum1);
          lcd_tem1 = String(Tem1, 1);
          if(lcd_tem1.length()>4) lcd_tem1.remove(4);
          lcd_hum1 = String(Hum1, 1);
          if(lcd_hum1.length()>4) lcd_hum1.remove(4);
          lcd.setCursor (5, 0);
          lcd.print(lcd_tem1);
          lcd.setCursor (12, 0);
          lcd.print(lcd_hum1);
          break;
        case 2:
          // print out updated values for sensor 2
          Serial.println();
          Serial.print(" Sensor ID=2 ");
          Serial.print(" Temperature (F)=");
          Serial.print(Tem2);
          Serial.print(" Humidity (RH%)=");
          Serial.println(Hum2);
          lcd_tem2 = String(Tem2, 1);
          if(lcd_tem2.length()>4) lcd_tem2.remove(4);
          lcd_hum2 = String(Hum2, 1);
          if(lcd_hum2.length()>4) lcd_hum2.remove(4);
          lcd.setCursor (5, 1);
          lcd.print(lcd_tem2);
          lcd.setCursor (12, 1);
          lcd.print(lcd_hum2);
          break;
        case 3:
          // print out updated values for sensor 1 and 2
          Serial.println();
          Serial.print(" Sensor ID=1 ");
          Serial.print(" Temperature (F)=");
          Serial.print(Tem1);
          Serial.print(" Humidity (RH%)=");
          Serial.print(Hum1);
          Serial.print(" Sensor ID=2 ");
          Serial.print(" Temperature (F)=");
          Serial.print(Tem2);
          Serial.print(" Humidity (RH%)=");
          Serial.println(Hum2);
          lcd_tem1 = String(Tem1, 1);
          if(lcd_tem1.length()>4) lcd_tem1.remove(4);
          lcd_hum1 = String(Hum1, 1);
          if(lcd_hum1.length()>4) lcd_hum1.remove(4);
          lcd.setCursor (5, 0);
          lcd.print(lcd_tem1);
          lcd.setCursor (12, 0);
          lcd.print(lcd_hum1);
          lcd_tem2 = String(Tem2, 1);
          if(lcd_tem2.length()>4) lcd_tem2.remove(4);
          lcd_hum2 = String(Hum2, 1);
          if(lcd_hum2.length()>4) lcd_hum2.remove(4);
          lcd.setCursor (5, 1);
          lcd.print(lcd_tem2);
          lcd.setCursor (12, 1);
          lcd.print(lcd_hum2);
          break;
        default:
          // error here
          Serial.println();
          Serial.println("** Unknown Sensor **");
          break;
      }
    }
  }
}

//-------------------------------------------
// Send the AT commands to set up the HM10/11
// must return the correct responses
// ok if 0 is returned, error otherwise
int SendCmd(void)
{
  int x, ecount;

  ecount = 0;
  for (x = 0; x < 4; x++) {
    sSerial.write(ATcmds[x]);
    Serial.print(ATcmds[x]);
    ReadScan(1500);
    Serial.print("->");
    Serial.println(response);
    if (response != ATresps[x]) {
      ecount++;
    }
  }
  // send ATDELO1 which does not have a response
  sSerial.write("ATDELO1");
  delay(500);
  return (ecount);
}
//-------------------------------------------
// Read the response string from the HM10/11
// stays for duration (seconds) to get a
// complete string

void ReadScan(unsigned long duration)
{
  char chr;
  response = "";
  // for timer
  unsigned long starttick = 0;
  unsigned long curtick = 0;

  // we stay here for a set amount of time reading the software serial
  starttick = millis();
  while ( ( (curtick = millis() ) - starttick) < duration ) {
    while (sSerial.available() != 0) {
      chr = sSerial.read();
      response.concat(chr);
    }
  }
}
//-------------------------------------------
// search the response string for correct
// UUID and get the sensor values if found

byte SearchScan()
{
  // search the response string for the UUID of the sensor
  // if found - set sensorN / TempN / HumN and keep searching
  // until the end of the string
  // return 0 - no sensor UUID in response
  //        1 - sensor 1 updated
  //        2 - sensor 2 updated
  //        3 - sensor 1 and 2 updated
  // update flag
  byte supdate = 0;
  if ((uIndex = response.indexOf(UUID)) == -1) {
    // sensor UUID was not found
    return (supdate);
  }
  // a sensor UUID was found and uIndex points to
  // the begining of the string - search for the next ':'
  if ((sliceS = response.indexOf(':', uIndex)) == -1) {
    // if no colon found assume error and dump the string
    return (supdate);
  }
  // get the next colon
  if ((sliceE = response.indexOf(':', sliceS + 1)) == -1) {
    // if no colon found assume error and dump the string
    return (supdate);
  }
  // get the substring bounded by the two colons
  sliceP2 = response.substring(sliceS + 1, sliceE);
  if (strlen(sliceP2 != 10)) {
    // P2 needs to be exactly 10 characters, if not, assume error and dump the string
    return (supdate);
  }
  // within sliceP2 we need to get the following ascii hex characters
  // into uints
  // 1,2,3,4 = sensor number
  // 5,6 = humidity
  // 7,8 = temperature
  // note: 9,10 are the rssi which we do not use
  // get the relevant characters into their own strings
  sID = sliceP2.substring(0, 4);
  sHUM = sliceP2.substring(4, 6);
  sTEM = sliceP2.substring(6, 8);
  // copy them to their constant char analogs
  sID.toCharArray(SsensorID, 5);
  sHUM.toCharArray(SsensorHUM, 3);
  sTEM.toCharArray(SsensorTEM, 3);
  // get hex ascii strings to unsigned longs
  sensorID = strtoul(SsensorID, NULL, 16);
  sensorHUM = strtoul(SsensorHUM, NULL, 16);
  sensorTEM = strtoul(SsensorTEM, NULL, 16);
  // convert humidity
  //RH[%] = 125 x (HUMIDITY x 256) / 65536 Ã¢â‚¬â€œ 6
  Hum = 125.0 * ((float)sensorHUM * 256.0) / 65536.0 - 6.0;
  // convert temperature
  //TEMP [Ã¢â€žÆ’] = 175.72 x (TEMP x 256) / 65536 Ã¢â‚¬â€œ 46.85
  Tem = 175.72 * ((float)sensorTEM * 256.0) / 65536.0 - 46.85;
  // comment out the next code line here and again later down
  // if you want celsius - leave it in if you want farenheit
  Tem = ((Tem * 9.0) / 5.0) + 32.0;

  // update the global vars for the sensor number
  // and flag the sensor number for an update
  if (sensorID == 1) {
    bitSet(supdate, 0);
    Hum1 = Hum;
    Tem1 = Tem;
  }
  if (sensorID == 2) {
    bitSet(supdate, 1);
    Hum2 = Hum;
    Tem2 = Tem;
  }
  else
  {
    // error catch here (likely sensorN value)
    // ignore the sensor update
    // alternatively you can uncomment the next line
    // and dump the whole string at this point
    //return(supdate);
  }
  // repeat the UUID search as above....
  // look for one more UUID in the string
  // since we already have the ending index past the found UUID
  // we can use it to start the search
  // note supdate is valid at this point
  if ((uIndex = response.indexOf(UUID, sliceE)) == -1) {
    // sensor UUID was not found
    return (supdate);
  }
  // a second sensor UUID was found and uIndex points to
  // the begining of the string
  // starting at the uIndex search for the next ':'
  if ((sliceS = response.indexOf(':', uIndex)) == -1) {
    // if no colon found assume error and dump the string
    return (supdate);
  }
  // get the next colon
  if ((sliceE = response.indexOf(':', sliceS + 1)) == -1) {
    // if no colon found assume error and dump the string
    return (supdate);
  }
  // get the substring bounded by the two colons
  sliceP2 = response.substring(sliceS + 1, sliceE);
  if (strlen(sliceP2 != 10)) {
    // P2 needs to be exactly 10 characters, if not, assume error and dump the string
    // but retain the first sensor reading which was valid
    return (supdate);
  }
  // within sliceP2 we get the following ascii hex characters into ints
  // 1,2,3,4 = sensor number
  // 5,6 = humidity
  // 7,8 = temperature
  // note: 9,10 are the rssi which we do not use
  // get the relevant characters into their own strings
  sID = sliceP2.substring(0, 4);
  sHUM = sliceP2.substring(4, 6);
  sTEM = sliceP2.substring(6, 8);
  // copy them to their constant char analogs
  sID.toCharArray(SsensorID, 5);
  sHUM.toCharArray(SsensorHUM, 3);
  sTEM.toCharArray(SsensorTEM, 3);
  // get hex ascii strings to unsigned longs
  sensorID = strtoul(SsensorID, NULL, 16);
  sensorHUM = strtoul(SsensorHUM, NULL, 16);
  sensorTEM = strtoul(SsensorTEM, NULL, 16);
  // convert humidity
  //RH[%] = 125 X (HUMIDITY X 256) / 65536 - 6
  Hum = 125.0 * ((float)sensorHUM * 256.0) / 65536.0 - 6.0;
  // convert temperature
  //TEMP [C] = 175.72 X (TEMP X 256) / 65536 - 46.85
  Tem = 175.72 * ((float)sensorTEM * 256.0) / 65536.0 - 46.85;
  // comment out the next code line
  // if you want celsius - leave it in if you want farenheit
  Tem = ((Tem * 9.0) / 5.0) + 32.0;
  // update the correct globals for the sensor
  // and flag the sensor number for an update
  if (sensorID == 1) {
    bitSet(supdate, 0);
    Hum1 = Hum;
    Tem1 = Tem;
    return (supdate);
  }
  if (sensorID == 2) {
    bitSet(supdate, 1);
    Hum2 = Hum;
    Tem2 = Tem;
    return (supdate);
  }
  else
  {
    // error catch here (likely sensorN value)
    // don't update anything
    // but retain the first and valid sensor update
    // alternatively uncomment the next line to
    // dump the entire string including the
    // valid update
    //supdate=0;
    return (supdate);
  }
}

void print_lcd()
{
  // initialize and draw the lcd screen
  lcd.home ();          // col 0 line 0
  lcd.print("S1 T=     H=    ");
  lcd.setCursor (0, 1); // col 0 line 1
  lcd.print("S2 T=     H=    ");
}



