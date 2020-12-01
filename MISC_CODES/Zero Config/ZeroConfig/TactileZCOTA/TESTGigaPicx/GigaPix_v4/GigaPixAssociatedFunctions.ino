// COMMON - SERVER
const char* gigapixHost = "http://10.208.26.254:81";             //main - Server address
String gigapixPostPath = "/gigapan/index.php";                 //main - Post service Path
//http://10.208.26.254:81/gigapan/index.php

//const char* gigapixHost = "http://10.208.34.80:3301";             //main - Server address
//String gigapixPostPath = "/data";

#define MAX_SWITCHES_ALLOWED 40

int Single=0,Double=1,Hold=2;
int bounceTime = 50;
int holdTime = 250;
long doubleTime = 500;
int hold[MAX_SWITCHES_ALLOWED] = {0}; //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
long onTime[MAX_SWITCHES_ALLOWED] = {0}; //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
long lastSwitchTime[MAX_SWITCHES_ALLOWED] = {0}; //  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


bool singlePressFlag[] ={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
                 false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool doublePressFlag[] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
                 false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool isSingleClick[]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
                 false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};                  
bool isDoubleClick[]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
                 false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
int reading[] ={LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,
                        LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW}; 
int lastReading[] = {LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,
                        LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW};

const int analogPin = A0;
//int reading[MAX_SWITCHES_ALLOWED];
int totalSwitches = 17, switchNum=0, i=0, j=0, adc_max = 1024;    // buf = 0, last_switch=0, press_count=0, 
float sensorValue = 0;    // , s, t, s_map;
float unit_step = adc_max/(totalSwitches+1);

void gigapixLoop()
{
  if(totalSwitches>=MAX_SWITCHES_ALLOWED)
  {   Serial.println("\n\n\tFATAL ERROR!! Total Switches ("+String(totalSwitches)+") exceeds limit ("+String(MAX_SWITCHES_ALLOWED)+") !!");
  }
  else
  {     for(j=0; j<totalSwitches; j++) {reading[j]=LOW;}     
          
        sensorValue=analogRead(analogPin);
        if(sensorValue>45){Serial.println(String("\n\tSensor value: ")+sensorValue);}
        delay(5);
      
        switchNum=0;
        if (sensorValue>(unit_step*0.7)){switchNum = round(sensorValue/unit_step);} // {switchNum = round((sensorValue+buf)/unit_step);}
      
        if(switchNum>0) 
          {  Serial.print(String(" -> switch ")+(switchNum)+String(" pressed "));
             i=switchNum-1; 
             reading[i] = HIGH;                       
          } 

        if (reading[i] == HIGH && lastReading[i] == LOW) { onTime[i] = millis(); }
      
             if (reading[i] == HIGH && lastReading[i] == HIGH) 
             {
                 if ((millis() - onTime[i]) > holdTime) 
                   {   Serial.print("Hold Clicked : ");
                       Serial.println(i+1);  //  (getButtonName(i));
                       gigapixPostData(gigapixPostPath, i+1, Hold); //  getButtonName(i), Hold);  //POST HOLD
                       hold[i] = 1;
                   }
             }
      
             if (reading[i] == LOW && lastReading[i] == HIGH) 
             {    if (((millis() - onTime[i]) > bounceTime) && hold[i] != 1) { doubleClickCheck(i);  }
                  if (hold[i] = 1) {  hold[i] = 0;  }   
             }
      
              if(isSingleClick[i]==true && isDoubleClick[i]==false && (millis() - lastSwitchTime[i]) 
                                    > doubleTime && reading[i]==LOW && lastReading[i]==LOW)
              {   Serial.print("Single Clicked : ");
                  Serial.println(i+1);  //  (getButtonName(i));
                  gigapixPostData(gigapixPostPath, i+1, Single);  //  getButtonName(i), Single); //POST SINGLE          
                  singlePressFlag[i]=true;
                  isSingleClick[i]=false;
                  isDoubleClick[i]=true;          
              }   
             lastReading[i] = reading[i];        
             if(singlePressFlag[i]==true) { singlePressFlag[i]=false;  isDoubleClick[i]=false; }
             if(doublePressFlag[i]==true){  doublePressFlag[i]=false; }     
  }                
}

void doubleClickCheck(int i) 
{
       if ((millis() - lastSwitchTime[i]) < doubleTime) 
       { Serial.print("Double Clicked : ");
         Serial.println(i+1);  //  (getButtonName(i));
         gigapixPostData(gigapixPostPath, i+1, Double); //  getButtonName(i), Double);  //POST DOUBLE   
         doublePressFlag[i]=true;
         isDoubleClick[i]=true;
       }
       else { isDoubleClick[i]=false;  }
       
       if ((millis() - lastSwitchTime[i]) > doubleTime) { isSingleClick[i]=true; }
       lastSwitchTime[i] = millis();
}

//POST data to Server
bool gigapixPostData(String path, int switchNum, int type)
{  
        HTTPClient httpPost;  
        Serial.println(String("\n\tSensor value: ")+sensorValue);
        httpPost.begin(serverUrl);
      //  httpPost.begin(applicationServerUrl);
      //  httpPost.begin(gigapixHost + path);
        httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
        String st_switchNum = String(switchNum);
        
        Serial.println("\npointID: " + st_switchNum + ", type: " + type + ", "+ "pointsSubmit: 1");
        int httpCode = httpPost.POST("pointID=" + st_switchNum + "&" + "type=" + type + "&" + "pointsSubmit=1");
         
        httpPost.end();
        
        Serial.println(httpCode);  
          
        if(httpCode == 200)
        {  
          Serial.println("POST success\n\n");
          delay(500);
          return true;
        }
        else  { return false; }    
}

int getButtonName(int gpioPin)
{        return (gpioPin+1);
}
