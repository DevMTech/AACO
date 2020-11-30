/*-------------------------------------------------
 * Purpose: To read RFID Tag periodically
 * Author: Kaushal 
 * Date: 26/09
 */


#include<SoftwareSerial.h>


//Set the frequency to read RFID Tags in Seconds
#define TIME_INTERVAL 3  

//Tag Id to check - 10 digits
//#define TAG {'4','4','0','0','1','8','7','6','2','8','0','2'}
#define TAG {'1','9','0','0','4','B','0','E','8','4','D','8'}
//19004B0E84D8

//Programmatical Serial Port
SoftwareSerial ser(10,11);


int  val = 0; 

//Reading Buffer
char code[12]; 

//The artefact TAG value
char Check_Tag[12] = TAG;

int bytesread = 0; 

int EQUAL = 1;

int Counter = 0;


void setup() {
   Serial.begin(9600);                      // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
   pinMode(2,OUTPUT);                       // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin 
   digitalWrite(2, HIGH);                   // Activate the RFID reader

   while(!Serial);
   Serial.println("Main program starts");
   
   //Serial Port is ready
   ser.begin(9600);
  

}

void loop() {
 if(ser.available() > 0) {      
    
    
    val = ser.read();         
        
    code[bytesread] = val; 
    
    bytesread++;  

     if(bytesread == 12) {                // 10 ASCII DATA （card no.）+ 2 ASCII DATA （XOR result）

        EQUAL = 1;
        
        Serial.println();
        Serial.print("TAG code is: ");   // RFID Tag Read value 
        Serial.println(code);

        /*
        //Check if the Read Tag is the desired TAG
        for(int i =0; i<12;i++)
        {
           if(code[i]!=Check_Tag[i])
              {
                EQUAL = 0;
                break;
              }
        }*/
        
        bytesread = 0;                                  // RESET buffer 
        ser.end();
        digitalWrite(2, LOW);                           // deactivate the RFID reader for a moment so it will not flood
        /*
        if(EQUAL)
        {
          //Serial.println("Tag is Present");
          Serial.println("Artefact Identified");
        }
        else
        {
          //Serial.println("Tag is Missing!");
          Serial.println("Unknown Artefact");
        }
        */
        //delay(TIME_INTERVAL*1000);                      // wait for a bit 
        digitalWrite(2, HIGH);                          // Activate the RFID reader
        ser.begin(9600);
        //Counter=0;  
        
        delay(100);
      }   
  }
  /* 
  else
  {
      delay(1000);
      Counter++;

      if(Counter>TIME_INTERVAL)
      {
        Serial.println("Artefact is Missing!");
        Counter=0;
      }
  }*/
}
