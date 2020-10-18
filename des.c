#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "des.h"

#define MAX_ROUNDS 16

DES_Config config;
static uint8_t des_key[8];
static uint8_t round_key[MAX_ROUNDS][8];

#define DEBUG //printf


static const uint8_t PC1[] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

static const uint8_t PC2[] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

static const uint8_t key_shifts[] = {
        1,
        1,
        2,
        2,
        2,
        2,
        2,
        2,
        1,
        2,
        2,
        2,
        2,
        2,
        2,
        1
};

static const uint8_t IP[] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

static const uint8_t FP[] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

static const uint8_t E[] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
};

static const uint8_t S1[] = {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};
static const uint8_t S2[] = {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9};
static const uint8_t S3[] = {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12};
static const uint8_t S4[] = {
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};
static const uint8_t S5[] = {
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3};
static const uint8_t S6[] = {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13};
static const uint8_t S7[] = {
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12};
static const uint8_t S8[] = {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

static const uint8_t P[] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25
};


const DES_Config DES_default = {
        .iperm = 1, .fperm = 1, .p = 1, .swap_before_fperm = 1, .check_parity = 0, .key_56bit = 0, .rounds = 16, 
        .IP = IP, .FP = FP, .PC1 = PC1, .PC2 = PC2, .keyShifts = key_shifts, .E = E, .P = P,  
        .S1 = S1, .S2 = S2, .S3 = S3, .S4 = S4, .S5 = S5, .S6 = S6, .S7 = S7, .S8 = S8
};


//---------------------------------------------------------------------------------------------------------------------
int get_bit(const uint8_t *bitfield, int bit) {
    return (bitfield[bit / 8] & (1 << (7 - (bit % 8)))) ? 1 : 0;
}

//---------------------------------------------------------------------------------------------------------------------
void set_bit(uint8_t *bitfield, int bit) {
    bitfield[bit / 8] |= 1 << (7 - (bit % 8));
}

//---------------------------------------------------------------------------------------------------------------------
void clear_bit(uint8_t *bitfield, int bit) {
    bitfield[bit / 8] &= ~(1 << (7 - (bit % 8)));
}

//---------------------------------------------------------------------------------------------------------------------
void change_bit(uint8_t *bitfield, int bit, int val) {
    if (val) set_bit(bitfield, bit);
    else clear_bit(bitfield, bit);
}

//---------------------------------------------------------------------------------------------------------------------
uint8_t count_bit(uint8_t x) {
    const unsigned char ones[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
    return ones[x & 0xf] + ones[x >> 4];
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t left_shift(uint32_t v) {
    v <<= 1;
    v |= (v & (1 << 28)) ? 1 : 0;
    v &= ~(1 << 28);
    return v;
}

//---------------------------------------------------------------------------------------------------------------------
void print_key(const uint8_t *key) {
    int i;
    DEBUG("Key: ");
    for (i = 0; i < 8; i++) {
        DEBUG("%02X ", key[i]);
    }
    DEBUG("\n");
}

//---------------------------------------------------------------------------------------------------------------------
int des_init(const unsigned char key[], const DES_Config cfg) {
    int i, key_bit = 0, b, round, shifts;
    config = cfg;

    if (cfg.rounds > MAX_ROUNDS || cfg.rounds < 0) {
        printf("Number of rounds must be between 0 and %d\n", MAX_ROUNDS);
        return 0;
    }

    // copy or expand key, depending on number of bits
    for (i = 0; i < 64; i++) {
        if (cfg.key_56bit) {
            if (i % 8 != 7) {
                change_bit(des_key, i, get_bit((const uint8_t *) key, key_bit));
                key_bit++;
            } else {
                if (!cfg.check_parity) {
                    clear_bit(des_key, i);
                } else {
                    change_bit(des_key, i, (count_bit(des_key[i / 8]) + 1) % 2);
                }
            }
        } else {
            change_bit(des_key, i, get_bit((const uint8_t *) key, key_bit));
            key_bit++;
        }
    }

    if (cfg.check_parity) {
        for (i = 0; i < 8; i++) {
            if (count_bit(des_key[i]) % 2 != 1) {
                printf("Wrong parity for key in byte %i!\n", i + 1);
                return 0;
            }
        }
    }

    print_key(key);
    print_key(des_key);

    // round key generation

    // permuted choice 1
    uint8_t kp[7];
    memset(kp, 0, sizeof(kp));
    for (b = 0; b < 56; b++) {
        change_bit(kp, b, get_bit(des_key, config.PC1[b] - 1));
    }
    DEBUG("K+ ");
    print_key(kp);

    // generate C and D for each round
    uint32_t c[MAX_ROUNDS + 1], d[MAX_ROUNDS + 1];

    // split permutated key into C and D
    c[0] = d[0] = 0;
    for (i = 0; i < 28; i++) {
        c[0] = (c[0] << 1) | get_bit(kp, i);
    }
    for (i = 0; i < 28; i++) {
        d[0] = (d[0] << 1) | get_bit(kp, 28 + i);
    }
    DEBUG("C[0]: %07X, D[0]: %07X\n", c[0], d[0]);

    uint8_t rk[8];
    for (round = 0; round < cfg.rounds; round++) {
        // do key shifting for C and D
        c[round + 1] = c[round];
        d[round + 1] = d[round];
        for (shifts = 0; shifts < config.keyShifts[round]; shifts++) {
            c[round + 1] = left_shift(c[round + 1]);
            d[round + 1] = left_shift(d[round + 1]);
        }
        DEBUG("C[%d]: %07X, D[%d]: %07X\n", round + 1, c[round + 1], round + 1, d[round + 1]);

        // merge C and D
        memset(rk, 0, sizeof(rk));
        c[0] = d[0] = 0;
        for (i = 0; i < 28; i++) {
            change_bit(rk, i, c[round + 1] & (1 << (27 - i)));
        }
        for (i = 0; i < 28; i++) {
            change_bit(rk, i + 28, d[round + 1] & (1 << (27 - i)));
        }
        DEBUG("Temp ");
        print_key(rk);

        // apply PC2
        memset(round_key[round], 0, sizeof(round_key[round]));
        for (b = 0; b < 48; b++) {
            change_bit(round_key[round], b, get_bit(rk, config.PC2[b] - 1));
        }
        DEBUG("Round %d ", round);
        print_key(round_key[round]);
    }
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------
void des_get_roundkey(unsigned char roundkey[6], int round) {
    memcpy(roundkey, round_key[round], 6);
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t des_f(uint32_t r, uint32_t round) {
    // expand r
    uint8_t re[6];
    int b;

    memset(re, 0, sizeof(re));
    for (b = 0; b < 48; b++) {
        change_bit(re, b, r & (1 << (31 - (config.E[b] - 1))));
    }
    DEBUG("E ");
    print_key(re);

    // xor with round key
    for (b = 0; b < 6; b++) {
        re[b] ^= round_key[round][b];
    }

    DEBUG("XOR ");
    print_key(re);

    // apply sboxes
    const uint8_t *S[] = {
            config.S1, config.S2, config.S3, config.S4, config.S5, config.S6, config.S7, config.S8
    };
    uint32_t res = 0, res_p = 0;
    for (b = 0; b < 8; b++) {
        uint8_t row = (get_bit(re, 6 * b) << 1) | get_bit(re, 6 * b + 5);
        uint8_t col = (get_bit(re, 6 * b + 1) << 3) | (get_bit(re, 6 * b + 2) << 2) | (get_bit(re, 6 * b + 3) << 1) |
                      get_bit(re, 6 * b + 4);
        uint8_t sbox_val = S[b][row * 16 + col];
        res = (res << 4) | sbox_val;
    }
    DEBUG("SBOX: %08X\n", res);

    // apply permutation
    if (config.p) {
        for (b = 0; b < 32; b++) {
            if (res & (1 << (31 - (config.P[b] - 1)))) res_p |= (1 << (31 - b));
        }
    } else res_p = res;
    DEBUG("P: %08X\n", res_p);
    return res_p;
}

//---------------------------------------------------------------------------------------------------------------------
int des_crypt(const unsigned char message[8], unsigned char output[8], int dir) {
    uint8_t msg[8];
    int b, round, i;

    // initial permutation
    memset(msg, 0, sizeof(msg));
    if (config.iperm) {
        for (b = 0; b < 64; b++) {
            change_bit(msg, b, get_bit((const uint8_t *) message, config.IP[b] - 1));
        }
    } else {
        for (b = 0; b < 8; b++) msg[b] = message[b];
    }
    DEBUG("Message after IP ");
    print_key(msg);

    // split message into L and R
    uint32_t l = 0, r = 0;
    for (i = 0; i < 32; i++) {
        l = (l << 1) | get_bit(msg, i);
    }
    for (i = 0; i < 32; i++) {
        r = (r << 1) | get_bit(msg, 32 + i);
    }
    DEBUG("L0: %08X, R0: %08X\n", l, r);

    // rounds
    for (round = 0; round < config.rounds; round++) {
        uint32_t old_r = r;
        r = l ^ des_f(r, (dir == 0) ? round : (config.rounds - 1 - round));
        l = old_r;
        DEBUG("L%d: %08X, R%d: %08X\n", round + 1, l, round + 1, r);
    }

    // swap L and R
    uint8_t LR[8];
    uint32_t lp = l, rp = r;
    if (config.swap_before_fperm) {
        lp = r;
        rp = l;
    }
    for (i = 0; i < 32; i++) {
        change_bit(LR, i, lp & (1 << (31 - i)));
    }
    for (i = 0; i < 32; i++) {
        change_bit(LR, i + 32, rp & (1 << (31 - i)));
    }
    DEBUG("LR ");
    print_key(LR);

    // final permutation
    memset(output, 0, 8);
    if (config.fperm) {
        for (b = 0; b < 64; b++) {
            change_bit(output, b, get_bit(LR, config.FP[b] - 1));
        }
    } else {
        for (b = 0; b < 8; b++) output[b] = LR[b];
    }
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------
int des_encrypt(const unsigned char message[8], unsigned char output[8]) {
    return des_crypt(message, output, 0);
}

//---------------------------------------------------------------------------------------------------------------------
int des_decrypt(const unsigned char message[8], unsigned char output[8]) {
    return des_crypt(message, output, 1);
}
