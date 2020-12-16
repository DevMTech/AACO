/*
 RGB LED PWM
 
 https://www.electronicshub.org/rgb-led-with-arduino/
 https://os.mbed.com/users/4180_1/notebook/rgb-leds/

 CONNECT A RESISTOR: 100ohm to 220ohm TO EACH LED
 
 COMMON ANODE -> RESISTORS BETWEEN CATHODE OF EACH LED & GND 
 COMMON CATHODE -> RESISTORS BETWEEN VCC & ANODE OF EACH LED 
 
 FORWARD CURRENT OF R, G, B EACH IS 20mA
 FORWARD VOLTAGE OF R=2V, G=2.4V, B=3.9V
 
  https://www.electronicshub.org/led-resistor-calculator/
  LED Colour   Forward Current (IF)   Forward Voltage (VF­)
  Red                 20 mA                 2 V
    
  Green               20 mA                 2.4 V
  
  Blue                20 mA                 3.9 V

  Yellow              20 mA                 2 V
  
  White               30 mA                 3.6 V
  https://www.allaboutcircuits.com/tools/led-resistor-calculator/
  
*/

const int redPin = 6;
const int greenPin = 7;
const int bluePin = 8;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()
{
  delay(1000);
  primaryColors(1, 0, 0); // Red
  delay(2000);
  primaryColors(0, 1, 0); // Green
  delay(2000);
  primaryColors(0, 0, 1); // Blue
  delay(2000);
  primaryColors(1, 1, 0); // Yellow
  delay(2000);
  primaryColors(1, 0, 1); // Magenta
  delay(2000);
  primaryColors(0, 1, 1); // Cyan
  delay(2000);
  primaryColors(1, 1, 1); // White
  delay(2000);

  RGBFading();
}

void primaryColors(int redValue, int greenValue, int blueValue)
{
  digitalWrite(redPin, redValue);
  digitalWrite(greenPin, greenValue);
  digitalWrite(bluePin, blueValue);
}

void RGBFading()
{
  int x;
  int redBrightness;
  int greenBrightness;
  int blueBrightness;

  for (x = 0; x < 768; x++)
  {
    if (x <= 255)
    {
      redBrightness = 255 - x;
      greenBrightness = x;
      blueBrightness = 0;
    }
    else if (x <= 511)
    {
      redBrightness = 0;
      greenBrightness = 255 - (x - 256);
      blueBrightness = (x - 256);
    }
    else
    {
      redBrightness = (x - 512);
      greenBrightness = 0;
      blueBrightness = 255 - (x - 512);
    }

    analogWrite(redPin, redBrightness);
    analogWrite(bluePin, blueBrightness);
    analogWrite(greenPin, greenBrightness);

    delay(10);
  }
}
