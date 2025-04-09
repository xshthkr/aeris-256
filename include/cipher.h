#ifndef CIPHER_H
#define CIPHER_H

#define BLOCK_SIZE 64
#define KEY_SIZE 256 

#include <stdint.h>

typedef enum {
        FEISTEL_NETWORK,
        SP_NETWORK
} CipherMode;

uint8_t* pad(uint8_t* buffer);

void encrypt(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* key, int rounds, CipherMode mode);
void decrypt(uint8_t* ciphertext, uint8_t* plaintext, uint8_t* key, int rounds, CipherMode mode);
void feistel_net(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* key, int rounds);
uint32_t f(uint32_t* half, uint64_t* round_key);

// void encrypt(uint64_t *plaintext, uint64_t *ciphertext, uint64_t *key, int rounds);
// void decrypt(uint64_t *ciphertext, uint64_t *plaintext, uint64_t *key, int rounds);

// uint64_t* feistel_net(uint64_t* input, uint64_t* key, int rounds);
// uint32_t f(uint32_t* half, uint64_t* round_key);

#endif // CIPHER_H