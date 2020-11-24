/*
  Communicate with BME280s with different I2C addresses
  Nathan Seidle @ SparkFun Electronics
  March 23, 2015
  
  This example shows how to connect sensors on the I2C bus.

  Hardware connections:
  BME280 -> Arduino
  GND   -> GND
  3.3   -> 3.3
  SDA   -> GPIO4 - A4/D2
  SCL   -> GPIO5 - A5/D1
*/

#include <Wire.h>
#include <SparkFunBME280.h>

BME280 bme; // = SensorAddr0x76;
//float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0;

void BME_setup()
{
//  Serial.begin(115200);
//  Serial.println("Example showing alternate I2C addresses");

  Wire.begin();
  //  BME280 has two I2C addresses: 0x76 (SDO to GND / jumper closed) 
  //  OR 0x77 (default I2C address, SDO FLOATING / jumper open)
  bme.setI2CAddress(0x76); // 0x77 // use I2C_scanner code to find this I2C Address

  if(bme.beginI2C() == false);   // Serial.println("Sensor BME280 connect failed");
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
  
//  delay(50);
//  printValues();
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
