#include <utils.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

void hex_to_bytes(const char* hex, uint8_t* out, size_t out_len) {
    for (size_t i = 0; i < out_len; ++i) {
        sscanf(hex + 2*i, "%2hhx", &out[i]);
    }
}
