#ifndef _CONFIGURABLE_DES_H_
#define _CONFIGURABLE_DES_H_

typedef struct {
    // do initial permutation IP (default: true)
    int iperm : 1;
    // do final permutation FP (default: true)
    int fperm : 1;
    // swap L and R before final permutation FP (default: true)
    int swap_before_fperm : 1;
    // check parity of key (default: false)
    int check_parity : 1;
    // key has only 7 byte, not 8 byte (default: false)
    int key_56bit : 1;
    // number of rounds (default: 16)
    int rounds;
    // pointer to tables (default: DES standard)
    const unsigned char *IP, *FP, *PC1, *PC2, *keyShifts, *E, *P, *S1, *S2, *S3, *S4, *S5, *S6, *S7, *S8;
} DES_Config;

// default DES configuration
const DES_Config DES_default;

// initialize DES with custom config and generate round keys
int des_init(const unsigned char key[], const DES_Config cfg);

// encrypt block using DES
int des_encrypt(const unsigned char message[8], unsigned char output[8]);

// decrypt block using DES
int des_decrypt(const unsigned char message[8], unsigned char output[8]);

#endif
