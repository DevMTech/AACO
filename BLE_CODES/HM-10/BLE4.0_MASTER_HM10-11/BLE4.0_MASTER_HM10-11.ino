/*
  BLE 4.0 MASTER
  PRO MINI + HM10
  http://wiki.seeedstudio.com/Bluetooth_V4.0_HM_11_BLE_Module/
 This example code is in the public domain.
  
 */

#include <String.h>
#include <SoftwareSerial.h>

SoftwareSerial HM10_Serial(2, 3); // RX, TX (PRO MINI, ARDUINO)

//SoftwareSerial HM10_Serial(14, 12, false, 256); // D5(RX), D6(TX) [NODEMCU12E]
// SoftwareSerial(rxPin, txPin, inverse_logic, buffer size); 
//https://circuits4you.com/2016/12/16/esp8266-software-serial/

static const uint8_t TX_DELAY_MS = 500;
String value, SLAVE_MAC = "C8FD194AF746"; // "C8FD199C8598";

void setup()
{   
    //    pinMode(4, OUTPUT); // D4 = EN
    //    digitalWrite(4, HIGH);
  
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) { ; // wait for serial port to connect. Needed for Leonardo only
    }    
    
    HM10_Serial.begin(9600); // set the data rate for the SoftwareSerial port
  
    Serial.println("I am MASTER/CENTRAL"); // Goodnight moon!

    // AT+RENEW // reset to the default settings
    HM10_CONFIG("AT+IMME1"); // Set to manual connection with “AT+IMME1″ 
                                // stops the HM-10 from auto-connecting
                                // waits for CONNECTION COMMANDS

//    HM10_CONFIG("AT+IMME0"); // AUTO CONNECT TO NEAREST SLAVE
//    AT+DISC? & AT+CON<MAC> NOT REQUIRED WITH AT+IMME0 
    
    // Set to Central mode with “AT+ROLE1″
    HM10_CONFIG("AT+ROLE1"); // set as MASTER/CENTRAL
    
    HM10_CONFIG("AT+RESET"); // RESET MODULE
    
    HM10_CONFIG("AT+DISC?"); //DISCOVER ONLY SLAVE/PERIPHERAL DEVICES: AT+DISC?
    
    Serial.println("trying to connect to C8FD199C8598\n");
    HM10_CONFIG("AT+CON"+SLAVE_MAC);  
    //    HM10_CONFIG("AT+CONN0");   delay(TX_DELAY_MS);
    /*Connect to device discovered during scanning 
    and saved at index 'x' 
    1st slave device mac is saved at index '0'*/

//    Serial.println("CALLING TX_RX();\n");    TX_RX();          
//    delay(1000);
}

void loop() // run over and over
{
//    HM10_Serial.print("test I am master");    // delay(1000);

    // add an LED to PIO2 pin of Slave/Peripheral device 
    HM10_CONFIG("AT+PIO21"); // “AT+PIO21″ sets PIO2 pin HIGH 

    HM10_CONFIG("AT+PIO20"); // “AT+PIO20″ sets PIO2 pin LOW


    // AT+ADCB? requests the voltage on pin PIOB of the remote HM-10 (SLAVE/PERIPHERAL)
    HM10_CONFIG("AT+ADCB?"); // “AT+PIO21″ sets PIO2 pin HIGH 
     
    if (Serial.available())   // WRITE
    { 
      value = Serial.readString();      // delay(10);
      Serial.println("Sent: \t" + value); // delay(10);
      HM10_Serial.print(value);
    }  

    if (HM10_Serial.available()) // READ
    { 
      value = HM10_Serial.readString();      delay(500);
      Serial.println("Received: " + value); // RESPONSE

      if(value.indexOf("SLAVE") > 0)
      { Serial.println("FOUND SLAVE\n");
      }

//      if(value.indexOf("C8FD199C8598") > 0)
//      { Serial.println("FOUND C8FD199C8598\n");
//        HM10_Serial.print("AT+CONC8FD199C8598");
//        delay(TX_DELAY_MS);
//      }
    }

    /*
    if (HM10_Serial.available())
    Serial.write(HM10_Serial.read());
    if (Serial.available())
    HM10_Serial.write(Serial.read());
    */
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
}

void TX_RX()
{           
    if (Serial.available())   // WRITE
    { 
      value = Serial.readString();      // delay(10);
      Serial.println("Sent: \tAT+DISC?"); //  + value); // delay(10);
      HM10_Serial.print("AT+DISC?");
      delay(TX_DELAY_MS);
    }  

//    HM10_RESPONSE();
    
    if (HM10_Serial.available()) // READ
    { 
      String scan_resp = HM10_Serial.readString();     // delay(10);
      Serial.println("Received: " + scan_resp); // RESPONSE

      if(scan_resp.indexOf("C8FD199C8598") > 0)
      { Serial.println("FOUND C8FD199C8598\n");
        HM10_Serial.print("AT+CONC8FD199C8598");
        delay(TX_DELAY_MS);
      }
    }
}


 /*    
    //  Simple remote control using HM-10s: LED on. LED off
    //  MASTER HM-10, in Central mode sends AT+PIO21, AT+PIO20 to SLAVE HM-10
    //  SLAVE HM-10, in Peripheral mode, LED connected to its PIO2 pin
    
    // MASTER/CENTRAL BLE CODE
    // Very simple debouce.
    boolean state1 = digitalRead(switchPin); delay(1);
    boolean state2 = digitalRead(switchPin); delay(1);
    boolean state3 = digitalRead(switchPin); delay(1);
     
    if ((state1 == state2) && (state1==state3))  
    { 
        switch_State = state1;  
     
        if (switch_State != oldswitch_State)
        {
              if ( switch_State == HIGH) { BTserial.print("AT+PIO21" );  Serial.println("The LED is ON"); }
              else                       { BTserial.print("AT+PIO20" );  Serial.println("The LED is OFF"); }
              
//              if ( switch_State == HIGH) { BTserial.print("1" );  Serial.println("1"); }
//              else                       { BTserial.print("0" );  Serial.println("0"); }

              oldswitch_State = switch_State;
        }
    }

    // SLAVE/PERIPHERAL BLE CODE
    // set Peripheral in AT+MODE1/2 as suitable 
    // AT+MODE1: PIO2,3 = O/P; PIO4-B = I/P, ADC
    // AT+MODE2: PIO2-B = O/P
    // Read from the Bluetooth module and check if it is a command
    if (BTSerial.available())
    {
        c = BTSerial.read();
 
        // 49 is the ascii code for "1"
        // 48 is the ascii code for "0"
        if (c==49)   { digitalWrite(LEDpin,HIGH);   }
        if (c==48)   { digitalWrite(LEDpin,LOW);    }
        Serial.println(c);
    }
    */

/*
    if (HM10_Serial.available())
    Serial.write(HM10_Serial.read());
    if (Serial.available())
    HM10_Serial.write(Serial.read());
*/

/*
    if (Serial.available())   // WRITE
    { HM10_Serial.print(Serial.readString());  // HM10_Serial.write(Serial.read());
    }  

    if (HM10_Serial.available()) // READ
    {    
      Serial.print("\nReceived: ");
      Serial.println(HM10_Serial.readString());   // Serial.write(HM10_Serial.read());
    }
*/
