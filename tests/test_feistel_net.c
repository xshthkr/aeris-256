#include <cipher.h>
#include <key_schedule.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main () {

        uint8_t* input = (uint8_t*) "Hello, World! This is a test string to chock the encryption and decryption process. It should be long enough to test the padding and unpadding functions. Let's see if it works correctly.";
        int input_length = strlen((char*)input);
        int length = strlen((char*)input) * 8;                  // in bits

        uint64_t* buffer_plaintext = pad(input, &length);       // changes length to block aligned length
        int num_block = length / BLOCK_SIZE;

        uint8_t* password = (uint8_t*) "password";
        uint8_t* salt = (uint8_t*) "salt";

        uint8_t* master_key = generate_master_key(password, salt);

        uint64_t* buffer_ciphertext = (uint64_t*) malloc(sizeof(uint64_t) * num_block);
        encrypt(buffer_plaintext, buffer_ciphertext, master_key, &length);

        uint64_t* buffer_decrypted = (uint64_t*) malloc(sizeof(uint64_t) * num_block);
        decrypt(buffer_ciphertext, buffer_decrypted, master_key, &length);

        uint8_t* buffer_plaintext_unpadded = unpad(buffer_decrypted, input_length + 1);
        buffer_plaintext_unpadded[input_length] = '\0';

        assert(memcmp(buffer_plaintext_unpadded, input, input_length) == 0);
        printf("All tests passed.\n");

        printf("Input: %s\n", (char*)input);
        // print hex of ciphertext
        printf("Ciphertext: ");
        for (int i = 0; i < num_block * 8; i++) {
                printf("%02x ", ((uint8_t*)buffer_ciphertext)[i]);
        }
        printf("\n");
        printf("Decrypted: %s\n", (char*)buffer_plaintext_unpadded);

        free(buffer_plaintext);
        free(buffer_ciphertext);
        free(buffer_decrypted);
        free(master_key);
        free(buffer_plaintext_unpadded);
        
        return 0;
}
