#include <utils.h>

#include <stdio.h>

void hex_to_bin(const char* hex, uint8_t* out, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        sscanf(hex + 2*i, "%2hhx", &out[i]);
    }
}