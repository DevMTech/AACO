/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas
  ----> http://www.adafruit.com/products/3660

Power Pins:
    Vin - this is the power pin. Since the sensor chip uses 3 VDC, we have included a voltage regulator on board that will take 3-5VDC and safely convert it down. To power the board, give it the same power as the logic level of your microcontroller - e.g. for a 5V micro like Arduino, use 5V
    3Vo - this is the 3.3V output from the voltage regulator, you can grab up to 100mA from this if you like
    GND - common ground for power and logic

SPI Logic pins:
All pins going into the breakout have level shifting circuitry to make them 3-5V logic level safe. Use whatever logic level is on Vin!
    SCK - This is the SPI Clock pin, its an input to the chip
    SDO - this is the Serial Data Out / Master In Slave Out pin, for data sent from the BME680 to your processor
    SDI - this is the Serial Data In / Master Out Slave In pin, for data sent from your processor to the BME680
    CS - this is the Chip Select pin, drop it low to start an SPI transaction. Its an input to the chip
If you want to connect multiple BME680's to one microcontroller, have them share the SDI, SDO and SCK pins. Then assign each one a unique CS pin.

I2C Logic pins:
    SCK - this is also the I2C clock pin, connect to your microcontrollers I2C clock line.
    SDI - this is also the I2C data pin, connect to your microcontrollers I2C data line.
Leave the other pins disconnected


  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  

Temperature is calculated in degrees C, you can convert this to F by using the classic F = C * 9/5 + 32 equation.

Pressure is returned in the SI units of Pascals. 100 Pascals = 1 hPa = 1 millibar. Often times barometric pressure is reported in millibar or inches-mercury. For future reference 1 pascal =0.000295333727 inches of mercury, or 1 inch Hg = 3386.39 Pascal. So if you take the pascal value of say 100734 and divide by 3386.39 you'll get 29.72 inches-Hg.

Humidity is returned in Relative Humidity %

Gas is returned as a resistance value in ohms. This value takes up to 30 minutes to stabilize! Once it stabilizes, you can use that as your baseline reading. Higher concentrations of VOC will make the resistance lower.

You can also calculate Altitude. However, you can only really do a good accurate job of calculating altitude if you know the hPa pressure at sea level for your location and day! The sensor is quite precise but if you do not have the data updated for the current day then it can be difficult to get more accurate than 10 meters.

this precision sensor from Bosch can measure humidity with ±3% accuracy, barometric pressure with ±1 hPa absolute accuracy, and temperature with ±1.0°C accuracy. Because pressure changes with altitude, and the pressure measurements are so good, you can also use it as an altimeter with  ±1 meter or better accuracy!
adafruit_products_3660_quarter_ORIG.jpg

The BME680 takes those sensors to the next step in that it contains a small MOX sensor. The heated metal oxide changes resistance based on the volatile organic compounds (VOC) in the air, so it can be used to detect gasses & alcohols such as Ethanol, Alcohol and Carbon Monoxide and perform air quality measurements. Note it will give you one resistance value, with overall VOC content, it cannot differentiate gasses or alcohols.

Please note, this sensor, like all VOC/gas sensors, has variability and to get precise measurements you will want to calibrate it against known sources! That said, for general environmental sensors, it will give you a good idea of trends and comparisons. We recommend that you run this sensor for 48 hours when you first receive it to "burn it in", and then 30 minutes in the desired mode every time the sensor is in use. This is because the sensitivity levels of the sensor will change during early use and the resistance will slowly rise over time as the MOX warms up to its baseline reading.

=============================================================================================================================================================================================
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
