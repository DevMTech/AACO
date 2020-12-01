//long randNumber;
//String str;

void IntCallback_sw1()
{ switch_seq(1);
}

void IntCallback_sw2()
{ switch_seq(2);
}

void IntCallback_sw3()
{ switch_seq(3);
}

void IntCallback_sw4()
{ switch_seq(4);
}

void IntCallback_sw5()
{ switch_seq(5);
}

void setup()
{
 Serial.begin(115200); delay(50);
// Serial1.begin(115200);
 
 pinMode( 2, INPUT);   // GPIO5 = D1 = sw1
 pinMode(20, INPUT);  // GPIO0 = D3 = sw2
 pinMode( 3, INPUT);   // GPIO4 = D2 = sw3
 pinMode(19, INPUT);  // GPIO0 = D3 = sw4
 pinMode(18, INPUT);  // GPIO0 = D3 = sw5
 
 attachInterrupt(digitalPinToInterrupt(2), IntCallback_sw1, RISING);
 attachInterrupt(digitalPinToInterrupt(20), IntCallback_sw2, RISING);
 attachInterrupt(digitalPinToInterrupt(3), IntCallback_sw3, RISING);
 attachInterrupt(digitalPinToInterrupt(19), IntCallback_sw4, RISING);
 attachInterrupt(digitalPinToInterrupt(18), IntCallback_sw5, RISING); 
  

// randomSeed(analogRead(0)); // unconnected analog input pin
// Serial.print(char(169)); // Copyright Symbol
// Serial.println(" TEST STRING ");
// delay(100);

}

void loop()
{ 
//  String str = "HELLO"; // "HELLO - Mega -> serial comm. -> ESP12E"; //\0"

//  randNumber = random(1,18);// print a random number from 1 to 17
//  Serial.println(randNumber);

//  Serial.print(str);
//  Serial.print(" randNumber :"); Serial.println(String(randNumber));

//  Serial1.println("HELLO - NodeMCU");
//  delay(10000);
}

void switch_seq(int sw)
{ if(sw)
  { //Serial.print(String(" -> ")+sw);
//    Serial.print(String(" -> switch ")+(sw)+String(" touched "));
//    switch(sw):  
//    {   case 1: 
//    }
//    Serial.println("Switch "+String(sw));
    Serial.println(sw);
    delay(100);
//    if(sw!=last_sw && count < maxCount)           
//    { code*=10;
//      code+=sw; 
//      count++;      
//    }
//    
//    last_sw = sw;
    
//    if(count==maxCount)
//    { Serial.println("\n\n Code : "+String(code));
//      if(code==matchCode)
//      { Serial.println("\n Success !!");
//      }
//      else
//      { Serial.println("\n Failure !!");
//      }
//      count = 0; code=0; last_sw=0;
//    }        
//    Serial.println("");

  }
}
