/*
  BLE 4.0 SLAVE
  PRO MINI + HM10
  http://wiki.seeedstudio.com/Bluetooth_V4.0_HM_11_BLE_Module/
 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

String value;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) { ; // wait for serial port to connect. Needed for Leonardo only
    }    
    
    mySerial.begin(9600); // set the data rate for the SoftwareSerial port

    Serial.println("test I am slave"); // Goodnight moon!
    mySerial.print("AT+IMME0"); // set as AUTO-CONNECTABLE
    mySerial.print("AT+ROLE0"); // set  SLAVE/PERIPHERAL
    // set Peripheral in AT+MODE1/2 as suitable 
    // AT+MODE1: PIO2,3 = O/P; PIO4-B = I/P, ADC
    // AT+MODE2: PIO2-B = O/P
    mySerial.print("AT+MODE2"); // AT+MODE2: set pins PIO2-B as O/P pins
    mySerial.print("AT+RESET"); // RESET DEVICE

    delay(1000);
}

void loop() // run over and over
{
    mySerial.print("TEST: I AM SLAVE ");    delay(1000);
    
//    if (Serial.available())   // WRITE
//    { 
//      value = Serial.readString();      // delay(10);
//      Serial.println("Sent: \t" + value); // delay(10);
//      mySerial.print(value);
//    }  
//    if (mySerial.available()) // READ
//    { 
//      value = mySerial.readString();     // delay(10);
//      Serial.println("Received: " + value); // RESPONSE
//    }
}
