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

    delay(TX_DELAY_MS);

//    Serial.println("CALLING TX_RX();\n");    TX_RX(); 
    
    Serial.println("HM10 MASTER SETUP ENDS");     
    delay(50);
}

void HM10_Serial_Read()
{   Serial.println("SERIAL READ STARTS\n");
//  while (!Serial) 
//    { ; // wait for serial port to connect. Needed for native USB port only
//    }  
  
//  String ReadString = "C1589T29.24P941.31A616.92H63.01L115.07"; /*DUMMY STRING FOR TESTING PURPOSE*/
//  String ReadString = "T24.94H53.01L515.07"; /*DUMMY STRING FOR TESTING PURPOSE*/

//    String ReadString = Serial.readStringUntil('E');  // C1589T29.24P941.31A616.92H63.01L115.07
//    String  ReadString=Serial.readString();
  
  int T, P, A, H, L;  

//  if (Serial.available())   // WRITE
//    { 
//      WriteString = Serial.readString();      // delay(10);
//      Serial.println("Sent: \t" + WriteString); // delay(10);
//      HM10_Serial.print(WriteString);
//    }  

    if (HM10_Serial.available()) // READ
    { 
      ReadString = HM10_Serial.readString();  // Until('E');      // delay(100);
      Serial.println("Received: " + ReadString); // RESPONSE

      if(ReadString.indexOf("E") > 0)
      { Serial.println("FOUND END\n");
      }

      
      if((ReadString.indexOf("LOST") > 0)||(ReadString.indexOf("OK+CONNF") > 0))
      {
        HM10_Serial.print("AT+RESET"); // RESET MODULE
//        HM10_setup();
        delay(TX_DELAY_MS);
      }
    
    }  

  int Str_size = ReadString.length();
  
  if(Str_size)
  {
    Serial.println("\nRead String : "+String(ReadString)+"\n"); 
  
    T=ReadString.indexOf('T');
//    P=ReadString.indexOf('P');
//    A=ReadString.indexOf('A');
    H=ReadString.indexOf('H');
    L=ReadString.indexOf('L');
    
    tempC=ReadString.substring(T+1,H);
    humidity=ReadString.substring(H+1,L);
    lux=ReadString.substring(L+1,Str_size);

         tempC="25.14";
//     pressure="941.31";
//     alti="616.92";
     humidity="60.01";
     lux="415.07";

//    tempC=ReadString.substring(T+1,P);
//    pressure=ReadString.substring(P+1,A);
//    alti=ReadString.substring(A+1,H);
//    humidity=ReadString.substring(H+1,L);
//    lux=ReadString.substring(L+1,Str_size);

  }
  Serial.println("SERIAL READ ENDS\n");
}
