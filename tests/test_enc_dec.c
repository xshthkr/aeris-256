#include <cipher.h>
#include <key_schedule.h>
#include <utils.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main () {

        // init
        uint8_t* buffer = (uint8_t*)"Hello! I am Shashank Thakur. This is a test message. beep boop beep boop!";
        uint8_t* password = (uint8_t*)"TestKey123";
        uint8_t* salt = (uint8_t*)"saltysalt";

        // pad buffer
        uint8_t* plaintext = pad(buffer);

        printf("Buffer (%d bits): %s\n", (int)strlen((char*)buffer) * 8, buffer);
        printf("Password (%d bits): %s\n", (int)strlen((char*)password) * 8, password);
        printf("Salt (%d bits): %s\n", (int)strlen((char*)salt) * 8, salt);
        printf("Padded buffer (%d bits): %s\n", (int)strlen((char*)plaintext) * 8, plaintext);

        // key generation
        uint8_t* master_key = generate_master_key(password, salt);

        printf("Master key (%d bits): ", KEY_SIZE);
        for (int i = 0; i < KEY_SIZE / 8; i++) {
                printf("%02x", master_key[i]);
        }
        printf("\n"); 

        uint8_t* ciphertext = (uint8_t*)malloc(strlen((char*)plaintext) + 1);
        encrypt(plaintext, ciphertext, master_key, 16, FEISTEL_NETWORK);

        printf("Ciphertext (%d bits): ", (int)strlen((char*)ciphertext) * 8);
        for (size_t i = 0; i < strlen((char*)ciphertext); i++) {
                printf("%02x", ciphertext[i]);
        }
        printf("\n");

        free(plaintext);
        
        uint8_t* decrypted_text = (uint8_t*)malloc(strlen((char*)ciphertext) + 1);
        decrypt(ciphertext, decrypted_text, master_key, 16, FEISTEL_NETWORK);

        printf("Decrypted text (%d bits): %s\n", (int)strlen((char*)decrypted_text) * 8, (char*)decrypted_text);

        free(ciphertext);
        free(decrypted_text);
        free(master_key);

        return 0;
}