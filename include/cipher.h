#ifndef CIPHER_H
#define CIPHER_H

#define BLOCK_SIZE 64
#define KEY_SIZE 256
#define ROUNDS 16

#include <stdint.h>

uint64_t* pad(uint8_t* input, int* length);
uint8_t* unpad(uint64_t* input, int length);

void encrypt(uint64_t* plaintext, uint64_t* ciphertext, uint8_t* master_key, int* length);
void decrypt(uint64_t* ciphertext, uint64_t* plaintext, uint8_t* master_key, int* length);

void feistel_net_encrypt(uint64_t* plaintext, uint64_t* ciphertext, uint8_t* master_key, int num_blocks);
void feistel_net_decrypt(uint64_t* ciphertext, uint64_t* plaintext, uint8_t* master_key, int num_blocks);
uint32_t f(uint32_t* half, uint64_t* round_key);

#endif // CIPHER_H