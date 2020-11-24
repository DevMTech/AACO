/*
  Communicate with BME280s with different I2C addresses
  Nathan Seidle @ SparkFun Electronics
  March 23, 2015
  
  This example shows how to connect two sensors on the same I2C bus.

  Hardware connections:
  BME280 -> Arduino
  GND -> GND
  3.3 -> 3.3
  SCL -> GPIO5 - A5/D1
  SDA -> GPIO4 - A4/D2
  
*/

//#include <Wire.h>
#include <SparkFunBME280.h>

BME280 bme; // = SensorAddr0x76;
//float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0;

void BME_setup()
{
//  Serial.begin(115200);
//  Serial.println("I2C addresses");

//  Wire.begin(4,5); //   ESP12E-NODEMCU   SDA-D2-4, SCL-D1-5
  
//  Wire.begin(0,2); // FOR ESP01/ESP8266   SDA-0, SCL-2

  //  BME280 has two I2C addresses: 0x76 (SDO to GND / jumper closed) 
  //  OR 0x77 (default I2C address, SDO FLOATING / jumper open)
  bme.setI2CAddress(0x76);  // use I2C_scanner code to find this I2C Address
  // BLUE BME -> 0x76; 
//  SPARKFUN, SPI+I2C BME -> 0x77 
  if(bme.beginI2C() == false) Serial.println("Sensor B connect failed");
}

void BME_loop()
{
  readBME280Values();
}

void readBME280Values() 
{
  tempC = bme.readTempC();
  tempF = bme.readTempF();
  pressure = bme.readFloatPressure() / 100.0F;
  humidity = bme.readFloatHumidity();

//  delay(50);   printValues();
}

void printValues() 
{      
    Serial.print("Temperature = "); 
    Serial.print(bme.readTempC());
//    Serial.print(tempC);
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bme.readFloatPressure() / 100.0F);
//    Serial.print(pressure);
    Serial.println(" hPa");
      
    Serial.print("Humidity = ");
    Serial.print(bme.readFloatHumidity());
//    Serial.print(humidity);
    Serial.println(" %");

    Serial.println("Illumination: "+String(lux)+" Lux");
    Serial.println();
}

/*
    // https://randomnerdtutorials.com/esp8266-bme280-arduino-ide/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//#include <SPI.h>
//#define BME_SCK 14
//#define BME_MISO 12
//#define BME_MOSI 13
//#define BME_CS 15

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}

void loop() { 
  printValues();
  delay(delayTime);
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
//  Serial.print("Temperature = ");
//  Serial.print(1.8 * bme.readTemperature() + 32);
//  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
*/
