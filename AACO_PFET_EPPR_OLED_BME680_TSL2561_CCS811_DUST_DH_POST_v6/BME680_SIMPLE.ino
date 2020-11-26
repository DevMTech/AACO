/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

BME680
Gas sensor measuring relative humidity, barometric pressure, ambient temperature and gas (VOC).   
  https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors-bme680/
  https://www.seeedstudio.com/blog/2019/11/01/getting-started-with-bme280-humidity-pressure-and-temperature-sensor/
https://www.letscontrolit.com/forum/viewtopic.php?t=5241

3.6V Vmax
Pressure: 300...1100 hPa
Humidity 0…100%
Temperature: -40…85°C 

Possible use cases:

    Personal air quality tracker
    Air quality mapping
    Air quality inside cars & public transport
    Enhanced context awareness
    Accurate step & calorie tracker
    Quick GPS-fix & improved navigation
    Indicator of too high / low humidity
    Air quality & well-being indicator
    Sleep / recovery tracker
    Weather trend
    Stair counter
    Floor level detection
 ***************************************************************************/

//#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme; // I2C

//#include <SPI.h>
//#define BME_SCK  13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS   10
////Adafruit_BME680 bme(BME_CS); // hardware SPI
////Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void BME680_Simple_setup()
{ //  Serial.begin(9600); //  while (!Serial);
  DEBUG_Sprintln(F("BME680 test"));

  int count = 0;
  while (count++ < 10)
  {
    if (!bme.begin())
    { DEBUG_Sprintln("Could not find a valid BME680 sensor, check wiring!"); //    while (1);
    }
    else
    { DEBUG_Sprintln(" BME680 sensor begins!"); 
      break;
    }
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void BME680_Simple_loop()
{
  if (! bme.performReading())
  {
    DEBUG_Sprintln("Failed to perform reading :(");
    return;
  }

  tempC = bme.temperature;
  tempF = (tempC * 18 / 5) + 32;
  pressure = bme.pressure / 100.0F;
  alti = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity = bme.humidity;
  VOC = bme.gas_resistance / 1000.0F; // volatile organic compounds

  DEBUG_Sprint("\nBME680_Temperature = ");  DEBUG_Sprint(tempC);  DEBUG_Sprintln(" *C");
  DEBUG_Sprint("BME680_Humidity = ");  DEBUG_Sprint(humidity);  DEBUG_Sprintln(" %");
  DEBUG_Sprint("BME680_Pressure = ");  DEBUG_Sprint(pressure);  DEBUG_Sprintln(" hPa");  
  DEBUG_Sprint("BME680_VOC_Gas = ");  DEBUG_Sprint(VOC);  DEBUG_Sprintln(" KOhms"); //  DEBUG_Sprint(bme.gas_resistance);  DEBUG_Sprintln(" Ohms");
  DEBUG_Sprint("BME680_Approx. Altitude = ");  DEBUG_Sprint(alti);  DEBUG_Sprintln(" m");

  //  DEBUG_Sprint("Temperature = ");  DEBUG_Sprint(bme.temperature);  DEBUG_Sprintln(" *C");
  //  DEBUG_Sprint("Pressure = ");  DEBUG_Sprint(bme.pressure / 100.0);  DEBUG_Sprintln(" hPa");
  //  DEBUG_Sprint("Humidity = ");  DEBUG_Sprint(bme.humidity);  DEBUG_Sprintln(" %");
  //  DEBUG_Sprint("Gas = ");  DEBUG_Sprint(bme.gas_resistance / 1000.0);  DEBUG_Sprintln(" KOhms"); //  DEBUG_Sprint(bme.gas_resistance);  DEBUG_Sprintln(" Ohms");
  //  DEBUG_Sprint("Approx. Altitude = ");  DEBUG_Sprint(bme.readAltitude(SEALEVELPRESSURE_HPA));  DEBUG_Sprintln(" m");

  DEBUG_Sprintln();
  delay(200);
}
