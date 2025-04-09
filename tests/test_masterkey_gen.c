#include <key_schedule.h>
#include <cipher.h>
#include <utils.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main () {

        // test 1
        uint8_t* masterkey1 = generate_master_key((uint8_t*)"TestKey123", (uint8_t*)"saltysalt");
        int len = strlen((char*)masterkey1) * 8;
        uint8_t* expected_masterkey1 = (uint8_t*)"3dc0601dfcb0057e5fa4a57568c1ff44a65ebf017997cac0db6c4794e1e66ae0";
        uint8_t bin_expected_masterkey1[KEY_SIZE / 8];
        hex_to_bytes((const char*)expected_masterkey1, bin_expected_masterkey1, KEY_SIZE / 8);
        int cmp = memcmp(masterkey1, bin_expected_masterkey1, KEY_SIZE / 8);
        free(masterkey1);

        assert(len == KEY_SIZE);
        assert(cmp == 0);

        printf("All tests passed!\n");
        return 0;

}