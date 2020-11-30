#include <CRCx.h> // https://github.com/hideakitai/CRCx

const uint8_t data[] = "Hello World"; // { 'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D' };
const size_t size = sizeof(data);

void setup()
{
    Serial.begin(115200);
    delay(2000);

    CRCx_hideakitai();

    CRC32_CBaker();
}

void CRCx_hideakitai()
{
    uint8_t result8 = crcx::crc8(data, size);
    uint16_t result16 = crcx::crc16(data, size);
    uint32_t result32 = crcx::crc32(data, size);
//    String s = data;
//    Serial.print("\nData = ");
//    Serial.println(s); // .toString().c_str();
    Serial.print("crc8  = 0x");
    Serial.println(result8, HEX);
    Serial.print("crc16 = 0x");
    Serial.println(result16, HEX);
    Serial.print("crc32 = 0x");
    Serial.println(result32, HEX);
}

void loop()
{
  
}
