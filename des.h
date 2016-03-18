#ifndef _CONFIGURABLE_DES_H_
#define _CONFIGURABLE_DES_H_

typedef struct {
    int iperm : 1;
    int fperm : 1;
    int check_parity : 1;
    int key_56bit : 1;
    int rounds;
} DES_Config;

int des_init(const char* key, DES_Config cfg);


#endif
