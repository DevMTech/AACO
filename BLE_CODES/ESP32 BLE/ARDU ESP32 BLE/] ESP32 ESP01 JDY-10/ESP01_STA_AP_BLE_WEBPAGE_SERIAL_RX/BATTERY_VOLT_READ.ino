/*****************************************************
 * ADC
  ----- 
* ESP8266 has one ADC, but it has a  voltage range: 0 - 1V, voltages above 1V might damage the board.
  The ADC has a resolution of 10 bits. You could use a resistive voltage divider or a trimpot as a voltage divider.
  
Analog input:
* You can use analogRead(A0) to get the analog voltage on the analog input. (0 = 0V, 1023 = 1.0V).

Measure supply voltage (VCC):
* The ESP can also use the ADC to measure the supply voltage (VCC). 
* For this, 
    include ADC_MODE(ADC_VCC); at the top of your sketch, and use 
    ESP.getVcc(); to actually get the voltage.
* If you use it to read the supply voltage, you can’t connect anything else to the analog pin 
* 
* The powerconsumption of an ESP8266 in deepsleep is about 77uA. 
* With the battery monitor this would be 87uA, which is a sizeable increase. 
* A solution could be to close off the Vbat to the A0 with a transistor, controlled from an ESP8266 pin
* 
 *****************************************************
 *  https://www.microcontroller-project.com/nodemcu-battery-voltage-monitor.html
 * RatioFactor = V-batt. / V-A0 = (R1 + R2) / R2
 *  TAKE R2 as 10k OR 100k & CALCULATE R1 AS REQUIRED, 
 *  V-A0 = 1V
 *  VIN AS PER BATTERY USED (SAY VIN = 5V FOR 5V BATTERY)
 *  
 *  battery(V-batt) - |
 *                    |
 *                    R1
 *                    |
 *                    |---- 1V (V-A0) - ESP8266 A0 (ANALOG I/P PIN)
 *                    |
 *                    R2 (10k or 100k)
 *                    |
 *                    |
 *                   GND (ESP8266)
 *                    
 *  uint16 adcValue = system_adc_read();
    os_printf("adc = %d\n", adcValue);                 
 * ***************************************************/

/* 
#include <ESP8266WiFi.h>
 
const char* ssid = "ASUS_X00TD";
const char* password = "4652b298f";

float R2 = 10.0, R1 = 46.4; // 21.6;
int BAT= A0;              //Analog channel A0 as used to measure battery voltage
unsigned int raw=0;
float volt=0.0;
// RatioFactor = (R1 + R2) / R2 : CALCUATE USING ABOVE FORMULAE AND MODIFY VALUE OF RatioFactor AS NECESSARY
float RatioFactor=(R1 + R2) / R2;  // 5.8125; // 5.714;  //Resistors Ration Factor 
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(A0, INPUT);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin(); // Start the server
  Serial.println("Server started");
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  int value = LOW;
  float Tvoltage=0.0;
  float Vvalue=0.0,Rvalue=0.0;

  raw = analogRead(A0);
  Serial.print("\nraw: ");
  Serial.println(raw);
  volt=raw*3.3*RatioFactor/1023.0; 
  Serial.print("\nBattery Volt: ");
  Serial.println(volt);
  Serial.println();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  
  if (request.indexOf("/bat=ON") != -1)  
  {
    /////////////////////////////////////Battery Voltage//////////////////////////////////  
    Serial.println("analogRead(BAT):\n");
    for(unsigned int i=0;i<10;i++)
    {
      Vvalue=Vvalue+analogRead(BAT);         //Read analog Voltage
      delay(5);                              //ADC stable
  //    Serial.println(analogRead(BAT));
    }
    Vvalue=(float)Vvalue/10.0;            //Find average of 10 values
    Rvalue=(float)(Vvalue/1023.0)*3.3;      //Convert Voltage in 5v factor  
  //  Rvalue=(float)(Vvalue/1024.0);
  
    Tvoltage=Rvalue*RatioFactor;          //Find original voltage by multiplying with factor
    Serial.print("\nBattery Voltage = ");
    Serial.print(Tvoltage);
    Serial.println(" V\n");
      /////////////////////////////////////Battery Voltage//////////////////////////////////
      value = HIGH;
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.println("Battery Voltage =");
  client.print(Tvoltage);
  client.println("<br>");

  if(value == HIGH) {
    client.println("Updated");
  } else {
    client.print("Not Updated");
  }
  client.println("--------");
  if(Tvoltage<=5){
    client.println("Battery dead OR disconnected");
    }
  else if(Tvoltage>5 && Tvoltage<=10){
    client.println("Need Imediate recharge");
    }
  else if(Tvoltage>10 && Tvoltage<=12){
    client.println("Recharge");
    }
  else{
      client.println("Battery Full");
      }
     
  client.println("<br><br>");
  client.println("<a href=\"/bat=ON\"\"><button>Status</button></a><br />");  
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
*/
