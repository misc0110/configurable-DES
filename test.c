#include <stdio.h>
#include "des.h"

int main() {
    // Create DES config by copying the default config
    DES_Config cfg = DES_default;
    // Disable IP and FP
    cfg.iperm = 0;
    cfg.fperm = 0;
    // Customize number of rounds
    cfg.rounds = 16;

    // Key for 64bit key
    const unsigned char key[] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e};
    // Same key in 56bit
    const unsigned char key7[] = {0x3a, 0x72, 0x61, 0xb1, 0x44, 0x3d, 0xaf};

    // Test message
    const unsigned char message[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    unsigned char output[8], msg[8], roundkey[6];

    // Initialize DES with custom config and 64bit key
    des_init(key, cfg);
    
    // Show round keys
    int i, b;
    for(i = 0; i < cfg.rounds; i++) {
      des_get_roundkey(roundkey, i);
      printf("Round Key %d: ", i + 1);
      for(b = 0; b < 6; b++) printf("%02X ", roundkey[b]);
      printf("\n");
    }
    
    // Encrypt message
    des_encrypt(message, output);

    printf("Encrypted: ");
    for (i = 0; i < 8; i++) printf("%02X ", output[i]);
    printf("\n");

    // Decrypt to check
    printf("Decrypted: ");
    des_decrypt(output, msg);
    for (i = 0; i < 8; i++) printf("%c", msg[i]);
    printf("\n");

    // Initialize DES with custom config and 64bit key
    cfg.key_56bit = 1;
    des_init(key7, cfg);
    // Encrypt same message
    des_encrypt(message, output);

    printf("Encrypted: ");
    for (i = 0; i < 8; i++) printf("%02X ", output[i]);
    printf("\n");

    // Decrypt to check
    des_decrypt(output, msg);
    printf("Decrypted: ");
    for (i = 0; i < 8; i++) printf("%c", msg[i]);
    printf("\n");

    return 0;
}