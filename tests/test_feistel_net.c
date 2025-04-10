#include <cipher.h>
#include <key_schedule.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main () {

        uint8_t* input = (uint8_t*) "Hello, World!";
        int input_length = strlen((char*)input);
        int length = strlen((char*)input) * 8;                  // in bits

        uint64_t* buffer_plaintext = pad(input, &length);       // changes length to block aligned length

        uint8_t* password = (uint8_t*) "password";
        uint8_t* salt = (uint8_t*) "salt";

        uint8_t* master_key = generate_master_key(password, salt);

        uint64_t* buffer_ciphertext = (uint64_t*) malloc(length / 8);
        encrypt(buffer_plaintext, buffer_ciphertext, master_key, &length);

        uint64_t* buffer_decrypted = (uint64_t*) malloc(length / 8);
        decrypt(buffer_ciphertext, buffer_decrypted, master_key, &length);

        uint8_t* buffer_plaintext_unpadded = (uint8_t*) malloc(input_length + 1);
        memcpy(buffer_plaintext_unpadded, buffer_plaintext, input_length);
        buffer_plaintext_unpadded[input_length] = '\0';

        assert(memcmp(buffer_plaintext_unpadded, input, input_length) == 0);
        printf("All tests passed.\n");

        free(buffer_plaintext);
        free(buffer_ciphertext);
        free(buffer_decrypted);
        free(master_key);
        free(buffer_plaintext_unpadded);
        
        return 0;
}
