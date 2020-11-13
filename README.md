# Configurable DES
[![Build Status](https://travis-ci.com/misc0110/configurable-DES.svg?branch=master)](https://travis-ci.com/misc0110/configurable-DES)

The goal of Configurable DES is to provide a customizable DES implementation. It is especially designed to test attacks against DES by allowing to change practically all aspects of the algorithm:
* Number of rounds (from 0 to 16)
* Enable/Disable permutations (permutation P, initial permutation (IP) and final permutation (FP))
* Allow 56bit and 64bit keys
* Enable/Disable parity check for key
* Provide custom SBOXes, IP table, FP table, expansion table, permutation table, Permuted Choice 1, Permuted Choice 2 and key shifts
 
The implementation is neither the fastest nor the the most memory efficient. It should not be used in productive code (but, hey, DES should not be used at all!). 

# Usage

The whole DES code (for encryption and decryption) is found in ```des.c```. All definitions are found in ```des.h```. The code can be compiled without any special flags and linked to your code. 

There is an example called ```test.c``` that demonstrates the usage. It can be started using ```make && make run```.

# Example

At first, the algorithm has to be configured using a ```DES_Config``` structure. It is advisable to copy the ```DES_default``` config and modify the copy. 

A simple example to disable the initial and final permutations (IP and FP) and reduce the number of rounds to 4 would be
```C
DES_Config cfg = DES_default;
cfg.iperm = 0; // disable initial permutation
cfg.fperm = 0; // disable final permutation
cfg.rounds = 4; // reduce to 4 rounds
```

Then, the algorithm has to be initialized using the key and the configuration:
```C
const unsigned char key[] = {0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e};
des_init(key, cfg);
```

Finally, the cipher can be used to encrypt and decrypt messages:
```C
const unsigned char message[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
unsigned char output[8], msg[8];
des_encrypt(message, output); // encrypt message to output
des_decrypt(output, msg); // decrypt output to msg -> must be same as the original message
```
