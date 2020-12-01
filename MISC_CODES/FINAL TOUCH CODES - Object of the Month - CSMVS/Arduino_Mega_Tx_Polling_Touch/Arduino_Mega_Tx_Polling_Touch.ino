//#include <dht.h>
//dht DHT;
//int temp,humi;
//#define DHT11_PIN A0
int count=0;
void setup()
{
 Serial.begin(115200); delay(50);
// Serial1.begin(115200);
 
 pinMode( 2, INPUT);   // GPIO5 = D1 = sw1
// pinMode(20, INPUT);  // GPIO0 = D3 = sw2
// pinMode( 3, INPUT);   // GPIO4 = D2 = sw3
// pinMode(19, INPUT);  // GPIO0 = D3 = sw4
// pinMode(18, INPUT);  // GPIO0 = D3 = sw5
  

// randomSeed(analogRead(0)); // unconnected analog input pin
// Serial.print(char(169)); // Copyright Symbol
// Serial.println(" TEST STRING ");
// delay(100);
}

void loop()
{ int sw=0;
  if(digitalRead( 2)==HIGH) sw=1;
//  if(digitalRead(20)==HIGH) sw=2;
//  if(digitalRead( 3)==HIGH) sw=3;
//  if(digitalRead(19)==HIGH) sw=4;
//  if(digitalRead(18)==HIGH) sw=5;
 
  if(sw)Serial.println(String(count++)+" Switch "+String(sw)); //Serial.println(sw);

  delay(165);
}


