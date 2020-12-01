// COMMON - SERVER
const char* gigapixHost = "http://10.208.34.80:3301";             //main - Server address
//String gigapixPostPath = "/gigapan/index.php";                 //main - Post service Path
String gigapixPostPath = "/data";


const int totalSwitches=17;
long onTime[] = {0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0};
int Single=0,Double=1,Hold=2;
int lastReading[] = {LOW,LOW,LOW,LOW,LOW ,LOW,LOW,LOW,LOW,LOW ,LOW,LOW,LOW,LOW,LOW ,LOW,LOW};
int bounceTime = 50;
int holdTime = 250;
int hold[] = {0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0};

long lastSwitchTime[] = {0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0 ,0,0};
long doubleTime = 500;

bool singlePressFlag[] ={false,false,false,false,false ,false,false,false,false,false ,false,false,false,false,false ,false,false}, 
doublePressFlag[] = {false,false,false,false,false ,false,false,false,false,false ,false,false,false,false,false ,false,false};

bool isSingleClick[]={false,false,false,false,false ,false,false,false,false,false ,false,false,false,false,false ,false,false}, 
isDoubleClick[]={false,false,false,false,false ,false,false,false,false,false ,false,false,false,false,false ,false,false};

const int analogPin = A0;

void gigapixLoop()
{
  int reading[]={LOW,LOW,LOW,LOW,LOW ,LOW,LOW,LOW,LOW,LOW ,LOW,LOW,LOW,LOW,LOW ,LOW,LOW};
 
  int sensorValue=analogRead(A0);
  //Serial.println(sensorValue);
  delay(5);
  
  if(sensorValue>=990){
    reading[16] = HIGH; //digitalRead(button);
  }else{
    reading[16] = LOW;
  }
 if(sensorValue>=930 && sensorValue<=960){
    reading[15] = HIGH; //digitalRead(button);
  }else{
    reading[15] = LOW;
  }
 if(sensorValue>=850 && sensorValue<=900){
    reading[14] = HIGH; //digitalRead(button);
  }else{
    reading[14] = LOW;
  }
 if(sensorValue>=800 && sensorValue<=840){
    reading[13] = HIGH; //digitalRead(button);
  }else{
    reading[13] = LOW;
  }
 if(sensorValue>=750 && sensorValue<=780){
    reading[12] = HIGH; //digitalRead(button);
  }else{
    reading[12] = LOW;
  }
 if(sensorValue>=680 && sensorValue<=730){
    reading[11] = HIGH; //digitalRead(button);
  }else{
    reading[11] = LOW;
  }
 if(sensorValue>=630 && sensorValue<=670){
    reading[10] = HIGH; //digitalRead(button);
  }else{
    reading[10] = LOW;
  }
 if(sensorValue>=570 && sensorValue<=610){
    reading[9] = HIGH; //digitalRead(button);
  }else{
    reading[9] = LOW;
  }
 if(sensorValue>=510 && sensorValue<=550){
    reading[8] = HIGH; //digitalRead(button);
  }else{
    reading[8] = LOW;
  }
 if(sensorValue>=460 && sensorValue<=490){
    reading[7] = HIGH; //digitalRead(button);
  }else{
    reading[7] = LOW;
  }
 if(sensorValue>=400 && sensorValue<=430){
    reading[6] = HIGH; //digitalRead(button);
  }else{
    reading[6] = LOW;
  }
 if(sensorValue>=340 && sensorValue<=370){
    reading[5] = HIGH; //digitalRead(button);
  }else{
    reading[5] = LOW;
  }
 if(sensorValue>=280 && sensorValue<=310){
    reading[4] = HIGH; //digitalRead(button);
  }else{
    reading[4] = LOW;
  }
 if(sensorValue>=220 && sensorValue<=250){
    reading[3] = HIGH; //digitalRead(button);
  }else{
    reading[3] = LOW;
  }
 if(sensorValue>=170 && sensorValue<=200){
    reading[2] = HIGH; //digitalRead(button);
  }else{
    reading[2] = LOW;
  }
 if(sensorValue>=100 && sensorValue<=140){
    reading[1] = HIGH; //digitalRead(button);
  }else{
    reading[1] = LOW;
  }
 if(sensorValue>=50 && sensorValue<=80){
    reading[0] = HIGH; //digitalRead(button);
  }else{
    reading[0] = LOW;
  }
 
  for(int i=0;i<totalSwitches;i++)
  {

 if (reading[i] == HIGH && lastReading[i] == LOW) {
   onTime[i] = millis();
 }

 if (reading[i] == HIGH && lastReading[i] == HIGH) {
   if ((millis() - onTime[i]) > holdTime) {
     
   Serial.print("Hold Clicked : ");
  Serial.println(getButtonName(i));
       gigapixPostData(gigapixPostPath, getButtonName(i),Hold);
     hold[i] = 1;
   }
 }

 if (reading[i] == LOW && lastReading[i] == HIGH) {
   if (((millis() - onTime[i]) > bounceTime) && hold[i] != 1) {
     doubleClickCheck(i);
   }
   if (hold[i] = 1) {
     hold[i] = 0;
   }   
 }

    if(isSingleClick[i]==true && isDoubleClick[i]==false && (millis() - lastSwitchTime[i]) 
                          > doubleTime && reading[i]==LOW && lastReading[i]==LOW)
    {
      Serial.print("Single Clicked : ");
      Serial.println(getButtonName(i));
      gigapixPostData(gigapixPostPath, getButtonName(i), Single);
  
      singlePressFlag[i]=true;
      isSingleClick[i]=false;
      isDoubleClick[i]=true;

    }
 
 lastReading[i] = reading[i];

 if(singlePressFlag[i]==true)
 {
  singlePressFlag[i]=false;
  isDoubleClick[i]=false;
 }
 if(doublePressFlag[i]==true){
  doublePressFlag[i]=false; 
 }
}
}


//POST data to Server
bool gigapixPostData(String path, int sw, int type)
{
  HTTPClient httpPost;  
  
  httpPost.begin(gigapixHost + path);
  httpPost.addHeader("Content-Type","application/x-www-form-urlencoded");
  String st_sw = String(sw);
  
  Serial.println("pointID:" + st_sw + ","+ "pointsSubmit:1");
  int httpCode = httpPost.POST("pointID=" + st_sw + "&" + "type=" + type + "&" + "pointsSubmit=1");
   
  httpPost.end();
  
  Serial.println(httpCode);  
    
  if(httpCode == 200)
  {  
    Serial.println("POST success");
    return true;
  }
  else  { return false; }    
}

void doubleClickCheck(int i) 
{
 if ((millis() - lastSwitchTime[i]) < doubleTime) {
  
    Serial.print("Double Clicked : ");
   Serial.println(getButtonName(i));
     gigapixPostData(gigapixPostPath, getButtonName(i), Double);

     doublePressFlag[i]=true;
     isDoubleClick[i]=true;
 }
 else{
  isDoubleClick[i]=false;
 }
 if ((millis() - lastSwitchTime[i]) > doubleTime) {
   isSingleClick[i]=true;
}
 lastSwitchTime[i] = millis();
}

int getButtonName(int gpioPin)
{
  return (gpioPin+1);
}
