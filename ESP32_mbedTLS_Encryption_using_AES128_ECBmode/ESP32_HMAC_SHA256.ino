
// ESP32 Arduino: Applying the HMAC SHA-256 mechanism
// https://techtutorialsx.com/2018/01/25/esp32-arduino-applying-the-hmac-sha-256-mechanism/

/*
As a brief introduction, HMAC is a mechanism for message authentication that uses hash functions under the hood [1]. 
The strength of the HMAC is related with the strength of the used hash function [1], so it needs to be chosen accordingly to the application.

The HMAC can be used with any iterated cryptographic hash function [1]. In our case, we will use the SHA-256 function and 
thus the output HMAC code will have 32 bytes.

The sender of the message generates the HMAC code and sends it with the message. The recipient can then generate 
the code again from the message and check if it matches the one received.

As mentioned, the HMAC is used to ensure message authentication, which is achieved by having a shared secret 
(usually called a key) between the two communicating entities [1].

Since this key is only known by the two parties and is used in the HMAC mechanism, only those two parties 
should be able to generate the message authentication code, thus guaranteeing authentication.

Note that key length impacts the cryptographic strength of the HMAC and thus longer keys are more secure [1]. 
Also take in consideration that the keys used should be randomly generated [1]. 
Nonetheless, for simplicity, we will not use a random key on this example code.

Additionally to authentication, we can use this mechanism to check the integrity of the message [2], 
since if a possible attacker changes something in the message, the recipient of the message will generate 
a HMAC code that will not match the one received, thus discarding the message.

Additionally, since the attacker doesn’t know the secret key, he cannot tamper with the message and generate a new valid code.

// ESP32 Espruino: Applying the SHA1 hash algorithm
// https://techtutorialsx.com/2017/10/21/esp32-espruino-applying-the-sha1-hash-algorithm/
// https://www.freeformatter.com/hmac-generator.html

*/

#include <mbedtls/md.h>

void ESP32_HMAC_SHA256_Msg_Auth()
{ 
  // Serial.begin(115200);
  char *key = "secretKey";
  char *payload = "Hello HMAC SHA 256!";
  byte hmacResult[32];
 
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
 
  const size_t payloadLength = strlen(payload);
  const size_t keyLength = strlen(key);            
 
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char *) key, keyLength);
  mbedtls_md_hmac_update(&ctx, (const unsigned char *) payload, payloadLength);
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);
 
  Serial.print("\n\n HMAC_SHA256_Msg_Auth: \nHash: ");
 
  for(int i= 0; i< sizeof(hmacResult); i++)
  {
      char str[3];
 
      sprintf(str, "%02x", (int)hmacResult[i]);
      Serial.print(str);
  }
}

// void loop(){}
