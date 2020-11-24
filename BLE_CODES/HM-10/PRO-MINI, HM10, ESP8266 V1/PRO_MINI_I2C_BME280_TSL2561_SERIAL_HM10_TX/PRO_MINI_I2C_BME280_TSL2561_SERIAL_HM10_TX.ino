/**
 * PRO_MINI_I2C_BME280_TSL2561_SERIAL_TX.ino
 *
 *  PRO MINI - I2C - A4 SDA, A5 SCL
 *  
 *  TO DOs:
 *  
 *  ROCKETSCREAM (LOW POWER + MINI PRO) 
              - POWER DOWN I2C BUS, SENSORS
              - ESP01 ON/OFF(CH_PD), RX-TX DATA ONCE/HOUR
              - MINI PRO @ 1-8MHz CLK
              - DEEPSLEEP

    MINI PRO (LOW-POWER) + ESP01 (NORMAL / ESP-NOW)

              
    TIME-STAMP, BATTERY STAT.
            
    CRON (ATTINY + ESP01)     - POWER DOWN ESP01 - ULTIMATE LOW POWER WEATHER STATION
    GAMMON (328P POWER DOWN)  - POWER DOWN SUBSYSTEMS (WDT, BO, ADC)
 *
 */
 
#include <SoftwareSerial.h>
SoftwareSerial HM10_Serial(2, 3); // Arduino : RX(2) | TX(3) <-> TXD | RXD : HM10 - BLE
/*
 * Pro Mini         HM10
 *  3V3              VCC
 *  GND              GND
 *  D2 (RX)          TX   
 *  D3 (TX)          RX
 *  -------------------------------------
 *    
 *  PRO MINI      I2C DEVICE    
 *  3V3             VCC
 *  GND             GND
 *  A4 (SDA)        SDA   
 *  A5 (SCL)        SCL
 *  
*/
 
float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0;
int count = 0;
double lux; // Resulting lux value
boolean good; // True if neither sensor is saturated
String str_Tx;

void setup() 
{   Serial.begin(9600);       //  Serial.flush();   delay(1000);
    while (!Serial)   { ; }
    
    // Start the software serial for communication with the ESP8266
    HM10_Serial.begin(9600); 
    delay(500);
    Serial.println("test I am slave"); // Goodnight moon!
    HM10_Serial.print("AT+IMME0"); // set as AUTO-CONNECTABLE
    HM10_Serial.print("AT+ROLE0"); // set  SLAVE/PERIPHERAL
    // set Peripheral in AT+MODE1/2 as suitable 
    // AT+MODE1: PIO2,3 = O/P; PIO4-B = I/P, ADC
    // AT+MODE2: PIO2-B = O/P
    HM10_Serial.print("AT+MODE2"); // AT+MODE2: set pins PIO2-B as O/P pins
    HM10_Serial.print("AT+RESET"); // RESET DEVICE
    delay(500);
    
//    Serial.println("\n\n*********************START UP CODE***********************\n\n");
    LUX_setup();
    BME_setup();

//    Serial.println("\n\n*********************START UP DONE***********************\n\n");
}

void loop() 
{   
    LUX_loop();
    BME_loop();    
//  printValues();
    
    // listen for communication from the ESP8266 and then write it to the serial monitor
//    if ( HM10_Serial.available() )   {  Serial.write( HM10_Serial.read() );  }
 
    // listen for user input and send it to the ESP8266
//    if ( Serial.available() )       {  HM10_Serial.write( Serial.read() );  }

//    str_Tx =String('C')+String(count)+String('T')+String(tempC)+String('P')+String(pressure)+String('A')+String(alti)+String('H')+String(humidity)+String('L')+String(lux);
    str_Tx =String('T')+String(tempC)+String('H')+String(humidity)+String('L')+String(lux)+String("E");
//    str_Tx =String('T')+String("24.5")+String('H')+String("50.4")+String('L')+String("675.2")+String("END");
//    count++;

    Serial.println(str_Tx);
//    HM10_Serial.write(str_Tx);
    HM10_Serial.print(str_Tx);  
    
//    Serial.print("\n\n*********************NEXT LOOP ");
//    Serial.print(count++);
//    Serial.println(" ***********************\n\n");
    delay(10000);
} 
