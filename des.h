#ifndef _CONFIGURABLE_DES_H_
#define _CONFIGURABLE_DES_H_

typedef struct {
    int iperm : 1;
    int fperm : 1;
    int swap_before_fperm : 1;
    int check_parity : 1;
    int key_56bit : 1;
    int rounds;
} DES_Config;

const DES_Config default_config;

int des_init(const unsigned char *key, const DES_Config cfg);

int des_encrypt(const unsigned char message[8], unsigned char output[8]);

#endif
