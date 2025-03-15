#include "can_socket.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
<<<<<<< Updated upstream
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

const unsigned char AES_KEY[16] = "0123456789abcdef";
const unsigned char AES_IV[16] = "abcdef9876543210";

void encrypt_data(const unsigned char *input, unsigned char *output) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, AES_SECRET_KEY, AES_IV);
    EVP_EncryptUpdate(ctx, output, &len, input, AES_BLOCK_SIZE);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, output + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
=======

#define AES_BLOCK_SIZE 16

void read_message(char *input) {
    printf("> ");
    memset(input, 0, AES_BLOCK_SIZE);
    fgets(input, AES_BLOCK_SIZE, stdin);
    input[strcspn(input, "\n")] = 0;
}

void send_encrypted_message(int sock, const char *message) {
    struct can_frame frame;
    unsigned char encrypted_data[AES_BLOCK_SIZE];

    encrypt_data((unsigned char*)message, encrypted_data);

    frame.can_id = 0x123;
    frame.can_dlc = 8;
    memcpy(frame.data, encrypted_data, 8);
    send_can_frame(sock, &frame);

    frame.can_dlc = 8;
    memcpy(frame.data, encrypted_data + 8, 8);
    send_can_frame(sock, &frame);
>>>>>>> Stashed changes
}

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

<<<<<<< Updated upstream
    struct can_frame frame;
    char input[AES_BLOCK_SIZE];
    unsigned char encrypted_data[AES_BLOCK_SIZE];
=======
    char input[AES_BLOCK_SIZE];
>>>>>>> Stashed changes

    printf("Type a message to send (max 16 chars) or 'exit' to terminate:\n");

    while (1) {
<<<<<<< Updated upstream
        printf("> ");
        memset(input, 0, AES_BLOCK_SIZE);
        fgets(input, AES_BLOCK_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;

=======
        read_message(input);
>>>>>>> Stashed changes
        if (strcmp(input, "exit") == 0) {
            printf("Exiting sender.\n");
            break;
        }
<<<<<<< Updated upstream

        encrypt_data((unsigned char*)input, encrypted_data);

        frame.can_id = 0x123;
        frame.can_dlc = 8;
        memcpy(frame.data, encrypted_data, 8);
        send_can_frame(sock, &frame);

        frame.can_dlc = 8;
        memcpy(frame.data, encrypted_data + 8, 8);
        send_can_frame(sock, &frame);
=======
        send_encrypted_message(sock, input);
>>>>>>> Stashed changes
    }

    close_can_socket(sock);
    printf("Sender terminated successfully.\n");
    return 0;

}

