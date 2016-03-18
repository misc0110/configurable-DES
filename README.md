# Configurable DES

The goal of Configurable DES is to provide a customizable DES implementation. It is especially designed to test attacks against DES by allowing to change practically all aspects of the algorithm:
* Number of rounds (from 0 to 16)
* Enable/Disable initial permutation (IP) and final permutation (FP)
* Allow 56bit and 64bit keys
* Enable/Disable parity check for key
* Provide custom SBOXes, IP table, FP table, expansion table, permutation table, Permuted Choice 1, Permuted Choice 2 and key shifts
 
The implementation is neither the fastest nor the the most memory efficient. It should not be used in productive code (but, hey, DES should not be used at all!). 
