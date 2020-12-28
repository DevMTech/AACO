/*  https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
     https://www.youtube.com/watch?v=RlKMJknsNpo
     https://microcontrollerslab.com/adc-esp32-measuring-voltage-example/
     https://github.com/espressif/arduino-esp32/issues/92
      https://www.esp32.com/viewtopic.php?t=769


      ESP32 ADC 12bit resolution
      0->4095 = 0->3.3V

     battery(V_batt) - 5V (TAKEN AS 5V HERE)
                      |
                      |
                      R1 (20k)
                      |
                      |---- 3V3 (V_A0) - ESP32 ADC PIN (ANALOG I/P PIN)
                      |
                      R2 (20k)
                      |
                      |
                     GND (ESP32)


 * ***************************************************/
extern "C" int rom_phy_get_vdd33();

#define ADC_VOLT_MAX 3.30f // 3.6V
#define BATTERY_VOLT_MAX 5 // 5V  // CHANGE THIS W.R.T. BATTERY USED TO RECALIBRATE 
//#define BUZZER 12 // GPIO12 ESP32
//#define BATTERY_V_IN 35 // GPIO35 ESP32
//#define PFET_POT_DIV 32 // GPIO32 ESP32

//float batteryRaw = 0.0, batteryLevel = 0.0, batteryLevelPercent = 0.0;

// 9.99/19.67;
//float R1 = 9.99; // 10k
//float R2 = 19.67; // 20k

// 21.62/21.58
float R1 = 19.58; // 21.62; // 20k
float R2 = 19.64; // 21.58; // 20k


float ratio = R2 / (R1 + R2);

void batteryLevelRead_setup() // batteryLevelRead()
{
  //pinMode(BUZZER, INPUT); // BUZ OFF // pinMode(BUZZER, OUTPUT); // BUZ ON
  pinMode(BATTERY_V_IN, INPUT);  // GPIO35 ESP32
  //  pinMode(PFET_POT_DIV, INPUT); // OFF // GPIO32 ESP32
  //  digitalWrite(PFET_POT_DIV, HIGH); // POWER OFF POT DIV
}



void batteryLevelRead()
{
  pinMode(PFET_POT_DIV, OUTPUT); // ON // GPIO32 ESP32
  //  delay(5);
  digitalWrite(PFET_POT_DIV, LOW); // POWER ON POT DIV
  delay(5);

  //VCC = getMyVCC();

  batteryRaw = analogRead(BATTERY_V_IN);
  //  delay(10);
  //  digitalWrite(PFET_POT_DIV, HIGH); // POWER OFF POT DIV

  batteryLevel = batteryRaw * ADC_VOLT_MAX / (4095 * ratio);
//  batteryLevel = batteryRaw * 3.6 / (4095);
//    batteryLevel = batteryRaw * ADC_VOLT_MAX / (4095.0f);

  Serial.println("Battery Level RAW : " + String(batteryRaw));

  //  batteryLevelPercent = map(batteryLevel, 0.0f, BATTERY_VOLT_MAX, 0, 100);

  Serial.println("Ratio : " + String(ratio) + "");

  Serial.println("Battery Level : " + String(batteryLevel) + "V");
  //  Serial.println("Battery Level : "+String(batteryLevelPercent)+ "%\n ***************** \n");

  //  digitalWrite(PFET_POT_DIV, HIGH); // POWER OFF POT DIV
  pinMode(PFET_POT_DIV, INPUT); // OFF // GPIO32 ESP32
  

  //  delay(100);

#ifdef BUZZER
  //  if((int)batteryLevelPercent < 20)
  //  { pinMode(BUZZER, OUTPUT);   // BUZ ON
  //    digitalWrite(BUZZER, !(digitalRead(BUZZER))); delay(500);
  //    pinMode(BUZZER, INPUT); // BUZ OFF
  //  }
#endif

}

// https://github.com/esp-now/esp-now-projects/blob/108a63d6f1534a3b2739cdf419d914b163ed47d3/Basic%20Demo/Sensor_Basic/MQTT.ino
//double getMyVCC()
//{
//  int internalBatReading = rom_phy_get_vdd33();
//  Serial.println("internalBatReading = "+ String(internalBatReading)); 
//  double vcc = (double)(((uint32_t)internalBatReading * 2960) / 2798) / 1000;
//  /*
//   * Ever since espressif released there latest arduino core the rom_phy_get_vdd33() 
//   * returns a weird number. The best I can tell is it's double the actual reading 
//   * so if the result is over 4 volts I half it before returning. 
//   * This should allow the code to work both with the new core and the original one.
//   */
//  // if (vcc > 4)    vcc = (vcc / 2);
//
//  Serial.println("VCC = "+ String(vcc));  
//  return vcc;
//}

/*
    ESP32 ADC
    https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
    https://www.youtube.com/watch?v=RlKMJknsNpo

    analogReadResolution(resolution): set the sample bits and resolution. It can be a value between 9 (0 – 511) and 12 bits (0 – 4095). Default is 12-bit resolution.
    analogSetWidth(width): set the sample bits and resolution. It can be a value between 9 (0 – 511) and 12 bits (0 – 4095). Default is 12-bit resolution.
    analogSetCycles(cycles): set the number of cycles per sample. Default is 8. Range: 1 to 255.
    analogSetSamples(samples): set the number of samples in the range. Default is 1 sample. It has an effect of increasing sensitivity.
    analogSetClockDiv(attenuation): set the divider for the ADC clock. Default is 1. Range: 1 to 255.
    analogSetAttenuation(attenuation): sets the input attenuation for all ADC pins. Default is ADC_11db. Accepted values:
        ADC_0db: sets no attenuation (1V input = ADC reading of 1088).
        ADC_2_5db: sets an attenuation of 1.34 (1V input = ADC reading of 2086).
        ADC_6db: sets an attenuation of 1.5 (1V input = ADC reading of 2975).
        ADC_11db: sets an attenuation of 3.6 (1V input = ADC reading of 3959).
    analogSetPinAttenuation(pin, attenuation): sets the input attenuation for the specified pin. The default is ADC_11db. Attenuation values are the same from previous function.
    adcAttachPin(pin): Attach a pin to ADC (also clears any other analog mode that could be on). Returns TRUE or FALSE result.
    adcStart(pin), adcBusy(pin) and resultadcEnd(pin): starts an ADC convertion on attached pin’s bus. Check if conversion on the pin’s ADC bus is currently running (returns TRUE or FALSE). Get the result of the conversion: returns 16-bit integer.


   https://www.esp32.com/viewtopic.php?t=769
  I use a 27 kOhm + 100 kOhm voltage divider to bring the 4.2 V maximum of a single cell LiPo battery down to 3.3 V, which is the reference voltage (AFAIK) of the ESP32 board I am using. I have the voltage divider connected to pin 34, and I read the pin as follows:

  Code: Select all

    float VBAT = (127.0f/100.0f) * 3.30f * float(analogRead(34)) / 4096.0f;  // LiPo battery
    Serial.print("Battery Voltage = "); Serial.print(VBAT, 2); Serial.println(" V");

  The ADC is 12-bit which is why the 3.3/4096 (ref voltage/max counts) conversion factor is in there. When the analog read returns 0, the battery voltage is 0, and when it returns 4095, the battery voltage is 4.2 V. The response of the ESP32 ADCs is apparently non-linear so you will have to calibrate the response and correct for maximum accuracy.

*/



//  for (int i = 0; i < 10; i++)
//  {
//    //Serial.println(digitalRead(LED_BUILTIN));
//    delay(500);
//    digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
//  }
