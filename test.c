#include <stdio.h>
#include "des.h"

int main() {
    DES_Config cfg = DES_default;
    cfg.rounds = 16;
    cfg.iperm = 0;
    cfg.fperm = 0;

    const unsigned char key[] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e};
    const unsigned char key7[] = {0b00111010, 0b01110010, 0b01100001, 0b10110001, 0b01000100, 0b00111101, 0b10101111};
    //const char key[] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};

    const unsigned char message[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    unsigned char output[8], msg[8];

    des_init(key, cfg);
    des_encrypt(message, output);

    int i;
    for(i = 0; i < 8; i++) printf("%02X ", output[i]);
    printf("\n");

    des_decrypt(output, msg);
    for(i = 0; i < 8; i++) printf("%c", msg[i]);
    printf("\n");


    return 0;
}