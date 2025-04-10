#include <cipher.h>
#include <key_schedule.h>
#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define SALT "aeris256ciphersalt"

int main (int argc, char* argv[]) {

        // ./cipher <input_file_path> <enc/dec> <password>

        if (argc != 4) {
                printf("Usage: %s <input_file_path> <enc/dec> <password>\n", argv[0]);
                return 1;
        }

        // read input file
        FILE* input_file = fopen(argv[1], "rb");
        if (input_file == NULL) {
                printf("Error: Could not open input file.\n");
                return 1;
        }
        fseek(input_file, 0, SEEK_END);
        long input_file_size = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);

        uint8_t* input = (uint8_t*) malloc(input_file_size);
        if (input == NULL) {
                printf("Error: Could not allocate memory for input.\n");
                fclose(input_file);
                return 1;
        }

        size_t bytes_read = fread(input, 1, input_file_size, input_file);
        if (bytes_read != (size_t)input_file_size) {
                printf("Error: Could not read input file.\n");
                free(input);
                fclose(input_file);
                return 1;
        }
        fclose(input_file);

        int input_length = bytes_read;
        int length = input_length * 8;

        uint8_t* password = (uint8_t*) argv[3];
        uint8_t* salt = (uint8_t*) SALT;
        uint8_t* master_key = generate_master_key(password, salt);

        if (strcmp(argv[2], "enc") == 0) {

                uint64_t* buffer_plaintext = pad(input, &length);
                int num_block = length / BLOCK_SIZE;

                uint64_t* buffer_ciphertext = (uint64_t*) malloc(sizeof(uint64_t) * num_block);

                clock_t start_time = clock();
                encrypt(buffer_plaintext, buffer_ciphertext, master_key, &length);
                clock_t end_time = clock();
                double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

                // write ciphertext to file
                FILE* output_file = fopen("encrypted", "wb");
                if (output_file == NULL) {
                        printf("Error: Could not open output file.\n");
                        free(buffer_plaintext);
                        free(master_key);
                        free(buffer_ciphertext);
                        return 1;
                }
                fwrite(buffer_ciphertext, sizeof(uint64_t), num_block, output_file);
                fclose(output_file);

                printf("Encryption successful. Ciphertext written to ciphertext.bin\n");
                printf("Time taken: %.5f seconds\n", time_taken);
                free(buffer_ciphertext);
                free(buffer_plaintext);
                
        } else if (strcmp(argv[2], "dec") == 0) {
                uint64_t* buffer_decrypted = (uint64_t*) malloc(sizeof(uint8_t) * input_length);
                if (buffer_decrypted == NULL) {
                        printf("Error: Could not allocate memory for decrypted buffer.\n");
                        free(master_key);
                        free(input);
                        return 1;
                }

                clock_t start_time = clock();
                decrypt((uint64_t*)input, buffer_decrypted, master_key, &length);
                clock_t end_time = clock();
                double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

                uint8_t* buffer_decrypted_unpadded = unpad(buffer_decrypted, input_length);

                // write decrypted plaintext to file
                FILE* output_file = fopen("decrypted", "wb");
                if (output_file == NULL) {
                        printf("Error: Could not open output file.\n");
                        free(master_key);
                        free(buffer_decrypted);
                        return 1;
                }
                fwrite(buffer_decrypted_unpadded, sizeof(uint8_t), input_length, output_file);
                fclose(output_file);

                printf("Decryption successful. Decrypted plaintext written to decrypted.txt\n");
                printf("Time taken: %.5f seconds\n", time_taken);
                free(buffer_decrypted_unpadded);
                free(buffer_decrypted);
                
        } else {
                printf("Error: Invalid operation. Use 'enc' for encryption or 'dec' for decryption.\n");
        }

        free(master_key);
        free(input);

        return 0;

}