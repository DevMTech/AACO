
/*  This example shows how to connect sensors on the I2C bus.

  Hardware connections:
  TSL -> Arduino
  3.3   -> 3.3
  GND   -> GND
  SCL   -> GPIO5 - A5/D1
  SDA   -> GPIO4 - A4/D2
  
*/
#include <Wire.h>
#include <SparkFunTSL2561.h>

//double lux; // Resulting lux value
//boolean good; // True if neither sensor is saturated
           
// Create an SFE_TSL2561 object, here called "light":
 
SFE_TSL2561 light;  // I2C Adddress: 0x39
 
// Global variables:
 
boolean gain; // Gain setting, 0 = X1, 1 = X16;
unsigned int ms; // Integration ("shutter") time in milliseconds
 
void LUX_setup()
{
// Initialize the Serial port:
 
//Serial.begin(115200);
//Serial.println("\n\nTSL2561 example sketch");
 
// Initialize the SFE_TSL2561 library
 
light.begin();
 
// Get factory ID from sensor:
// (Just for fun, you don't need to do this to operate the sensor)
 
unsigned char ID;
 
if (light.getID(ID))
{
//    Serial.print("Got factory ID: 0X");
//    Serial.print(ID,HEX);
//    Serial.println(", should be 0X5X");
}
// Most library commands will return true if communications was successful,
// and false if there was a problem. You can ignore this returned value,
// or check whether a command worked correctly and retrieve an error code:
else
{
    byte error = light.getError();
//    printError(error);
}
 
// The light sensor has a default integration time of 402ms,
// and a default gain of low (1X).
 
// If you would like to change either of these, you can
// do so using the setTiming() command.
 
// If gain = false (0), device is set to low gain (1X)
// If gain = high (1), device is set to high gain (16X)
 
gain = 0;
 
// If time = 0, integration will be 13.7ms
// If time = 1, integration will be 101ms
// If time = 2, integration will be 402ms
// If time = 3, use manual start / stop to perform your own integration
 
unsigned char time = 2;
 
// setTiming() will set the third parameter (ms) to the
// requested integration time in ms (this will be useful later):
 
//Serial.println("Set timing...");
light.setTiming(gain,time,ms);
 
// To start taking measurements, power up the sensor:
 
//Serial.println("Powerup...");
light.setPowerUp();
 
// The sensor will now gather light during the integration time.
// After the specified time, you can retrieve the result from the sensor.
// Once a measurement occurs, another integration period will start.
}
 
void LUX_loop()
{
 
// ms = 1000;
// light.manualStart();
// delay(ms);
// light.manualStop();
 
// Once integration is complete, we'll retrieve the data.
 
// There are two light sensors on the device, one for visible light
// and one for infrared. Both sensors are needed for lux calculations.
 
// Retrieve the data from the device:
 
unsigned int data0, data1;
 
    if (light.getData(data0,data1))
    {
          // getData() returned true, communication was successful
           
          //Serial.print("\tdata0: ");
          //Serial.print(data0);
          //Serial.print("\t  |  data1: ");
          //Serial.print(data1);
           
          // To calculate lux, pass all your settings and readings
          // to the getLux() function.
           
//          double lux; // Resulting lux value
//          boolean good; // True if neither sensor is saturated
           
          // Perform lux calculation:
                    
          good = light.getLux(gain,ms,data0,data1,lux); // value of good is True if neither sensor is saturated 
           
          // Print out the results:
           
          //Serial.print("\t\t |  lux: ");
          
//          Serial.print("\n\tlux: ");
//          Serial.print(lux);
//          Serial.println();

//          if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
    }
    else
      {
    //   getData() returned false because of an I2C error, inform the user.
       
      byte error = light.getError();
//      printError(error);
      }
}

// If there's an I2C error, this function will
// print out an explanation. 
void printError(byte error)
{
  Serial.print("I2C error: "); Serial.print(error,DEC); Serial.print(", ");
 
  switch(error)
  {
    case 0:
    Serial.println("success");
    break;
    case 1:
    Serial.println("data too long for transmit buffer");
    break;
    case 2:
    Serial.println("received NACK on address (disconnected?)");
    break;
    case 3:
    Serial.println("received NACK on data");
    break;
    case 4:
    Serial.println("other error");
    break;
    default:
    Serial.println("unknown error");
  }
}
