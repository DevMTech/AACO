/*
    http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/
    http://www.hangar42.nl/hm10
    http://archive.fabacademy.org/archives/2016/fablabdigiscope/students/456/W19_project_development.html
    http://fab.cba.mit.edu/classes/863.15/doc/tutorials/programming/bluetooth.html
    http://archive.fabacademy.org/archives/2016/fablabdigiscope/students/456/project_summary.html
    https://www.hackster.io/achindra/bluetooth-le-using-cc-41a-hm-10-clone-d8708e
    https://www.hackster.io/mayooghgirish/arduino-bluetooth-basic-tutorial-d8b737

    https://arduinoinfo.mywikis.net/wiki/BlueTooth-HC05-HC06-Modules-How-To
    http://wiki.sunfounder.cc/index.php?title=Bluetooth_4.0_HM-10_Master_Slave_Module

    https://evothings.com/control-an-led-using-hm-10-ble-module-an-arduino-and-a-mobile-app/
    
    12345678901234567890
    1234567890123456789011111
    MY NAME IS DEBAJYOTI MAITRA
    DEBAJYOTI MAITRA
    ONLY LAST 20 CHARACTERS
    VCC to 5V
    GND to GND
    TXD to pin D10
    RXD to pin D11

  AT+VERR?
  AT+VERS?
  AT+ADDR?
  AT+BAUD0
  AT+BAUD?

            BLE_BAUD
  0 -------- 9600
  1 -------- 19200
  2 -------- 38400
  3 -------- 57600
  4 -------- 115200
  5 -------- 4800
  6 -------- 2400
  7 -------- 1200 // Do not recommend using the baud rate.
  8 -------- 230400
  Default: 0(9600)

  Note: after the switch to the 1200, module will no longer support
  the configurations of the AT command, and press the PIO0 under standby,
  module can restore the factory Settings.
  Do not recommend using the baud rate.
  After setting the baud rate, modules should be on electricity,
  new set parameters can take effect.

  AT+CONN[para1]   // [para1] bluetooth address: 0017EA0943AE
  AT+CLEAR
  AT+MODE?
  Para: the range of 0 ~ 2. 0 represents passthrough mode,
  on behalf of the PIO acquisition + remote control + 1 passthrough,
  2 representative passthrough + remote control mode.The default is 0.

  AT+PASS123456
  AT+PASS?  // Para1 range is 000000 ~ 999999, the default is 000000.


  AT+RENEW  // Restore Factory Settings

  AT+ROLE[para1]  // AT+ROLE0 --> SLAVE // AT+ROLE1 --> MASTER
  AT+RESET        // RESTART MODULE AFTER CHANGING ROLE

  AT+ROLE? returns either 0 or 1.
            0 = Slave or Peripheral
            1 = Master or Central.
            The default setting is 0 (Slave).

  AT+IMME?  //   Query the start mode
  AT+IMME? returns either 0 or 1.
  0 = Connect immediately (assuming a previous connection has been applied
  1 = Wait for a connection command before connecting (AT+START, AT+CONN, AT+CONNL
  The default setting is 0 (connect on start).

  AT+IMMEx   // Set the start up mode
  AT+IMME0 sets auto connect on start*
  AT+IMME1 sets manual connection mode
  AT+IMMEx is often used together with AT+ROLEx
  AT+IMMEx mat require a reset before the changes take place.
  AT+RESET        // RESTART MODULE AFTER CHANGING START UP MODE

  AT+IMME0 --> If there are no previous connections, HM-10s will auto-connect
  to any other HM-10 available (normally the one with the
  strongest signal). The HM-10 (by default) remembers the
  address of the last module it was connected to and if there
  is stored a previous connection this will take priority when
  the HM-10 is retrying to auto-connect.

  ROLE  IMME
  0     0     AUTO-DISCOVERABLE MODE (PERIPHERAL)
              Connect immediately

  0     1     COMMAND-DISCOVERABLE MODE (PERIPHERAL)
              stops the HM-10 from auto-connecting
              waits for CONNECTION COMMANDS
              AT+START required

  1     0     AUTO-SCAN-CONNECT MODE (CENTRAL)
              Connect immediately

  1     1     COMMAND-SCAN-CONNECT MODE (CENTRAL)
              stops the HM-10 from auto-connecting
              waits for CONNECTION COMMANDS

  AT+IMME1    stops the HM-10 from auto-connecting
  AT+ROLE1    puts the HM-10 in to Central mode

  AT+RESET
  //DISCOVER ONLY SLAVE/PERIPHERAL DEVICES: AT+DISC?
  AT+DISC?  //  OK+DISCS  OK+DISCSOK+DIS0:C8FD199C8598
            //  OK+DISCE --> E shows that the scan has Ended

  AT+DISI?  //  OK+DISIS    OK+DISCE
            //  OK+DISC:00000000:00000000000000000000000000000000:0000000000:1291EC5518B7:-089
  AT+DISA?

  AT+CONNx --> Connect to device discovered during scanning
              and saved at index 'x'
              1st slave device mac is saved at index '0'
  AT+CON<SlaveMAC>
  AT+CONC8FD199C8598  // OK+CONNA
          --> connection with slave with MAC C8FD199C8598 is successful
  AT+CONNL
  AT+START

  AT+TCON     forget device being scanned after timeout & re-scan

  AT+SAVE     check if device address saved or not

  AT+NOTI0 –-> turn off notifications
  AT+NOTI1 –-> turn on notifications
  AT+NOTI?

  AT+CHAR?    // 0xFFE1
  AT+UUID?    // 0xFFE0

  AT+SLEEP  // WAKE UP BY SENDING 1024 CHAR. STRING USING UART
  AT+PWRM1 // AUTO-SLEEP UPON POWER-UP MODE


  AT commands only work when the HM-10 is not connected.
  After a connection is made the commands are treated as data.
  “AT” is the exception, the “AT” command breaks the connection.

    Wiring the HM-10 to the Arduino Uno/Pro Mini board should go as follows:
  Pro Mini
  Arduino  | HM-10
  D2 (RX)  | TX
  D3 (TX)  | RX
  GND      | GND
  3.3V     | VCC
  D4       | EN (Optional)

  USE THIS CODE (HM10_328P_SOFTSERIAL_COMM.ino),
  ITS BETTER THAN HM10_BLE_Serial_Write.ino


*/

#include <SoftwareSerial.h>
#define BLE_BAUD 9600

//SoftwareSerial HM10Serial(8, 9);
SoftwareSerial HM10Serial(2, 3);  // pro min // D2=RX, D3=TX
//SoftwareSerial HM10Serial(10, 11); // Uno // RX, TX

//SoftwareSerial HM10Serial(14, 12, false, 256); // D5(RX), D6(TX) [ESP8266]
// SoftwareSerial(rxPin, txPin, inverse_logic, buffer size);
//https://circuits4you.com/2016/12/16/esp8266-software-serial/

String value;

void setup()
{
  //  pinMode(4, OUTPUT); // D4 = EN
  //  digitalWrite(4, HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(9600); // 115200); //
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Experimenting with HM10");

  delay(500);

  // set the data rate for the SoftwareSerial port
  HM10Serial.begin(BLE_BAUD);
  // If the baudrate of the HM-10 module has been updated,
  // you may need to change 9600 by another value
  // Once you have found the correct baudrate,
  // you can update it using AT+BAUDx command
  // e.g. AT+BAUD0 for 9600 bauds
  Serial.print("BTserial started at ");
  Serial.println(BLE_BAUD);
}

void loop() // run over and over
{
  /* http://blog.blecentral.com/2015/05/05/hm-10-peripheral/
   * Any data received by the HM-10 over BLE (from the mobile app) 
   * is passed to the Arduino Serial port and displayed on the 
   * IDE console. Similarly, any data input in the IDE console is 
   * passed through the software serial port to the HM-10 and is 
   * then transmitted over BLE to the Android app. If the HM10 is 
   * not connected to any central device (Android smartphone), 
   * the data input in the console is simply discarded.
  */
  HM10Serial.print("HI FROM BLE1");

  HM10Serial.print("Testing...123456789");

  //the IDE sends, phone receives
  if (Serial.available()) // WRITE
  {
    value = Serial.readString();      // delay(10);
    Serial.println("Sent by HM10: \t" + value); // delay(10);
    HM10Serial.print(value);
  }
  //Serial.println("\n After Serial");

  //the phone sends, IDE receives
  if (HM10Serial.available()) // READ
  {
    value = HM10Serial.readString();     // delay(10);
    Serial.println("Received by HM10: " + value); // RESPONSE

    //      if (value == "all LED turn on")
    //      {   digitalWrite(5, HIGH);  //    digitalWrite(6, HIGH);
    //      }
  }
  //Serial.println("\n After HM10Serial");
  
}

/*
    if (Serial.available())   // WRITE
    { Serial.print("\nWRITING ... ");
      HM10Serial.print(Serial.readString());  // HM10Serial.write(Serial.read());
    }

    if (HM10Serial.available()) // READ
    {
      Serial.print("\nRESPONSE: ");
      Serial.println(HM10Serial.readString());   // Serial.write(HM10Serial.read());
    }
*/

/*
    if (HM10Serial.available())
    Serial.write(HM10Serial.read());
    if (Serial.available())
    HM10Serial.write(Serial.read());
*/

/*  int analogValue = analogRead(0); // A0
    HM10Serial.println(analogValue);
*/

/*
  if (Serial.available())  // WRITE
  {
    String s = "";      char c;
    while((c = Serial.read()) != -1)
    {  s += c;       delay(10);
    }
    delay(10);
    Serial.println("Sent: \t\t" + s);
    HM10Serial.print(s);
  }

  if (HM10Serial.available()) // READ
  {
    String s = "";     char c;
    while((c = HM10Serial.read()) != -1)
    { s += c;      delay(10);
    }
    Serial.println("Received: \t" + s);
  }
*/
