/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1014AU0F Dust Sensor Demo
//S.  NO.       Pin Name  Pin Description
//1   V-LED     LED Vcc Pin. Connect to 5V through 150Ω Resistor
//2   LED-GND   LED Ground Pin. Connect to GND
//3   LED       Used to Toggle LED On/Off. Connect to any digital pin of Arduino
//4   S-GND     Sensor Ground Pin. Connect to GND of Arduino
//5   VOUT      Sensor Analog Output Pin. Connect to any Analog Pin
//6   VCC       Positive Supply Pin. Connect to 5V of Arduino
//
//  GP2Y1010AU0F Optical Dust Sensor
//
//  Sharp Dust Sensor  Attached To
//  1 (V-LED)     3.3V Pin (150 Ohm in between Pin 1 & 3V3)
//  2 (LED-GND)   GND Pin
//  3 (LED)     Digital Pin 12 (to turn on the IR Led of the Dust Sensor)
//  4 (S-GND)     GND Pin
//  5 (Vo)      Analog Pin A6
//  6 (Vcc)     3.3V Pin (Direct)
//
//  220uF between Pin 1(+) & Pin 2(-)
//
// Board Connection:
//  PIN   GP2Y1014    Arduino
//  1     V-LED       Connect to 5V through 150Ω Resistor
//  2     LED-GND     GND, 220uF BETWEEN V-LED & LED-GND
//  3     LED         D7
//  4     S-GND       GND
//  5     Vo          A0
//  6     Vcc         5V
//
// Serial monitor setting:
//   115200 baud
//  https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-GP2Y1014AU0F-Dust-Sensor
//  https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Sharp%20GP2Y1010AU0F%20-%20Dust%20Density%20Conversion.pdf
//  https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1014au0f_demo/sharp_gp2y1014au0f_demo.ino
/////////////////////////////////////////////////////////////////////////////

#ifdef DUST_SHARP

// Choose program options.
//#define PRINT_RAW_DATA
#define USE_AVG

// Arduino pin numbers.
//const int sharpLEDPin = 7;   // Arduino digital pin 7 connect to sensor LED.
//const int sharpVoPin = A0;   // Arduino analog pin 0 connect to sensor Vo.

// ESP32 pin numbers.
const int sharpLEDPin = 13;   // ESP32 IO13 pin connect to sensor LED (3).
const int sharpVoPin = 14;   // ESP32 IO14 pin connect to sensor Vo (5).


int VoRaw = 0;

// For averaging last N raw voltage readings.
#ifdef USE_AVG
#define N 100
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
#endif // USE_AVG

// Set the typical output voltage in Volts when there is zero dust.
static float Voc = 0.6;

// Use the typical sensitivity in units of V per 100ug/m3.
const float K = 0.5;

const float DUST_VCC = 3.3;

/////////////////////////////////////////////////////////////////////////////

// Helper functions to print a data value to the serial monitor.
void printValue(String text, unsigned int value, bool isLast = false)
{
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast)
  {
    Serial.print(", ");
  }
}

void printFValue(String text, float value, String units, bool isLast = false)
{
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  Serial.print(units);
  if (!isLast)
  {
    Serial.print(", ");
  }
}

/////////////////////////////////////////////////////////////////////////////

// setup function.
void DUST_SHARP_setup()
{
  // Set LED pin for output.
  pinMode(sharpLEDPin, OUTPUT);

  // Start the hardware serial port for the serial monitor.
  //Serial.begin(115200);

  // Wait two seconds for startup.
  delay(200);
  //  Serial.println("");
  //  Serial.println("GP2Y1010AU0F VoRaw");
  //  Serial.println("=================");

  for(int i = 0; i < 5; i++)
  {
    DUST_SHARP_print();
  }
  
  
}

int DUST_SHARP_VoRaw()
{
  // Turn on the dust sensor LED by setting digital pin LOW.
  digitalWrite(sharpLEDPin, LOW);

  // Wait 0.28ms before taking a reading of the output voltage as per spec.
  delayMicroseconds(280);

  // Record the output voltage. This operation takes around 100 microseconds.
  int VoRaw = analogRead(sharpVoPin);

  // Turn the dust sensor LED off by setting digital pin HIGH.
  digitalWrite(sharpLEDPin, HIGH);

  // Wait for remainder of the 10ms cycle = 10000 - 280 - 100 microseconds.
  delayMicroseconds(9620);

  // Print raw voltage value (number from 0 to 1023).
#ifdef PRINT_RAW_DATA
  printValue("VoRaw", VoRaw, true);
  Serial.println("");
#endif // PRINT_RAW_DATA

  return VoRaw;

}

void DUST_SHARP_print()
{
  float Vo = 0;

  // Use averaging if needed.

#ifdef USE_AVG
  for (VoRawCount = 1; VoRawCount <= N; VoRawCount++)
  { VoRawTotal += DUST_SHARP_VoRaw();    //VoRawCount++;
  }
  
  Serial.println("\n VoRawCount "+String(VoRawCount));
  Serial.println("\n VoRawTotal "+String(VoRawTotal));
  
  if ( VoRawCount >= N )
  {
    Vo = 1.0 * VoRawTotal / N;
    VoRawCount = 0;
    VoRawTotal = 0;
  }
  Serial.println("\n Vo "+String(Vo));
//#else
//  Vo = DUST_SHARP_VoRaw();
#endif // USE_AVG

#ifndef USE_AVG
  Vo = DUST_SHARP_VoRaw();
#endif // DO NOT USE_AVG

  // Compute the output voltage in Volts.
  Vo = Vo * DUST_VCC / 4096.0; // ESP32
  //Vo = Vo / 1024.0 * DUST_VCC; // ARDUINO
  printFValue(" Vo", Vo * 1000.0, "mV");

  // Convert to Dust Density in units of ug/m3.
  float dV = Vo - Voc; // Voc = typical output voltage in Volts when there is zero dust.
  if ( dV < 0 )
  {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / K * 100.0;
  printFValue(" DustDensity", dustDensity, "ug/m3", true);
  pm_10 = dustDensity;
  Serial.println("\n");

} // END PROGRAM

#endif
