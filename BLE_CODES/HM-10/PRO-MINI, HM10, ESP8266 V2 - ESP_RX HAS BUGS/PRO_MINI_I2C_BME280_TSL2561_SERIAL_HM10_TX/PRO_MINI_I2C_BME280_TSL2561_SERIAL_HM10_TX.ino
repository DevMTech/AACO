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

// [ESP01] GPIO2(RX)=HM10_TXD, GPIO0(TX)=HM10_RXD
//SoftwareSerial HM10_Serial(2, 0, false, 256);  // [ESP01]

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

static const uint8_t TX_DELAY_MS = 500; 
float tempC = 0, tempF = 0, pressure = 0, alti = 0, humidity = 0;
double lux; // Resulting lux value
boolean good; // True if neither sensor is saturated
String str_Tx;
static uint8_t SKIP = 10; // skipping initial 10 readings to allow sensor readings to stabilize
void setup() 
{   Serial.begin(9600);       //  Serial.flush();   delay(1000);
    while (!Serial)   { ; }
    
    // Start the software serial for communication with the ESP8266
    HM10_Serial.begin(9600); 
    delay(TX_DELAY_MS);
    Serial.println("\n\nI am SLAVE/PERIPHERAL device"); // Goodnight moon!

    HM10_CONFIG("AT+IMME0");  // set as AUTO-CONNECTABLE device
    HM10_CONFIG("AT+ROLE0");  // set as SLAVE/PERIPHERAL device
    HM10_CONFIG("AT+MODE2");  // AT+MODE2: set pins PIO2-B as O/P pins
    HM10_CONFIG("AT+RESET");  // RESET DEVICE

    // AT+MODE1: PIO2,3 = O/P; PIO4-B = I/P, ADC
    // AT+MODE2: PIO2-B = O/P
    
//    Serial.println("\n\n*********************START UP CODE***********************\n\n");
    LUX_setup();
    BME_setup();

//    Serial.println("\n\n*********************START UP DONE***********************\n\n");
}

void loop() 
{   static uint8_t count = 0;
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
    if(count<SKIP)  // skipping initial 10 readings to allow sensor readings to stabilize 
    { count++;
    }
    else
    { HM10_Serial.print(str_Tx);  //    HM10_Serial.write(str_Tx);
    }
//    Serial.print("\n\n*********************NEXT LOOP ");
//    Serial.print(count++);
//    Serial.println(" ***********************\n\n");
    delay(10000);
}

void HM10_CONFIG(String Config)
{
  Serial.print("Sent : "+Config+" : ");
    HM10_Serial.print(Config); // AT+MODE2: set pins PIO2-B as O/P pins
    HM10_RESPONSE();
}

void HM10_RESPONSE()
{   String  HM10response;
    delay(TX_DELAY_MS);
    if (HM10_Serial.available()) // READ
    { 
      HM10response = HM10_Serial.readString();  // Until('E');      // delay(100);
      Serial.println("Received: " + HM10response); // RESPONSE

//      if(ReadString.indexOf("END") > 0)
//      { Serial.println("FOUND END\n");
//      }
    }
    else
    {Serial.println("Received: NO RESPONSE FROM HM10!!");
    }
    
    delay(TX_DELAY_MS);
} 
