//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//
//const char* ssid = "cdac";
//const char* password = "";
//
//ESP8266WebServer server(80);

const int led = 13;

int TIMER = 1000;

//void handleConfigParamsRoot() 
//{ 
//  String s;
//  s = "Enter Configuration Parameters of " + hostName;
////  s += "<p><a href=\"/gpio\">Control GPIO</a><br />";
////  s += "<a href=\"/configParams\">Edit Config Parameters</a><br />";
////  s += st;                                        // st -> List of available WiFi Hotspots
////  s += "<p><form method='get' action='a'>";
//
////  s += "<p><form method='get' action='b'>";         // ACTION ID = /b
//
//  s += "\r\n\r\n<form method='get' action='b'><fieldset><legend>Configuration Parameters</legend><ul class=form-fields>";
//    
////  s += "<label>SSID: </label><input name='ssid' length=32> <label> Pass: </label><input name='pass' type='password' length=64></br>";
////  s += "The following is not ready yet!</br>";
////  s += "<label>IOT mode: </label><input type='radio' name='iot' value='0'> HTTP<input type='radio' name='iot' value='1' checked> MQTT</br>";
////  
//
//  s += "<label>PLATFORM URL [http://10.208.35.169:3300]: </label><input name='platformUrl' length=64></br>";       //10.208.35.169 //3300
//  
////  s += "<label>PORT[3300]: </label><input name='restPort' length=8></br>";
//
//  s += "<label>APPLICATION SERVER URL [http://10.208.26.254:81/gigapan/index.php]: </label><input name='serverUrl' length=64></br>";  //http://10.208.26.254:81/gigapan/index.php
//
//  s += "<label>BUTTON Nos.: </label><input name='buttonNos' length=8></br>";
//  
//  s += "<label>BUTTON ID: </label><input name='buttonID' length=8></br>";
//
//  s += "<label>BUTTON VALUE: </label><input name='buttonValue' length=64></br>";
//  
////  s += "<label>MQTT Broker IP/DNS: </label><input name='host' length=15></br>";
////  s += "<label>MQTT Publish topic: </label><input name='pubtop' length=64></br>";
////  s += "<label>MQTT Subscribe topic: </label><input name='subtop' length=64></br>";
//
//  s += "</ul></fieldset><input type='submit'></form>";
////  s += "<input type='submit'></form></p>";
//  s += "\r\n\r\n";
//  Serial.println("Sending 200");  
//  server.send(200, "text/html", s);
//}

void handleConfigSave(){
  // /a?ssid=blahhhh&pass=poooo
  bool configChange = false;
           
  String qsid; 
  qsid = server.arg("ssid");
  qsid.replace("%2F","/");
  if(qsid.length())
    { configChange = true;
      Serial.println("Got SSID: " + qsid);
      esid = (char*) qsid.c_str();
    }
  else {Serial.println(" esid: No value entered.");}
  
  String qpass;
  qpass = server.arg("pwd");
  qpass.replace("%2F","/");
  if(qpass.length())
    { configChange = true;
      Serial.println("Got pwd: " + qpass);
      epass = (char*) qpass.c_str();
    }
  else {Serial.println(" epass: No value entered.");}
  
//
//  String qiot;
//  qiot= server.arg("iot");
//  Serial.println("Got iot mode: " + qiot);
//  qiot=="0"? iotMode = 0 : iotMode = 1 ;
//

  String qUrl;
  qUrl = server.arg("platformUrl");
  qUrl.replace("%2F","/");
  if(qUrl.length())
  { configChange = true;
    Serial.println("Got platformUrl: " + qUrl);
    platformUrl = (char*) qUrl.c_str();
  }
  else {Serial.println(" platformUrl: No value entered.");}

//  String qPort;
//  qPort = server.arg("restPort");
//  qPort.replace("%2F","/");
//  if(qPort.length())
//  { configChange = true;
//    Serial.println("Got restPort: " + qPort);
//    restPort = (char*) qPort.c_str();
//  }
//  else {Serial.println(" restPort: No value entered.");}

  String qserverUrl;
  qserverUrl = server.arg("serverUrl");
  qserverUrl.replace("%2F","/");
  if(qserverUrl.length())
  { configChange = true;
    Serial.println("Got serverUrl: " + qserverUrl);
    serverUrl = (char*) qserverUrl.c_str();
  }
  else {Serial.println(" serverUrl: No value entered.");}
   
  String qbuttonNos;
  qbuttonNos = server.arg("buttonNos");
  qbuttonNos.replace("%2F","/");
  if(qbuttonNos.length())
  { configChange = true;
    Serial.println("Got buttonNos: " + qbuttonNos);
    //String sbuttonNos = (char*) qUrl.c_str();
    buttonNos = qbuttonNos.toInt();
  }
  else {Serial.println(" buttonNos: No value entered.");}

  String qbuttonID;
  qbuttonID = server.arg("buttonID");
  qbuttonID.replace("%2F","/");
  if(qbuttonID.length())
  { configChange = true;
    Serial.println("Got buttonID: " + qbuttonID);
    //String sbuttonID = (char*) qUrl.c_str();
    buttonID = qbuttonID.toInt();
  }
  else {Serial.println(" buttonID: No value entered.");}

  String qbuttonValue;
  qbuttonValue = server.arg("buttonValue");
  qbuttonValue.replace("%2F","/");
  if(qbuttonValue.length())
  { configChange = true;
    Serial.println("Got buttonValue: " + qbuttonValue);
    buttonValue = (char*) qbuttonValue.c_str();
  }
  else {Serial.println(" buttonValue: No value entered.");}

//  String qsubTop;
//  qsubTop = server.arg("subtop");
//  qsubTop.replace("%2F","/");
//  Serial.println("Got subtop: " + qsubTop);
//  subTopic = (char*) qsubTop.c_str();
//  
//  String qpubTop;
//  qpubTop = server.arg("pubtop");
//  qpubTop.replace("%2F","/");
//  Serial.println("Got pubtop: " + qpubTop);
//  pubTopic = (char*) qpubTop.c_str();
//
//  mqttServer = (char*) server.arg("host").c_str();
//  Serial.print("Got mqtt Server: ");
//  Serial.println(mqttServer);

  /*CHECK IF REST URL, PORT HAS BEEN CHANGED THEN REGISTER DEVICE TO THE NEW ADDRESS */
  //Serial.println("\n\n\tRegistering the device to the new REST address");
  //registerDevice();

  String s;

  if(configChange)
  { Serial.println("[handleConfigSave] New Config Params received");
  
    s = "<p>[handleConfigSave] Settings saved to eeprom</p>\r\n\r\n";
    //s = "<p>Settings saved to eeprom and reset to boot into new settings</p>\r\n\r\n";
         
    Serial.println("clearing EEPROM.");
    clearConfig();
  
    Serial.print("Settings written ");
  
//    saveConfig()? Serial.println("sucessfully.") : Serial.println("not succesfully!");;
    Serial.println("Restarting!"); 
  
    delay(1000);
    ESP.reset();
  }
  else 
  {Serial.println("\n\nNo changes made to Config Params.");
    s = "<p>[handleConfigSave] No changes made to Config Params.</p>\r\n\r\n";
  }
  //alert("no changes");
  //server.send(200, "text/html", s); 
  //delay(2000);
  
  server.sendHeader("Location","/");
  server.send(303);
  
  delay(2000);
}

String webPage = "<!DOCTYPE HTML>\
<html lang=\"en\">\
  <head>\
    <title>\
      NodeMCU Configuration\
    </title>\
    <link href=\"https://bootswatch.com/4/pulse/bootstrap.css\" rel=\"stylesheet\"></link>\
                <link href=\"https://bootswatch.com/4/pulse/bootstrap.min.css\" rel=\"stylesheet\"></link>\
  </head>\  
  <body>\
    <br/>\
    <br/>\
    <div class=\"text-center\">\
      <h1><a href=\"#\">NodeMCU Configuration</a></h1>\
    </div>\
    </script>\
    <div class=\"container\">\      
    <form action=\"/timer\" method=\"post\" >\
         <fieldset>\
      <div class=\"form-group\">\
           <table class=\"table table-striped table-hover table-bordered\">\
                         <thead class=\"thead-dark\">\
                          <tr>\
                           <th>S.No.</th> \
                           <th>GPIO Number</th>\
                           <th>Value</th>\    
                         </tr>\
                        </thead>\
                        <tbody>\
                         <tr>\
                          <td>1</td>\
                          <td>Switches</td>\
                          <td><input id=\"gpio14\" name=\"gpio14\" placeholder=\"Number of Switches\" type=\"text\" min=\"1\"/></td>\
        </tr>\
                         <tr>\
                          <td>2</td>\
                          <td>Password</td>\
                          <td><input id=\"passkey\" name=\"passkey\" placeholder=\"Enter you password\" type=\"password\" /></td>\
                         </tr>\
                         <tr >\
                       </tbody>\
                      </table>\
                      <div class=\"text-center\">\
                       <button type=\"submit\" id=\"gpio-btn\" class=\"btn btn-success\" type=\"button\">Submit</button>\
          </div>\
       </div>\
                </fieldset>\
    </form>\
    </div>\
    <br/>\
    <br/>\
    <div style=\"border-top: solid 1px #eee; padding-top:1em;\">\
      <div class=\"container\">\
        <div class=\"row\">\
          <div class=\"col-sm-12 text-center\">\
            <p class=\"text-muted\">CDAC |&copy; All Rights Researved\
            </p>\
            <p class=\"text-center\">\
            </p>\
          </div>\
        </div>\
      </div>\
    </div>\
  </body>\
</html>";
      
void handleConfigParamsWebPage() 
{
  digitalWrite(led, 1);
  server.send(200, "text/html",webPage);
  digitalWrite(led, 0);
}

void handleConfigParamsWithPassKey()
{
  server.sendHeader("Location","/");
  if(!server.hasArg("passkey") || server.arg("passkey")==NULL)
      server.send(400,"text/html","<h1>HTTP 400:</h1><h3> Bad Request</h3>");
  else
  {
    if(server.arg("passkey")=="100") //"123456789")
    {   
        int temp = server.arg("gpio14").toInt();
        if(temp !=0)
        {
          TIMER = temp * 1000;
          Serial.println(TIMER);
        
          server.send(303);
        
          Serial.println("New number of Switches received");
        }
        else
          server.send(400,"text/html","<h1>HTTP 400:</h1><h3> Bad Request</h3>");
    }
    else
      server.send(401,"text/html","<h1>HTTP 401:</h1><h3> Not Authorised!</h3>");
  }
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

//void setup(void)
//{
//  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(115200);
//  WiFi.begin(ssid,password);
//
//  
//// Wait for connection
//  while ( WiFi.status() != WL_CONNECTED ) {
//    delay ( 500 );
//    Serial.print ( "." );
//  }
//  Serial.println();
//  Serial.println(WiFi.localIP());
//  Serial.println(WiFi.macAddress());
//
//  Serial.printf("\n\nWeb server started. \n\nOpen %s in a web browser \nto enter configuration parameters \n\n\n", WiFi.localIP().toString().c_str());
//
//  if (MDNS.begin("esp8266")) {
//    Serial.println("MDNS responder started");
//  }
//
//  server.on("/", handleRoot);
//
//  server.on("/timer",HTTP_POST, handleConfigParams);
//
// server.onNotFound(handleNotFound);
//
//  server.begin();
//  Serial.println("HTTP server started");
//}

//void loop(void){
//  server.handleClient();
//
//  digitalWrite(LED_BUILTIN,LOW);
//  Serial.println("LED ON");
//  delay(TIMER);
//  digitalWrite(LED_BUILTIN,HIGH);
//  delay(TIMER);
//  
//}
