#include <cipher.h>
#include <key_schedule.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint64_t* pad(uint8_t* input, int* length) {

        // calculate the block aligned length
        if (*length % BLOCK_SIZE != 0) *length = (*length / BLOCK_SIZE + 1) * BLOCK_SIZE;
        else *length = (*length / BLOCK_SIZE) * BLOCK_SIZE;

        // malloc block aligned length to buffer
        uint64_t* buffer_plaintext = (uint64_t*) malloc(*length / 8);

        // memcpy the input to the buffer
        memcpy((uint8_t*)buffer_plaintext, input, strlen((char*)input));
        memset((uint8_t*)buffer_plaintext + strlen((char*)input), 'X', *length / 8 - strlen((char*)input));

        return buffer_plaintext;
}

uint8_t* unpad(uint64_t* input, int length) {
        uint8_t* unpadded_input = (uint8_t*) malloc(length);
        memcpy(unpadded_input, input, length);
        return unpadded_input;
}

void encrypt(uint64_t* plaintext, uint64_t* ciphertext, uint8_t* master_key, int* length) {

        feistel_net_encrypt(plaintext, ciphertext, master_key, *length / BLOCK_SIZE);

}

void decrypt(uint64_t* ciphertext, uint64_t* plaintext, uint8_t* master_key, int* length) {

        feistel_net_decrypt(ciphertext, plaintext, master_key, *length / BLOCK_SIZE);

}


void feistel_net_encrypt(uint64_t* plaintext, uint64_t* ciphertext, uint8_t* master_key, int num_blocks) {

        // generate round keys
        uint64_t* round_keys = (uint64_t*) malloc(ROUNDS * sizeof(uint64_t));
        generate_round_keys(master_key, round_keys);

        // for each block in message: encrypt using feistel network
        for (int i = 0; i < num_blocks; i++) {
                uint32_t left = (uint32_t)(plaintext[i] >> 32) & 0xFFFFFFFF;
                uint32_t right = (uint32_t)(plaintext[i] & 0xFFFFFFFF);

                for (int j = 0; j < ROUNDS; j++) {
                        uint32_t temp = right;
                        right = left ^ f(&right, &round_keys[j]);
                        left = temp;
                }

                ciphertext[i] = ((uint64_t)left << 32) | right;
        }

        free(round_keys);
}

void feistel_net_decrypt(uint64_t* ciphertext, uint64_t* plaintext, uint8_t* master_key, int num_blocks) {

        // generate round keys
        uint64_t* round_keys = (uint64_t*) malloc(ROUNDS * sizeof(uint64_t));
        generate_round_keys(master_key, round_keys);

        // for each block in message: decrypt using feistel network
        for (int i = 0; i < num_blocks; i++) {
                uint32_t left = (uint32_t)(ciphertext[i] >> 32) & 0xFFFFFFFF;
                uint32_t right = (uint32_t)(ciphertext[i] & 0xFFFFFFFF);

                for (int j = ROUNDS - 1; j >= 0; j--) {
                        uint32_t temp = left;
                        left = right ^ f(&left, &round_keys[j]);
                        right = temp;
                }

                plaintext[i] = ((uint64_t)left << 32) | right;
        }

        free(round_keys);
}

uint32_t f(uint32_t* half, uint64_t* round_key) {
    *half ^= (uint32_t)(*round_key & 0xFFFFFFFF);
    *half = (*half << 3) | (*half >> (32 - 3));
    *half = ~*half + 0x12345678;
    return *half;
}

