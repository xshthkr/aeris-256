#include <cipher.h>
#include <key_schedule.h>
#include <utils.h>

#include <string.h>
#include <stdlib.h>

uint8_t* pad(uint8_t* buffer) {
        int len = strlen((char*)buffer);
        int pad_len = (BLOCK_SIZE / 8) - (len % (BLOCK_SIZE / 8));

        if (pad_len == 0) {
                pad_len = BLOCK_SIZE / 8;
        } else {
                pad_len = pad_len;
        }
        
        uint8_t* padded_buffer = (uint8_t*)malloc(len + pad_len + 1);
        memcpy(padded_buffer, buffer, len);
        memset(padded_buffer + len, pad_len, pad_len);
        padded_buffer[len + pad_len] = '\0';
        return padded_buffer;
}

void encrypt(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* key, int rounds, CipherMode mode) {
        if (mode == FEISTEL_NETWORK) {
                feistel_net(plaintext, ciphertext, key, rounds);
        }
        else {
                return;
        }
}

void decrypt(uint8_t* ciphertext, uint8_t* plaintext, uint8_t* key, int rounds, CipherMode mode) {
        if (mode == FEISTEL_NETWORK) {
                feistel_net(ciphertext, plaintext, key, rounds);
        }
        else {
                return;
        }
}

void feistel_net(uint8_t* plaintext, uint8_t* ciphertext, uint8_t* key, int rounds) {

        uint64_t* round_keys = (uint64_t*)malloc(rounds * sizeof(uint64_t));
        generate_round_keys(key, round_keys, rounds);

        // for block in message: encrypt using feistel network
        for (size_t i = 0; i < strlen((char*)plaintext); i += BLOCK_SIZE / 8) {
                uint64_t* block = (uint64_t*)(plaintext + i);
                
                uint32_t left = (uint32_t)(*block >> 32) & 0xFFFFFFFF;
                uint32_t right = (uint32_t)(*block & 0xFFFFFFFF);


                for (int j = 0; j < rounds; j++) {
                        uint32_t temp = right;
                        right = left ^ f(&right, &round_keys[j]);
                        left = temp;
                }

                uint64_t* encrypted_block = (uint64_t*)malloc(BLOCK_SIZE / 8);
                *encrypted_block = ((uint64_t) right << 32) | left;

                memcpy(ciphertext + i, encrypted_block, BLOCK_SIZE / 8);

                free(encrypted_block);
        }

        free(round_keys);
        
}


uint32_t f(uint32_t* half, uint64_t* round_key) {
        return (*half ^ *round_key) + ((*half << 7) | (*half >> 25));
}
