/*-------------------------------------------------
 * Purpose: To read RFID Tag periodically
 */

#include<SoftwareSerial.h>

//Set the frequency to read RFID Tags in Seconds
//#define TIME_INTERVAL 3  
//Tag Id to check - 10 digits
//#define TAG {'1','9','0','0','4','B','0','E','8','4','D','8'}
//19004B0E84D8

SoftwareSerial ser(10,11); //Serial Port

int  val = 0, bytesread = 0; 
char code[12]; //Reading Buffer
//char Check_Tag[12] = TAG; //The artefact TAG value

void setup() 
{
   Serial.begin(9600);                      // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
   pinMode(2,OUTPUT);                       // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin 
   digitalWrite(2, HIGH);                   // Activate the RFID reader

   while(!Serial);
   Serial.println("Main program starts");  
   ser.begin(9600); //Serial Port is ready
}

void loop() 
{
 if(ser.available() > 0) 
 {        
    val = ser.read();    
    code[bytesread] = val;     
    bytesread++;  

     if(bytesread == 12)                 // 10 ASCII DATA （card no.）+ 2 ASCII DATA （XOR result）
      {         
        Serial.println();
        Serial.print("TAG code is: ");   // RFID Tag Read value 
        Serial.println(code);
        
        bytesread = 0;                                  // RESET buffer 
        ser.end();
        digitalWrite(2, LOW);                           // deactivate the RFID reader for a moment so it will not flood
    
        //delay(TIME_INTERVAL*1000);                      // wait for a bit 
        digitalWrite(2, HIGH);                          // Activate the RFID reader
        ser.begin(9600);
       
        delay(2000);
      }   
  }
 }
