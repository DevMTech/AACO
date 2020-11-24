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
 
//WiFiServer server(80);
 
void WEBSERVER_setup() 
{
    server.begin(); // Start the server
    Serial.println("Server started");
}
 
void WEB_loop() {
    
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
//  if (request.indexOf("/bat=ON") != -1)  {
//  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
//  client.println("DEVICE MAC =");
  client.println("Temperature(C) =");
  client.print(tempC);
  client.println("<br>");

//  client.println("STATUS =");
  client.println("Humidity(%) =");
  client.print(humidity);
  client.println("<br>");

  client.println("Lux =");
  client.print(lux);
  client.println("<br>");

//  <a href=\" /bat=ON \" \" >
     
  client.println("<br><br>");
//  client.println("<a href=\"/bat=ON\"\"><button>Status</button></a><br />"); 
  client.println("<a href=\"/device=REFRESH\"\"><button>REFRESH</button></a><br />");  
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println(""); 
}
