void HM10_setup()
{   Serial.println("HM10 MASTER SETUP STARTS"); 
    //    pinMode(4, OUTPUT); // D4 = EN
    //    digitalWrite(4, HIGH); 
    
    HM10_Serial.begin(9600); // set the data rate for the SoftwareSerial port

    // AT+RENEW // reset to the default settings
    HM10_Serial.print("AT+IMME1"); // Set to manual connection with “AT+IMME1″ 
                                // stops the HM-10 from auto-connecting
                                // waits for CONNECTION COMMANDS
    delay(TX_DELAY_MS);

//    HM10_Serial.print("AT+IMME0"); // AUTO CONNECT TO NEAREST SLAVE
//    AT+DISC? & AT+CON<MAC> NOT REQUIRED WITH AT+IMME0 
    
    // Set to Central mode with “AT+ROLE1″
    HM10_Serial.print("AT+ROLE1"); // set as MASTER/CENTRAL
    delay(TX_DELAY_MS);
    
    HM10_Serial.print("AT+RESET"); // RESET MODULE
    delay(TX_DELAY_MS);
    
//    Serial.println("Sent: \tAT+DISC?");
//    HM10_Serial.print("AT+DISC?"); //DISCOVER ONLY SLAVE/PERIPHERAL DEVICES: AT+DISC?
//    delay(TX_DELAY_MS);
    
//    HM10_Serial.print("AT+CONN0");   delay(TX_DELAY_MS);
//    if(scan_resp.indexOf("C8FD199C8598") > 0)
//    { Serial.println("FOUND C8FD199C8598\n");

//      Serial.println("trying to connect to C8FD199C8598\n");  // BLE3
//      HM10_Serial.print("AT+CONC8FD199C8598");

      Serial.println("trying to connect to C8FD194AF746\n");  // BLE2
      HM10_Serial.print("AT+CONC8FD194AF746");
//    }
    delay(TX_DELAY_MS);

//    Serial.println("CALLING TX_RX();\n");    TX_RX(); 

    Serial.println("HM10 MASTER SETUP ENDS");     
    delay(50);
}
