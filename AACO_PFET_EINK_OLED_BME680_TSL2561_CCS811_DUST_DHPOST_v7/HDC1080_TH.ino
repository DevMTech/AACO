// https://www.innovatorsguru.com/hdc1080-arduino/


//#include <Wire.h>


void HDC1080_setup()
{
  //Initialize I2C Communication
  //  Wire.begin();
  //  Serial.begin(119200);
  //Configure HDC1080
  Wire.beginTransmission(0x40);
  Wire.write(0x02);
  Wire.write(0x90);
  Wire.write(0x00);
  Wire.endTransmission();
  //Delay for Startup of HDC1080
  delay(20);
}

void HDC1080_loop()
{
  //  double HDC1080_temperature;
  //  double HDC1080_humidity;
  readSensor(&HDC1080_temperature, &HDC1080_humidity);

  //HDC1080_humidity(%) = reading/(2^16)*100%
  HDC1080_humidity = HDC1080_humidity * 100L / 65536;

  //Print the current HDC1080_temperature to the right of the label
  Serial.print("\nHDC1080_temperature: ");
  Serial.print(HDC1080_temperature);
  Serial.println(" *C");

  //Print the current HDC1080_humidity to the right of the label
  Serial.print("HDC1080_humidity: ");
  Serial.print(HDC1080_humidity);
  Serial.println(" %");

  //Wait 1 second for the next reading
  //  delay(1000);
}

void readSensor(double* HDC1080_temperature, double* HDC1080_humidity)
{
  //holds 2 bytes of data from I2C Line
  uint8_t Byte[4];
  //holds the total contents of the temp register
  uint16_t temp;
  //holds the total contents of the HDC1080_humidity register
  uint16_t humid;
  //Point to device 0x40 (Address for HDC1080)
  Wire.beginTransmission(0x40);
  //Point to register 0x00 (HDC1080_temperature Register)
  Wire.write(0x00);
  //Relinquish master control of I2C line
  //pointing to the temp register triggers a conversion
  Wire.endTransmission();
  //delay to allow for sufficient conversion time
  delay(20);
  //Request four bytes from registers
  Wire.requestFrom(0x40, 4);
  delay(1);
  //If the 4 bytes were returned sucessfully
  if (4 <= Wire.available())
  {
    //take reading
    //Byte[0] holds upper byte of temp reading
    Byte[0] = Wire.read();
    //Byte[1] holds lower byte of temp reading
    Byte[1] = Wire.read();
    //Byte[3] holds upper byte of HDC1080_humidity reading
    Byte[3] = Wire.read();
    //Byte[4] holds lower byte of HDC1080_humidity reading
    Byte[4] = Wire.read();

    //Combine the two bytes to make one 16 bit int
    temp = (((unsigned int)Byte[0] << 8 | Byte[1]));
    //Temp(C) = reading/(2^16)*165(C) – 40(C)
    *HDC1080_temperature = (double)(((temp) * 165 / (65536))  - 40.00);

    //Combine the two bytes to make one 16 bit int
    humid = (((unsigned int)Byte[3] << 8 | Byte[4]));

    Serial.print("HDC1080_humid: ");
    Serial.println(humid);


    *HDC1080_humidity = (double)((humid)); //  * 100L / (65536));
    //    return (double)((HDC1080_humidity) * 100 / (65536));
  }
  else  Serial.println("HDC1080 READ ERROR !!!!");
  //  return NULL;
}
