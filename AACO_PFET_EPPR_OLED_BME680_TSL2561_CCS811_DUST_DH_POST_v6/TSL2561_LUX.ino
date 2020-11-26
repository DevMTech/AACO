//#include <Wire.h>
#include <SparkFunTSL2561.h>

//double lux; // Resulting lux value
boolean good; // True if neither sensor is saturated
           
// Create an SFE_TSL2561 object, here called "light":
 
SFE_TSL2561 light; // 0x39
 
// Global variables:
 
boolean gain; // Gain setting, 0 = X1, 1 = X16;
unsigned int ms; // Integration ("shutter") time in milliseconds
 
void LUX_TSL2561_setup()
{
// Initialize the Serial port:
 
//Serial.begin(115200);
//DEBUG_Sprintln("\n\nTSL2561 example sketch");
 
// Initialize the SFE_TSL2561 library
 
  light.begin();
 
// Get factory ID from sensor:
// (Just for fun, you don't need to do this to operate the sensor)
 
  unsigned char ID;
   
  if (light.getID(ID))
  {
  DEBUG_Sprint("Got factory ID: 0X");
  DEBUG_Sprint2(ID, HEX);
  DEBUG_Sprintln(", should be 0X5X");
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else
  {
  byte error = light.getError();
  printError(error);
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
   
  DEBUG_Sprintln("Set timing...");
  light.setTiming(gain,time,ms);
   
  // To start taking measurements, power up the sensor:
   
  DEBUG_Sprintln("Powerup...");
  light.setPowerUp();
 
// The sensor will now gather light during the integration time.
// After the specified time, you can retrieve the result from the sensor.
// Once a measurement occurs, another integration period will start.
}
 
void LUX_TSL2561_loop()
{
 
  // ms = 1000;
  // light.manualStart();
  delay(ms);
  // light.manualStop();
   
  // Once integration is complete, we'll retrieve the data.
   
  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both sensors are needed for lux calculations.
   
  // Retrieve the data from the device:
   
  unsigned int data0, data1;
 
    if (light.getData(data0,data1))
    {
          // getData() returned true, communication was successful
           
          //DEBUG_Sprint("\tdata0: ");
          //DEBUG_Sprint(data0);
          //DEBUG_Sprint("\t  |  data1: ");
          //DEBUG_Sprint(data1);
           
          // To calculate lux, pass all your settings and readings
          // to the getLux() function.
           
//          double lux; // Resulting lux value
//          boolean good; // True if neither sensor is saturated
           
          // Perform lux calculation:
                    
          good = light.getLux(gain,ms,data0,data1,lux); // value of good is True if neither sensor is saturated 
           
          // Print out the results:
           
          //DEBUG_Sprint("\t\t |  lux: ");
          DEBUG_Sprint("\nLux: ");
          DEBUG_Sprint(lux);
          DEBUG_Sprintln();
          if (good); //DEBUG_Sprintln(" (good)"); else DEBUG_Sprintln(" (BAD)");
    }
    else
      {
    //   getData() returned false because of an I2C error, inform the user.
       
      byte error = light.getError();
      printError(error);
      }
}

// If there's an I2C error, this function will
// print out an explanation. 
void printError(byte error)
{
  DEBUG_Sprint("I2C error: ");
  DEBUG_Sprint2(error, DEC);
  DEBUG_Sprint(", ");
 
  switch(error)
  {
    case 0:
    DEBUG_Sprintln("success");
    break;
    case 1:
    DEBUG_Sprintln("data too long for transmit buffer");
    break;
    case 2:
    DEBUG_Sprintln("received NACK on address (disconnected?)");
    break;
    case 3:
    DEBUG_Sprintln("received NACK on data");
    break;
    case 4:
    DEBUG_Sprintln("other error");
    break;
    default:
    DEBUG_Sprintln("unknown error");
  }
}
