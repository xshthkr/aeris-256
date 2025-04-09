#ifndef KEY_SCHEDULE_H
#define KEY_SCHEDULE_H

#include <stdint.h>

uint8_t* generate_master_key(uint8_t* password, uint8_t* salt);
void generate_round_keys(uint8_t* key, uint64_t* round_keys, int rounds);

#endif // KEY_SCHEDULE_H