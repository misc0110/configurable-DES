#ifndef _CONFIGURABLE_DES_H_
#define _CONFIGURABLE_DES_H_

typedef struct {
    int iperm : 1;
    int fperm : 1;
    int swap_before_fperm : 1;
    int check_parity : 1;
    int key_56bit : 1;
    int rounds;
    const unsigned char *IP, *FP, *PC1, *PC2, *keyShifts, *E, *P, *S1, *S2, *S3, *S4, *S5, *S6, *S7, *S8;

} DES_Config;

const DES_Config DES_default;

int des_init(const unsigned char key[], const DES_Config cfg);

int des_encrypt(const unsigned char message[8], unsigned char output[8]);
int des_decrypt(const unsigned char message[8], unsigned char output[8]);

#endif
