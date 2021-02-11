/*
  ccs811basic.ino - Demo sketch printing results of the CCS811 digital gas sensor for monitoring indoor air quality from ams.
  Created by Maarten Pennings 2017 Dec 11

  I2C device found at address 0x40  !
  I2C device found at address 0x5A  ! 90 // CCS811
  I2C device found at address 0xC0  !
  I2C device found at address 0xDA  !

  =============================================================================================================================================================================================

  Adafruit CCS811 Air Quality Sensor
  https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor?view=all
  https://github.com/adafruit/Adafruit_CCS811

  This sensor uses I2C address 0x5A

  The CCS811 has a configurable interrupt pin that
  can fire when a conversion is ready and/or when
  a reading crosses a user-settable threshold.

  The CCS811 supports multiple drive modes to take
  a measurement every 1 second, every 10 seconds,
  every 60 seconds, or every 250 milliseconds.

  The CCS811 has a 'standard' hot-plate MOX sensor,
  as well as a small microcontroller that controls
  power to the plate, reads the analog voltage, and
  provides an I2C interface to read from.

  This part will measure eCO2 (equivalent calculated
  carbon-dioxide) concentration within a range of
  400 to 8192 parts per million (ppm), and
  TVOC (Total Volatile Organic Compound)
  concentration within a range of 0 to 1187
  parts per billion (ppb).

  According to the fact sheet it can detect
  Alcohols, Aldehydes, Ketones, Organic Acids,
  Amines, Aliphatic and Aromatic Hydrocarbons.

  Please note, this sensor, like all VOC/gas sensors,
  has variability and to get precise measurements
  you will want to calibrate it against known sources!
  That said, for general environmental sensors, it
  will give you a good idea of trends and comparisons.

  AMS recommends that you run this sensor for 48 hours
  when you first receive it to "burn it in", and
  then 20 minutes in the desired mode every time the
  sensor is in use. This is because the sensitivity
  levels of the sensor will change during early use.

  Power Pins:
    Vin - this is the power pin. 3.3V
    GND - common ground for power and logic

  Logic pins:
    SCL - this is the I2C clock pin, connect to I2C clock line.
          There is a 10K pullup on this pin and it is level shifted so you can use 3 - 5VDC.
    SDA - this is the I2C data pin, connect to your microcontrollers I2C data line.
          There is a 10K pullup on this pin and it is level shifted so you can use 3 - 5VDC.
    INT - this is the interrupt-output pin.
          It is 3V logic and you can use it to detect
          when a new reading is ready or
          when a reading gets too high or too low.
    WAKE - this is the wakeup pin for the sensor.
          It needs to be pulled to ground in order
          to communicate with the sensor.
          This pin is level shifted so you can use 3-5VDC logic.
    RST - this is the reset pin. When it is pulled
          to ground the sensor resets itself.
          This pin is level shifted so you can use 3-5VDC logic.

  =============================================================================================================================================================================================

  Carbon dioxide in Earth's atmosphere is a trace gas, currently (start of 2020)
  having a global average concentration of 412 parts per million by volume
  (or 622 parts per million by mass)
  https://en.wikipedia.org/wiki/Carbon_dioxide
  https://upload.wikimedia.org/wikipedia/commons/3/35/Main_symptoms_of_carbon_dioxide_toxicity.svg


   CO2 LEVELS -
  https://www.kane.co.uk/knowledge-centre/what-are-safe-levels-of-co-and-co2-in-rooms

   What are safe levels of CO and CO2 in rooms?
  CO2

  250-350ppm  Normal background concentration in outdoor ambient air
  350-1,000ppm  Concentrations typical of occupied indoor spaces with good air exchange
  1,000-2,000ppm  Complaints of drowsiness and poor air.
  2,000-5,000 ppm   Headaches, sleepiness and stagnant, stale, stuffy air. Poor concentration, loss of attention, increased heart rate and slight nausea may also be present.
  5,000   Workplace exposure limit (as 8-hour TWA) in most jurisdictions.
  >40,000 ppm   Exposure may lead to serious oxygen deprivation resulting in permanent brain damage, coma, even death.


  ESP32 and CCS811 gas sensor example
  https://github.com/adafruit/Adafruit_CCS811
  http://www.esp32learning.com/code/esp32-and-ccs811-gas-sensor-example.php

  CCS811 driver for ESP-IDF (ESP32)
  https://github.com/gschorcht/ccs811-esp-idf

*/

// #if defined(ARDUINO_ARCH_ESP32)
// #endif

#include <Wire.h>    // I2C library


#include <ccs811.h>  // https://github.com/maarten-pennings/CCS811 // CCS811 library


// Wiring for ESP8266 NodeMCU boards: VDD to 3V3, GND to GND, SDA to D2, SCL to D1, nWAKE to D3 (or GND)
//CCS811 ccs811(D3); // nWAKE on D3

// Wiring for Nano: VDD to 3v3, GND to GND, SDA to A4, SCL to A5, nWAKE to 13
//CCS811 ccs811(13);

// nWAKE not controlled via Arduino host, so connect CCS811.nWAKE to GND
//CCS811 ccs811;

// Wiring for ESP32 NodeMCU boards:
// VDD to 3V3,
// GND to GND,
// SDA to 21,
// SCL to 22,
// nWAKE to GPIO15 (any GPIO)
// nWAKE -> PULLDOWN TO GND to ACTIVATE/WAKE, pass -1 as parameter to ccs811(-1) instead of ccs811(GPIO)
// Pin number connected to nWAKE (nWAKE can also be bound to GND, then pass -1),
// CCS811(int nwake=-1, int slaveaddr=CCS811_SLAVEADDR_0); // 0x5A[90] or 0x5B[91]
// CCS811 ccs811(-1, 0x5A);// nWAKE to GND, I2C address 0x5A specified
// CCS811 ccs811(-1, 0x5B);// nWAKE to GND, I2C address 0x5B specified
// CCS811 ccs811(15, 0x5A);// nWAKE on GPIO15, I2C address 0x5A specified
// CCS811 ccs811(15, 0x5B);// nWAKE on GPIO15, I2C address 0x5B specified
//CCS811 ccs811(15); // nWAKE on GPIO15, I2C default address 0x5A
CCS811 ccs811(-1); // pass -1 when nWAKE to GND, I2C default address 0x5A

unsigned int CO2_loop_counter = 0;
// bool CCS811_flag = true;
// uint16_t eCO2, tVOC; // estimated total Volatile Organic Compunds // errstat, raw;

int CO2_I2C_CCS811_setup()
{
  //  Serial.begin(115200); // Enable serial

  Serial.println("\nsetup: Starting CCS811 basic demo");
  Serial.print("setup: ccs811 lib version: "); Serial.println(CCS811_VERSION);

  Wire.begin();  // Enable I2C

  // Enable CCS811
  ccs811.set_i2cdelay(50); // Needed for ESP8266 because it doesn't handle I2C clock stretch correctly
  bool ok = ccs811.begin();
  if ( !ok )
  {
    Serial.println("setup: CCS811 begin FAILED");
    return -1;
  }

  // Print CCS811 versions
  Serial.print("setup: hardware    version: "); Serial.println(ccs811.hardware_version(), HEX);
  Serial.print("setup: bootloader  version: "); Serial.println(ccs811.bootloader_version(), HEX);
  Serial.print("setup: application version: "); Serial.println(ccs811.application_version(), HEX);

  // Start measuring
  ok = ccs811.start(CCS811_MODE_1SEC); // Start CCS811 (measure every 1 second)
  if ( !ok )
  {
    Serial.println("setup: CCS811 begin FAILED");
    //CCS811_flag = false; //
    return -1;
  }

  //  if (CCS811_flag)
  //  {
  int eCO2samples = 0, tVOCsamples = 0;
  int loop_count = 0; // , initialSamplesToIgnore = 3;
  eCO2 = tVOC = 0;
  CO2_loop_counter = 0;
  
  //  Serial.print(" eCO2=");  Serial.print(eCO2);     Serial.println(" ppm  ");
  //  Serial.print(" Total eVOC="); Serial.print(tVOC);    Serial.println(" ppb  ");
  //  Serial.print(" eCO2samples = ");  Serial.println(eCO2samples);
  //  Serial.print(" tVOCsamples = ");  Serial.println(tVOCsamples);
  //  Serial.print(" loop_count = "); Serial.println(loop_count);
  Serial.println("CCS811 -> CO2: BEGIN");

  unsigned long CCS811_loop_Duration = millis();
  //  for (int i = 0; i < 10; i++)
  while (loop_count < 5) // && eCO2 < 400)
  {

    CO2_I2C_CCS811_loop();



    // The equivalent CO2 (eCO2) output range for CCS811 is from 400ppm up to 29206ppm.
    // The equivalent Total Volatile Organic Compound (eTVOC)output range for CCS811 is from 0ppb up to 32768ppb.
    if ((eCO2 >= 400 && eCO2 < 10000) && (tVOC >= 0 && tVOC < 10000)) // (eCO2 || tVOC)
      //if ((eCO2 >= 400 && eCO2 <= 29206) && (tVOC <= 32768))
    {
      //if (loop_count >= initialSamplesToIgnore)

      eCO2samples += eCO2;
      tVOCsamples += tVOC;

      loop_count++;
      Serial.println("-------------------------------------------------------------------------------");
      Serial.println("CO2_I2C_CCS811_setup() -> CCS811: ");
      Serial.print(" eCO2 = ");  Serial.print(eCO2);     Serial.println(" ppm  ");
      Serial.print(" eVOC = "); Serial.print(tVOC);    Serial.println(" ppb  ");
      //      Serial.print("\n eCO2samples = ");  Serial.println(eCO2samples);
      //      Serial.print(" tVOCsamples = ");  Serial.println(tVOCsamples);
      Serial.print(" loop_count = "); Serial.println(loop_count);
      Serial.println("-------------------------------------------------------------------------------");
    }
    //delay(1000);
    //Serial.println("\n"+String(i)+" ");
    //    if(eCO2>0 && i>0)
    //    { Serial.print("\neCO2=");  Serial.print(eCO2);   Serial.println(" ppm");
    //      i=11;
    //    }
    //  eCO2 = 0;

    //break;
  }

  CCS811_loop_Duration = millis() - CCS811_loop_Duration;
  Serial.println("CCS811_loop_Duration: " + String(CCS811_loop_Duration) + "ms\n");

  Serial.print("\nTOTAL eCO2samples = ");  Serial.println(eCO2samples);
  Serial.print("TOTAL tVOCsamples = ");  Serial.println(tVOCsamples);
  Serial.print("TOTAL loop_count = "); Serial.println(loop_count);

  //  loop_count -= initialSamplesToIgnore;
  CO2 = (float)eCO2samples / loop_count;
  VOC = (float)tVOCsamples / loop_count;

  if(CO2 == 400)
  {
    int randN = (int) random(3,9);
    CO2+=(float)randN;
  }

  Serial.print("\n AVG. CO2 = ");  Serial.print(CO2);   Serial.println(" ppm");
  Serial.print(  " AVG. VOC = ");  Serial.print(VOC);   Serial.println(" ppb");

  //  if (CO2 > 0 && CO2 < 1000) Serial.println("NORMAL CO2 concentration...\n");
  //  if (CO2 > 1000 && CO2 < 2000) Serial.println("HIGH CO2 concentration... can cause drowsiness, poor concentration!\n");
  //  if (CO2 > 2000) Serial.println("DANGEROUSLY HIGH CO2 concentration... can cause loss of attention, Headaches, sleepiness, increased heart rate!!!\n");

  return 0;
}


void CO2_I2C_CCS811_loop()
{
  // Read
  uint16_t errstat, raw; // eCO2, tVOC,

  // ccs811.set_envdata210(tempC, humidity); // Pass environmental data (T, RH) to CCS811

  //unsigned long ccs811readDuration = millis();
  ccs811.read(&eCO2, &tVOC, &errstat, &raw);
  //ccs811readDuration = millis() - ccs811readDuration;
  //Serial.println("ccs811readDuration: " + String(ccs811readDuration) + "ms\n");
  //Serial.println("-------------------------------------------------------------------------------");

  // Print measurement results based on status
  if ( errstat == CCS811_ERRSTAT_OK )
  { //Serial.println(" CO2_I2C_CCS811_loop() -> ");
    Serial.println("CCS811: CCS811_ERRSTAT_OK ");
  }
  else if ( errstat == CCS811_ERRSTAT_OK_NODATA )
  { Serial.println("CCS811: waiting for (new) data");
  }
  else if ( errstat & CCS811_ERRSTAT_I2CFAIL )
  { Serial.println("CCS811: I2C error");
  }
  else
  { Serial.print("CCS811: errstat = "); Serial.println(errstat);
    Serial.print(errstat, HEX); Serial.print(" = ");
    Serial.println( ccs811.errstat_str(errstat) );
  }

//  Serial.println(" CO2_loop_counter = " + String(++CO2_loop_counter));
//  Serial.print(" eCO2 = ");  Serial.print(eCO2);     Serial.println(" ppm  ");
//  Serial.print(" tVOC = "); Serial.print(tVOC);    Serial.println(" ppb  "); // Total
//  Serial.println("-------------------------------------------------------------------------------");

  delay(1000); // Wait: 1s -> CCS811_MODE_1SEC
}

//Serial.println("-------------------------------------------------------------------------------");
//Serial.println(" CO2_I2C_CCS811_loop() -> CCS811: ");
//Serial.print(" eCO2=");  Serial.print(eCO2);     Serial.println(" ppm  ");
//Serial.print(" tVOC="); Serial.print(tVOC);    Serial.println(" ppb  "); // Total
//Serial.print("raw6=");  Serial.print(raw/1024); Serial.print(" uA  ");
//Serial.print("raw10="); Serial.print(raw%1024); Serial.print(" ADC  ");
//Serial.print("R="); Serial.print((1650*1000L/1023)*(raw%1024)/(raw/1024)); Serial.print(" ohm");
//Serial.println("-------------------------------------------------------------------------------");
