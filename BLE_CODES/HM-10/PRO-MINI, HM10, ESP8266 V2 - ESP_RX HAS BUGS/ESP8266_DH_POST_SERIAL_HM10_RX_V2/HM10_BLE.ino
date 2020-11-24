//#define HM10_CONFIG HM10_Serial.print

void HM10_setup()
{   Serial.println("HM10 MASTER SETUP STARTS"); 
    //    pinMode(4, OUTPUT); // D4 = EN
    //    digitalWrite(4, HIGH); 
    
    HM10_Serial.begin(9600); // set the data rate for the SoftwareSerial port
    Serial.println("I am MASTER/CENTRAL"); //

//    HM10_CONFIG("AT"); // 
//    HM10_CONFIG("AT+RESET"); // RESET MODULE
    
    // AT+RENEW // reset to the default settings
    HM10_CONFIG("AT+IMME1"); // Set to manual connection with “AT+IMME1″ 
                                // stops the HM-10 from auto-connecting
                                // waits for CONNECTION COMMANDS

//    HM10_Serial.print("AT+IMME0"); // AUTO CONNECT TO NEAREST SLAVE
//    AT+DISC? & AT+CON<MAC> NOT REQUIRED WITH AT+IMME0 
    
    // Set to Central mode with “AT+ROLE1″
    HM10_CONFIG("AT+ROLE1"); // set as MASTER/CENTRAL

    
    HM10_CONFIG("AT+RESET"); // RESET MODULE

    
//    Serial.println("Sent: \tAT+DISC?");
//    HM10_CONFIG("AT+DISC?"); //DISCOVER ONLY SLAVE/PERIPHERAL DEVICES: AT+DISC?    
//    HM10_Serial.print("AT+CONN0");   delay(TX_DELAY_MS);

      Serial.println("trying to connect to "+SLAVE_MAC_2+"\n");  // C8FD199C8598
      HM10_CONFIG("AT+CON"+SLAVE_MAC_2);

//    Serial.println("CALLING TX_RX();\n");    TX_RX(); 

    Serial.println("HM10 MASTER SETUP ENDS");     
    delay(50);
}

void HM10_CONFIG(String Config)
{
    Serial.println("Sent : "+Config); //+" : ");
    HM10_Serial.print(Config); // AT+MODE2: set pins PIO2-B as O/P pins
    delay(TX_DELAY_MS);
//    HM10_RESPONSE();
}

/*void HM10_RESPONSE()
{   String  HM10response;
    
    if (HM10_Serial.available()) // READ
    { 
      HM10response = HM10_Serial.readString();  // Until('E');      // delay(100);
      Serial.println("Received: " + HM10response); // RESPONSE
      delay(TX_DELAY_MS);
      
      if(HM10response.indexOf(SLAVE_MAC_1) > 0)
      { Serial.println("FOUND "+SLAVE_MAC_1+"\n");  delay(TX_DELAY_MS);
      }
//      else
//      { HM10_CONFIG("AT+CON"+SLAVE_MAC_1);        
//      }

//      if(ReadString.indexOf("END") > 0)
//      { Serial.println("FOUND END\n");
//      }
    }
    else
    {Serial.println("Received: NO RESPONSE FROM HM10!!"); delay(TX_DELAY_MS);
    }
//    delay(TX_DELAY_MS);
}*/
