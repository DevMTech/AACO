

/*    Bluetooth SSP (Serial Port Protocol)  
 *           BLUETOOTH CLASSIC 2.0
 * Turotial on:  https://circuitdigest.com/microcontroller-projects/using-classic-bluetooth-in-esp32-and-toogle-an-led
 * 
 * Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
 * 
 * The Bluetooth used in modules like HC-05 is a version of the 
 * classic Bluetooth called Bluetooth SSP (Serial Port Protocol), 
 * meaning the Bluetooth follows the standard serial protocol 
 * which makes it easier to send and receive data without much overhead.
 * 
 * Thanks to Neil Kolbans for his efforts in adding the support to Arduino IDE
 * 
 * 
 */

#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
int LED_BUILTIN = 2;

void setup() {
  Serial.begin(9600); //Start Serial monitor in 9600
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Device
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
}

void loop() {
  
  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    incoming = ESP_BT.read(); //Read what we recevive 
    Serial.print("Received:"); Serial.println(incoming);

    if (incoming == 49)
        {
        digitalWrite(LED_BUILTIN, HIGH);
        ESP_BT.println("LED turned ON");
        }
        
    if (incoming == 48)
        {
        digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("LED turned OFF");
        }     
  }
  delay(20);
}
