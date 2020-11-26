/*
   SDS-011 Sensor - IoT air quality monitoring
  -------------------------------------------
  The Raspberry Pi server is running InfluxDB, Node-RED, and Grafana inside Docker.
  It's also running Mosquitto as the MQTT broker, and Portainer to give me
  a nice GUI to the Docker host. And of course, I'm also monitoring the server
  itself with Telegraf running directly on the host.
  https://dev.to/minkovsky/working-on-my-iot-air-quality-monitoring-setup-40a5

  AIR QUALITY - ATMOSCAN
  https://www.instructables.com/id/AtmoScan/
  https://github.com/MarcFinns/AtmoScan
  https://www.youtube.com/watch?v=iyFuKU8ZcuA

  MCU     SDS011

  5V      5V
  GND     GND
  RX      TX
  TX      RX
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Read MZ-Z19 CO2 concentration using UART interface and publish value on Domoticz with HTTP request
  Publish CO2 concentration on Domoticz via HTTP / JSON interface, Arduino code compatible ESP8266 and ESP32
  https://diyprojects.io/publish-co2-concentration-mh-z19-sensor-domoticz-arduino-code-compatible-esp32-esp8266/
  https://github.com/jehy/arduino-esp8266-mh-z19-serial
  https://github.com/jehy/co2-online-display

  CO2 LEVELS -
  https://www.kane.co.uk/knowledge-centre/what-are-safe-levels-of-co-and-co2-in-rooms

  MCU                 MH-Z19B

  VCC(3V3)            VIN
  GND                 GND
  SELECTED RX PIN     PIN MARKED TX
  SELECTED TX PIN     PIN MARKED RX

  What are safe levels of CO and CO2 in rooms?
  CO2

  250-350ppm  Normal background concentration in outdoor ambient air
  350-1,000ppm  Concentrations typical of occupied indoor spaces with good air exchange
  1,000-2,000ppm  Complaints of drowsiness and poor air.
  2,000-5,000 ppm   Headaches, sleepiness and stagnant, stale, stuffy air. Poor concentration, loss of attention, increased heart rate and slight nausea may also be present.
  5,000   Workplace exposure limit (as 8-hour TWA) in most jurisdictions.
  >40,000 ppm   Exposure may lead to serious oxygen deprivation resulting in permanent brain damage, coma, even death.
*/

// https://quadmeup.com/arduino-esp32-and-3-hardware-serial-ports/
// THESE ARE DEFAULT DECLATIONS, NO NEED TO DEFINE HERE AGAIN
//#include <HardwareSerial.h> //
//HardwareSerial Serial2(2); // 0 (UART 1) & 2 (UART 3)

// UART2 - HARDWARE SERIAL PORT - FREE TO USE
#define RX2 16
#define TX2 17

#define DUST_TX RX2 // 16 ESP32 UART2 //  8 // MCU RX
#define DUST_RX TX2 // 17 ESP32 UART2 //  9 // MCU TX
#define DUST_Serial Serial2 // RX2 16, TX2 17 

void SDS011_DUST_MHZ19B_CO2_setup()
{
  //  pinMode(DUST_TX, INPUT);
  //  pinMode(DUST_RX, OUTPUT);

  // FORMAT: Serial2.begin(baud-rate, protocol, MCU_RX pin, MCU_TX pin);
  DUST_Serial.begin(9600, SERIAL_8N1, DUST_TX, DUST_RX);    // Serial2 RX2 16, TX2 17

}

void SDS011_DUST_loop()
{
  delay(50);
  DEBUG_Sprintln("\nDUST_SDS011");  
  print_DUST_SDS011();
}


void print_DUST_SDS011()
{
  //  while (DUST_Serial.available()>0)
  if (DUST_Serial.available())
  {
    byte pm_buf[10];
    DUST_Serial.readBytes(pm_buf, 10);

    unsigned int pm_25_count = 0;
    pm_25_count = pm_buf[3] << 8;
    pm_25_count |= pm_buf[2];
    pm_25 = (float) pm_25_count / 10.0f;

    unsigned int pm_10_count = 0;
    pm_10_count = pm_buf[5] << 8;
    pm_10_count |= pm_buf[4];
    pm_10 = (float) pm_10_count / 10.f;

    DEBUG_Sprint("PM2.5 = " + String(pm_25));
    DEBUG_Sprintln("\tPM10 = " + String(pm_10) + "\n");
  }

  //  DUST_Serial.end();
  delay(50);
}
