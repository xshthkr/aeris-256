#include <cipher.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main () {

        // test 1 blank input
        uint8_t* input = (uint8_t*) "";
        // printf("Input: %s\n", input);
        int length = strlen((char*)input) * 8;
        uint64_t* buffer_plaintext = pad(input, &length);
        // printf("Buffer plaintext: %s\n", (char*)buffer_plaintext);
        assert(memcmp((uint8_t*)buffer_plaintext, input, strlen((char*)input)) == 0);

        // test 2 input "1234567"
        input = (uint8_t*) "1234567";
        // printf("Input: %s\n", input);
        length = strlen((char*)input) * 8;
        buffer_plaintext = pad(input, &length);
        // printf("Buffer plaintext: %s\n", (char*)buffer_plaintext);
        assert(memcmp((uint8_t*)buffer_plaintext, (uint8_t*)"1234567X", 8) == 0);

        // test 3 input "12345678"
        input = (uint8_t*) "12345678";
        // printf("Input: %s\n", input);
        length = strlen((char*)input) * 8;
        buffer_plaintext = pad(input, &length);
        // printf("Buffer plaintext: %s\n", (char*)buffer_plaintext);
        assert(memcmp((uint8_t*)buffer_plaintext, input, 8) == 0);
        
        // test 4 input "123456789"
        input = (uint8_t*) "123456789";
        // printf("Input: %s\n", input);
        length = strlen((char*)input) * 8;
        buffer_plaintext = pad(input, &length);
        // printf("Buffer plaintext: %s\n", (char*)buffer_plaintext);
        assert(memcmp((uint8_t*)buffer_plaintext, (uint8_t*)"123456789XXXXXXX", 16) == 0);

        free(buffer_plaintext);

        printf("All tests passed.\n");
        return 0;

}