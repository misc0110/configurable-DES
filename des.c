#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "des.h"

#define MAX_ROUNDS 16

DES_Config config;
uint8_t des_key[8];
uint8_t round_key[MAX_ROUNDS][8];

#define DEBUG printf


const uint8_t PC1[] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

const uint8_t PC2[] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

const uint8_t key_shifts[] = {
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


int get_bit(const uint8_t *bitfield, int bit) {
    return (bitfield[bit / 8] & (1 << (7 - (bit % 8)))) ? 1 : 0;
}

void set_bit(uint8_t *bitfield, int bit) {
    bitfield[bit / 8] |= 1 << (7 - (bit % 8));
}

void clear_bit(uint8_t *bitfield, int bit) {
    bitfield[bit / 8] &= ~(1 << (7 - (bit % 8)));
}

void change_bit(uint8_t *bitfield, int bit, int val) {
    if (val) set_bit(bitfield, bit);
    else clear_bit(bitfield, bit);
}

uint32_t left_shift(uint32_t v) {
    v <<= 1;
    v |= (v & (1 << 28)) ? 1 : 0;
    v &= ~(1 << 28);
    return v;
}

void print_key(const uint8_t *key) {
    int i;
    DEBUG("Key: ");
    for (i = 0; i < 8; i++) {
        DEBUG("%02X ", key[i]);
    }
    DEBUG("\n");
}

int des_init(const char *key, DES_Config cfg) {
    int i, key_bit = 0, b, round, shifts;
    config = cfg;

    if (cfg.rounds > MAX_ROUNDS || cfg.rounds < 0) {
        printf("Number of rounds must be between 0 and %d\n", MAX_ROUNDS);
        return 0;
    }

    // copy or expand key, depending on number of bits
    for (i = 0; i < 64; i++) {
        if (cfg.key_56bit) {
            if (i % 8 != 0 || !i) {
                change_bit(des_key, i, get_bit((const uint8_t *) key, key_bit));
                key_bit++;
            } else {
                if (!cfg.check_parity) {
                    clear_bit(des_key, i);
                } else {
                    // TODO: correct parity
                }
            }
        } else {
            change_bit(des_key, i, get_bit((const uint8_t *) key, key_bit));
            key_bit++;
        }
    }

    if (cfg.check_parity) {
        // TODO: check parity
    }

    print_key(key);
    print_key(des_key);

    // round key generation

    // permuted choice 1
    uint8_t kp[7];
    memset(kp, 0, sizeof(kp));
    for (b = 0; b < 56; b++) {
        change_bit(kp, b, get_bit(des_key, PC1[b] - 1));
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
        for (shifts = 0; shifts < key_shifts[round]; shifts++) {
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
            change_bit(round_key[round], b, get_bit(rk, PC2[b] - 1));
        }
        DEBUG("Round %d ", round);
        print_key(round_key[round]);
    }

}
