#include <cipher.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

int main () {

        int og_len, pad_len;

        // test 1
        uint8_t *buffer1 = pad((uint8_t*)"");
        og_len = strlen("") * 8;
        printf("Original Buffer 1 (%d bits): %s\n", og_len, (char*)buffer1);
        pad_len = strlen((char*)buffer1) * 8;
        printf("Padded Buffer 1 (%d bits): %s\n", pad_len, (char*)buffer1);
        free(buffer1);

        assert(og_len == 0);
        assert(pad_len == BLOCK_SIZE);

        // test 2
        uint8_t *buffer2 = pad((uint8_t*)"1234567");
        og_len = strlen("1234567") * 8;
        printf("Original Buffer 2 (%d bits): %s\n", og_len, (char*)buffer2);
        pad_len = strlen((char*)buffer2) * 8;
        printf("Padded Buffer 2 (%d bits): %s\n", pad_len, (char*)buffer2);
        free(buffer2);

        assert(og_len == 56);
        assert(pad_len == BLOCK_SIZE);

        // test 3
        uint8_t *buffer3 = pad((uint8_t*)"12345678");
        og_len = strlen("12345678") * 8;
        printf("Original Buffer 3 (%d bits): %s\n", og_len, (char*)buffer3);
        pad_len = strlen((char*)buffer3) * 8;
        printf("Padded Buffer 3 (%d bits): %s\n", pad_len, (char*)buffer3);
        free(buffer3);

        assert(og_len == 64);
        assert(pad_len == 128);

        // test 4
        uint8_t *buffer4 = pad((uint8_t*)"123456789");
        og_len = strlen("123456789") * 8;
        printf("Original Buffer 4 (%d bits): %s\n", og_len, (char*)buffer4);
        pad_len = strlen((char*)buffer4) * 8;
        printf("Padded Buffer 4 (%d bits): %s\n", pad_len, (char*)buffer4);
        free(buffer4);

        assert(og_len == 72);
        assert(pad_len == 128);


        printf("All tests passed!\n");
        return 0;

}