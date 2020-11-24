
// ESP32 Arduino: Encrypt & Decrypt AES-128 in ECB mode
// https://techtutorialsx.com/2018/05/10/esp32-arduino-decrypt-aes-128-in-ecb-mode/
// https://techtutorialsx.com/2018/04/09/python-pycrypto-using-aes-128-in-ecb-mode/
// https://esp32.com/viewtopic.php?t=4004
// https://techtutorialsx.com/2018/04/18/esp32-arduino-encryption-using-aes-128-in-ecb-mode/
// https://www.dfrobot.com/blog-911.html

// AES CBC : 
// ARM MbedTLS Knowledge Base
// https://tls.mbed.org/kb
// mbedTLS - How to encrypt data with AES-CBC mode
// https://tls.mbed.org/kb/how-to/encrypt-with-aes-cbc
// https://tls.mbed.org/kb/how-to/generate-an-aes-key
// https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Electronic_Codebook_(ECB)


// The difference in five modes in the AES encryption algorithm
// https://www.highgo.ca/2019/08/08/the-difference-in-five-modes-in-the-aes-encryption-algorithm/
// https://stackoverflow.com/questions/1220751/how-to-choose-an-aes-encryption-mode-cbc-ecb-ctr-ocb-cfb


// HTTPS on the ESP32 - Server and Client Side
// https://www.thingforward.io/techblog/2018-07-18-https-on-the-esp32-server-and-client-side.html

/*
we are using AES-128, so we need to pass the value 128 to the key length. Naturally, 
this value should be coherent with the actual size of our key string, which has indeed 128 bits (16 bytes).
*/

// aes.h File Reference - API Documentation (Doxygen generated)
// https://tls.mbed.org/api/aes_8h.html
// https://tls.mbed.org/api/aes_8h.html#acec17c6592b98876106d035c372b1efa

#include <mbedtls/aes.h> 
// https://esp32.com/viewtopic.php?t=4004
// https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/include/mbedtls/mbedtls/aes.h
// https://www.devglan.com/online-tools/aes-encryption-decryption
// http://aes.online-domain-tools.com/

void encrypt(char * plainText, char * key, unsigned char * outputBuffer)
{
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb( &aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)plainText, outputBuffer);
  mbedtls_aes_free( &aes );
}
 
void decrypt(unsigned char * chipherText, char * key, unsigned char * outputBuffer)
{
 
  mbedtls_aes_context aes;
 
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char*)chipherText, outputBuffer);
  mbedtls_aes_free( &aes );
}
 
void setup() 
{
 
  Serial.begin(115200);
 
  char * key = "abcdefghijklmnop";
 
  char *plainText = "MY SECRET MESSAGE!!";
  unsigned char cipherTextOutput[16];
  unsigned char decipheredTextOutput[16];
 
  encrypt(plainText, key, cipherTextOutput);
  decrypt(cipherTextOutput, key, decipheredTextOutput);
 
  Serial.println("\nOriginal plain text:");
  Serial.println(plainText);
 
  Serial.println("\nAES ECB - Ciphered text:");
  for (int i = 0; i < 16; i++) 
  {
 
    char str[3];
 
    sprintf(str, "%02x", (int)cipherTextOutput[i]);
    Serial.print(str);
  }
 
  Serial.println("\n\nAES ECB - Deciphered text:");
  for (int i = 0; i < 16; i++) 
  {
    Serial.print((char)decipheredTextOutput[i]);
  }

  ESP32_SHA256_MD_Hash_Generation();

  ESP32_HMAC_SHA256_Msg_Auth();
  
}
 
void loop() 
{}
