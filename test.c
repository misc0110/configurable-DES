#include <stdio.h>
#include "des.h"

int main() {
    DES_Config cfg;
    cfg.key_56bit = 0;
    cfg.check_parity = 0;
    cfg.rounds = 16;
    cfg.iperm = 1;

    const char key[] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e};
    //const char key[] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};

    const char message[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char output[8];

    des_init(&key, default_config);
    des_encrypt(&message, (char*)output);

    int i;
    for(i = 0; i < 8; i++) printf("%02X ", (unsigned char)output[i]);
    printf("\n");

    return 0;
}