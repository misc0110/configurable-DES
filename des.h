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

/// Default DES configuration
///
/// A DES configuration structure containing the default parameters for DES.
/// You should take this as a basis for custom configurations.
///
const DES_Config DES_default;

/// DES initialization
///
/// Initializes the DES algorithm with the given configuration and key.
/// The round keys are generated in this function. This has to be called before any other des_ function is called.
///
/// @param  key Key for DES encryption/decryption. Is either 7 or 8 bytes long,
//          depending on the value of key_56bit in the config.
/// @param  cfg Configuration for DES
/// @return 1 on success, 0 otherwise
int des_init(const unsigned char key[], const DES_Config cfg);

/// DES Encryption
///
/// Encrypt a block using DES. The des_init() function has to be called before this function can be called.
///
/// @param  message The message to encrypt. It must have a size of 8 bytes (64bit)
/// @param  output A buffer to which the encrypted message is saved to. It must have a size of at least 8 bytes
/// @return 1 on success, 0 otherwise
int des_encrypt(const unsigned char message[8], unsigned char output[8]);

/// DES Decryption
///
/// Decrypt a block using DES. The des_init() function has to be called before this function can be called.
///
/// @param  message The message to decrypt. It must have a size of 8 bytes (64bit)
/// @param  output A buffer to which the decrypted message is saved to. It must have a size of at least 8 bytes
/// @return 1 on success, 0 otherwise
int des_decrypt(const unsigned char message[8], unsigned char output[8]);

#endif
