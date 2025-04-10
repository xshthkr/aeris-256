#include <key_schedule.h>
#include <cipher.h>
#include <utils.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main () {
        uint8_t* password = (uint8_t*)"password";
        uint8_t* salt = (uint8_t*)"salt";
        uint8_t* expected_master_key_hex = (uint8_t*)"7a37b85c8918eac19a9089c0fa5a2ab4dce3f90528dcdeec108b23ddf3607b99";
        uint8_t* expected_master_key_bin = (uint8_t*)malloc(KEY_SIZE / 8);
        hex_to_bin((const char*)expected_master_key_hex, expected_master_key_bin, KEY_SIZE / 8);

        uint8_t* master_key = generate_master_key(password, salt);

        assert(memcmp(master_key, expected_master_key_bin, KEY_SIZE / 8) == 0);
        printf("All tests passed.\n");

        free(master_key);
        free(expected_master_key_bin);

        return 0;
}