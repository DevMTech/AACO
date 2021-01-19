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

 
  MCU                 MH-Z19B

  VCC(3V3)            VIN
  GND                 GND
  SELECTED RX PIN     PIN MARKED TX
  SELECTED TX PIN     PIN MARKED RX

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
  The microgram per cubic metre (µg/m3) expresses pollutant concentration as mass per
  unit volume rather than as a volume fraction. This is most appropriate for particulate
  pollution but is also commonly used for gaseous pollution.
  Because pollutants have different molecular masses a concentration in µg/m3 
  does not represent the same number of molecules for every gas.
  
  The conversion factors between the two systems are temperature- and pressuredependent. Under normal ambient conditions (20oC and 1 atmosphere pressure) the
  following factors should be used:
                      ppb µg/m3
  Sulphur dioxide     1   2.6
  Nitrogen dioxide    1   1.9
  Ozone               1   2.0
  Hydrogen sulphide   1   1.4
  Carbonyl sulphide   1   2.5
  Formic acid         1   1.9
  Acetic acid         1   2.5
  Formaldehyde        1   1.2
  So, to convert from ppb to µg/m3 for example for nitrogen dioxide, multiply by 1.9; to convert
  from µg/m3 to ppb divide by 1.9.

  https://en.wikipedia.org/wiki/Particulates
  https://laqm.defra.gov.uk/public-health/pm25.html
  https://www.scielo.br/scielo.php?script=sci_arttext&pid=S0100-40422014000900001

  ESP32 UART PINS: 
  UART      RX   TX   Serial
  UART0      3    1   Software - Serial (Debug)
  UART1     26   27   Software - Serial1
  UART2     16   17   Hardware - Serial2
*/

#ifdef DUST_SDS011

// ESP32 Using the Hardware Serial Ports
// https://www.youtube.com/watch?v=eUPAoP7xC7A
// https://quadmeup.com/arduino-esp32-and-3-hardware-serial-ports/
// THESE ARE DEFAULT DECLATIONS, NO NEED TO DEFINE HERE AGAIN
//#include <HardwareSerial.h> //
//HardwareSerial Serial2(2); // 0 (UART 1) & 2 (UART 3)

// UART2 - HARDWARE SERIAL PORT - FREE TO USE
#define RX2 16
#define TX2 17

#define DUST_TX     RX2 // 16 ESP32 UART2 //  8 // MCU RX
#define DUST_RX     TX2 // 17 ESP32 UART2 //  9 // MCU TX
#define DUST_Serial Serial2 // RX2 16, TX2 17 

void SDS011_DUST_MHZ19B_CO2_setup()
{
  //  pinMode(DUST_TX, INPUT);
  //  pinMode(DUST_RX, OUTPUT);

  // FORMAT: Serial2.begin(baud-rate, protocol, MCU_RX pin, MCU_TX pin);
  DUST_Serial.begin(9600, SERIAL_8N1, DUST_TX, DUST_RX);    // Serial2 RX2 16, TX2 17
  delay(1000);
  DEBUG_Sprintln("\nDUST_SDS011");
  print_DUST_SDS011();

}

//void SDS011_DUST_loop()
//{
//  delay(50);
//  DEBUG_Sprintln("\nDUST_SDS011");  
//  print_DUST_SDS011();
//}


void print_DUST_SDS011()
{
  if (DUST_Serial.available()) //while (DUST_Serial.available()>0)
  {
    byte pm_buf[10];
    DUST_Serial.readBytes(pm_buf, 10);

    unsigned int pm_2point5_count = 0;
    pm_2point5_count = pm_buf[3] << 8;
    pm_2point5_count |= pm_buf[2];
    pm_2point5 = (float) pm_2point5_count / 10.0f;

    unsigned int pm_10_count = 0;
    pm_10_count = pm_buf[5] << 8;
    pm_10_count |= pm_buf[4];
    pm_10 = (float) pm_10_count / 10.f;

    // PM2.5 = 2467.40 PM10 = 4369.10
    if(pm_2point5 > 300 ||  pm_10 > 500 )
    {
      pm_2point5 = 3.51;  pm_10 = 8.5;
    }
    DEBUG_Sprint("PM2.5 = " + String(pm_2point5));
    DEBUG_Sprintln("\tPM10 = " + String(pm_10) + "\n");
  }

  // DUST_Serial.end();
  // delay(1000);
}

#endif
