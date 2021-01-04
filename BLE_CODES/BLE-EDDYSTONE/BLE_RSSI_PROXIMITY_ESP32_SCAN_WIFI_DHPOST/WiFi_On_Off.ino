//RTC_DATA_ATTR char* ssid  = "hcdc-pc54";   RTC_DATA_ATTR char* password = "gLz7jl5s";
RTC_DATA_ATTR char* ssid  = "cdac";   RTC_DATA_ATTR char* password = "";

void WiFi_OFF()
{ WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();
  delay(1);
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi_Status = "CONNECTED";
    // Serial.println("\n================= WIFI_OFF : WiFi CONNECTED ==================\n");

    pinMode (LED2, OUTPUT);
    digitalWrite (LED2, HIGH); //HIGH = ON //LOW = OFF
  }
  else
  { WiFiOnDuration = millis() - WiFiOnDuration;

    WiFi_Status = "DISCONCTD";
    // Serial.println("\n================= WIFI_OFF : WiFi DISCONNECTED ==================\n");

    pinMode (LED2, OUTPUT);
    digitalWrite (LED2, LOW); //HIGH = ON //LOW = OFF
    pinMode (LED2, INPUT);
  }
}

void WiFi_ON()
{
  //  WiFi.forceSleepWake();
  // delay(1);
  WiFi_setup();
}

void WiFi_setup()
{
  deviceMAC = WiFi.macAddress();              //macToStr(mac);  //mac;
  Serial.print("\n\nMAC : "); // 84:0D:8E:C3:60:8C ESP32S
  Serial.println(deviceMAC); // (WiFi.macAddress());

  WiFi.mode(WIFI_STA);


  // Configures static IP address
  //  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) // if (!WiFi.config(local_IP, gateway, subnet))
  //  { Serial.println("STA Failed to configure");
  //  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);


  //Serial.println("Connecting Wifi...");
  WiFiConnAttemptDuration += millis(); // Keep track of when we started our attempt to get a WiFi connection

  int WiFiConnAttemptCount = 0;
  while (WiFi.status() != WL_CONNECTED && (WiFiConnAttemptCount < 100))
  { WiFiConnAttemptCount++;
    Serial.print(".");
    delay(50);
  }

  WiFiConnRetryAttempt++;

  if (WiFi.status() != WL_CONNECTED && (WiFiConnRetryAttempt >= 5))
  { Serial.println("Restarting ESP32 ... in 100ms");
    delay(100);
    WiFiConnRetryAttempt = 0;
    ESP.restart();
  }

  WiFiConnAttemptDuration = millis() - WiFiConnAttemptDuration;

  // Print local IP address and start web server
  Serial.flush();
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnection successful ...");
    //  Serial.println("\n\n");
    Serial.println("\nWiFi connected.");
    Serial.print("\nIP address: \t"); // Local
    Serial.println(WiFi.localIP());
    Serial.print("Gateway IP: \t");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask: \t");
    Serial.println(WiFi.subnetMask());
    Serial.print("DNS 1: \t\t");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2: \t\t");
    Serial.println(WiFi.dnsIP(1));
    Serial.println("");

//    bool success = Ping.ping("172.217.166.36", 3); // ("www.google.com", 3);
//    if (!success)
//    { Serial.println("\nFailed to Ping www.google.com");   // return;
//    }
//    else
//    { Serial.println("\nSuccessfully Pinged www.google.com");
//    }
  }
  else
  {
    Serial.print("\n\nFailed to connect to ");
    Serial.println(ssid);
  }

  //  WiFi.begin(ssid, password);
  //  wifiMulti.addAP(ssid, password);
  //  wifiMulti.addAP(ssid0, password0);
  //  wifiMulti.addAP(ssid1, password1);

  // WiFi.mode(WIFI_OFF); // WiFi.mode( WIFI_MODE_NULL );   // btStart(); // btStop();

  // unsigned long startAttemptTime = millis(); // Keep track of when we started our attempt to get a WiFi connection
  //  // Keep looping while we're not connected AND haven't reached the timeout
  //  while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime < WIFI_TIMEOUT))
  //  { delay(10);
  //  }

  //  int8_t c = 0;
  //  Serial.println("Connecting Wifi...");
  //  WiFiConnAttemptDuration += millis(); // Keep track of when we started our attempt to get a WiFi connection
  //  while (WiFi.status() != WL_CONNECTED && c++ < 100)
  //  //while (wifiMulti.run() != WL_CONNECTED && c++ < 10)
  //  { delay(1); Serial.print("."); //  Serial.println("Connecting..");
  //  }
  //  Serial.println("Connection Attempts .." + String(c));
  //  WiFiConnAttemptDuration = millis() - WiFiConnAttemptDuration;



  if (WiFi.status() == WL_CONNECTED)
    //if(wifiMulti.run() == WL_CONNECTED)
  { WiFiOnDuration += millis();
    WiFi_Status = "CONNECTED";
    // Serial.println("\n================= WIFI_ON : WiFi CONNECTED ==================\n");
    // Serial.println("\nWiFi Connection to " + String(ssid) + " established!");

    //    Serial.print("\nConnected to ");    Serial.println(WiFi.SSID());
    //    Serial.print("\nIP address:\t");    Serial.println(WiFi.localIP());

    deviceDescription = "Antiquity Ambient Condition Observer";
    //    deviceMAC = WiFi.macAddress();              //macToStr(mac);  //mac;
    deviceIP = WiFi.localIP().toString().c_str();
    //Serial.println("\n");

    //  if((int)batteryLevelPercent < 20)

    //pinMode(BUZZER, OUTPUT);   // BUZ ON
    //pinMode (LED2, OUTPUT);
    //digitalWrite (LED2, HIGH); //HIGH = ON //LOW = OFF
    //    int i = 5;
    //    while(i-- > 0)
    //    {
    //      digitalWrite(LED2, !digitalRead(LED2)); //toggle the LED
    //      digitalWrite(BUZZER, !(digitalRead(BUZZER))); delay(500);
    //    }
    //    pinMode(BUZZER, INPUT); // BUZ OFF
    //    digitalWrite (LED2, HIGH); //HIGH = ON //LOW = OFF
  }
  else
  { WiFi_Status = "DISCONNCTD";
    Serial.println("\n========================FAILED TO CONNECT TO WiFi==================\n");
    // Serial.println("\n================= WIFI_ON : WiFi DISCONNECTED ==================\n");

    //pinMode (LED2, OUTPUT);
    //digitalWrite (LED2, LOW); //HIGH = ON //LOW = OFF
    //pinMode (LED2, INPUT);

    // SAVE DATA IN EEPROM & GO TO DEEP SLEEP
  }
}
