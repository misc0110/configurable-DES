#include <stdio.h>
#include "des.h"

int main() {
    DES_Config cfg;
    cfg.key_56bit = 0;
    cfg.check_parity = 0;
    cfg.rounds = 16;

    const char key[] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e};
    //const char key[] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};

    des_init(&key, cfg);

    return 0;
}