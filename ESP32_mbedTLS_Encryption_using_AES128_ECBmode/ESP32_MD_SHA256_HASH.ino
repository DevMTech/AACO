// ESP32 Arduino mbed TLS: using the SHA-256 algorithm
// https://techtutorialsx.com/2018/05/10/esp32-arduino-mbed-tls-using-the-sha-256-algorithm/


#include <mbedtls/md.h>
 
void ESP32_SHA256_MD_Hash_Generation()
{
 
  // Serial.begin(115200);
 
  char *payload = "Hello SHA 256!";
  byte shaResult[32];
 
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
 
  const size_t payloadLength = strlen(payload);         
 
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);
 
  Serial.print("\n\n SHA256_MD_Hash_Generation: \n Hash: ");
 
  for(int i= 0; i< sizeof(shaResult); i++){
      char str[3];
 
      sprintf(str, "%02x", (int)shaResult[i]);
      Serial.print(str);
  }
}
 
// void loop(){}
