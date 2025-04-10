#include <cipher.h>
#include <key_schedule.h>
#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main () {

        // store raw bytes of input.txt in a buffer
        FILE *fp = fopen("input.txt", "rb");
        if (fp == NULL) {
                perror("Failed to open file");
                return 1;
        }
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        uint8_t* buffer = malloc(file_size);
        if (buffer == NULL) {
                perror("Failed to allocate memory");
                fclose(fp);
                return 1;
        }

        size_t bytes_read = fread(buffer, 1, file_size, fp);
        if (bytes_read != (size_t)file_size) {
                perror("Failed to read file");
                free(buffer);
                fclose(fp);
                return 1;
        }
        fclose(fp);

        int length = file_size * 8;
        uint64_t* plaintext = pad(buffer, &length);

        uint8_t* password = (uint8_t*) "password";
        uint8_t* salt = (uint8_t*) "salt";
        uint8_t* master_key = generate_master_key(password, salt);

        uint64_t* ciphertext = (uint64_t*) malloc(length / 8);
        if (ciphertext == NULL) {
                perror("Failed to allocate memory");
                free(buffer);
                free(plaintext);
                free(master_key);
                return 1;
        }

        encrypt(plaintext, ciphertext, master_key, &length);

        // write encrypted data to encrypted.txt
        FILE *fp_encrypted = fopen("encrypted.txt", "wb");
        if (fp_encrypted == NULL) {
                perror("Failed to open file");
                free(buffer);
                free(plaintext);
                free(ciphertext);
                free(master_key);
                return 1;
        }

        size_t bytes_written = fwrite(ciphertext, 1, length / 8, fp_encrypted);
        if (bytes_written != (size_t)length / 8) {
                perror("Failed to write file");
                free(buffer);
                free(plaintext);
                free(ciphertext);
                free(master_key);
                fclose(fp_encrypted);
                return 1;
        }

        fclose(fp_encrypted);

        uint64_t* decrypted = (uint64_t*) malloc(length / 8);
        if (decrypted == NULL) {
                perror("Failed to allocate memory");
                free(buffer);
                free(plaintext);
                free(ciphertext);
                free(master_key);
                return 1;
        }

        decrypt(ciphertext, decrypted, master_key, &length);
        uint8_t* unpadded_plaintext = (uint8_t*) malloc(file_size + 1);
        if (unpadded_plaintext == NULL) {
                perror("Failed to allocate memory");
                free(buffer);
                free(plaintext);
                free(ciphertext);
                free(master_key);
                free(decrypted);
                return 1;
        }

        memcpy(unpadded_plaintext, decrypted, file_size);

        // write decrypted data to decrypted.txt
        FILE *fp_decrypted = fopen("decrypted.txt", "wb");
        if (fp_decrypted == NULL) {
                perror("Failed to open file");
                free(buffer);
                free(plaintext);
                free(ciphertext);
                free(master_key);
                free(decrypted);
                free(unpadded_plaintext);
                return 1;
        }

        size_t bytes_written_decrypted = fwrite(unpadded_plaintext, 1, file_size, fp_decrypted);
        if (bytes_written_decrypted != (size_t)file_size) {
                perror("Failed to write file");
                free(buffer);
                free(plaintext);
                free(ciphertext);
                free(master_key);
                free(decrypted);
                free(unpadded_plaintext);
                fclose(fp_decrypted);
                return 1;
        }

        fclose(fp_decrypted);
        printf("Encryption and decryption completed successfully.\n");
        free(buffer);
        free(plaintext);
        free(ciphertext);
        free(master_key);
        free(decrypted);
        free(unpadded_plaintext);
        return 0;
}