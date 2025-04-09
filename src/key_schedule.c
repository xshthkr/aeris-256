#include <key_schedule.h>
#include <cipher.h>

#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

uint8_t* generate_master_key(uint8_t* password, uint8_t* salt) {
    uint8_t* master_key = (uint8_t*)malloc(KEY_SIZE / 8);
    uint8_t hash[SHA256_DIGEST_LENGTH];

    uint8_t* input = (uint8_t*)malloc(strlen((char*)password) + strlen((char*)salt) + 1);
    strcpy((char*)input, (char*)password);
    strcat((char*)input, (char*)salt);

    SHA256(input, strlen((char*)input), hash);

    memcpy(master_key, hash, KEY_SIZE / 8);

    free(input);
    return master_key;
}

void generate_round_keys(uint8_t* key, uint64_t* round_keys, int rounds) {
    for (int i = 0; i < rounds; i++) {
        // first 64 bits of the key xor with the round number
        uint64_t round_key = 0;
        for (size_t j = 0; j < KEY_SIZE / 8; j++) {
            round_key ^= ((uint64_t)key[j] << (j * 8));
        }
        round_key ^= (uint64_t)i;
        round_keys[i] = round_key;
    }
}