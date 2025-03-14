#include "can_socket.h"
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

const unsigned char AES_KEY[16] = "0123456789abcdef";
const unsigned char AES_IV[16] = "abcdef9876543210";

void decrypt_data(const unsigned char *input, char *output) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, AES_SECRET_KEY, AES_IV);
    EVP_DecryptUpdate(ctx, (unsigned char*)output, &len, input, AES_BLOCK_SIZE);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, (unsigned char*)output + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

    struct can_frame frame;
    unsigned char encrypted_data[AES_BLOCK_SIZE];
    char decrypted_message[AES_BLOCK_SIZE];

    printf("Listening for CAN frames...\n");

    // Receive CAN frame
    while (1) {
        if (receive_can_frame(sock, &frame) == 0) {
            printf("Received CAN ID: %X Data: ", frame.can_id);
            for (int i = 0; i < frame.can_dlc; i++) {
                printf("%02X ", frame.data[i]);
            }
            printf("\n");
            fflush(stdout);

            if (frame.can_dlc == 8) {
                memcpy(encrypted_data + received_bytes, frame.data, 8);
                received_bytes += 8;

                if (received_bytes == AES_BLOCK_SIZE) {
                    decrypt_data(encrypted_data, decrypted_message);
                    decrypted_message[AES_BLOCK_SIZE - 1] = '\0';  // Ensure null termination
                    printf("Decrypted message: %s\n", decrypted_message);
                    received_bytes = 0;  // Reset for next message
                }
            } else {
                printf("⚠️ Warning: Received unexpected frame size (%d bytes). Ignoring.\n", frame.can_dlc);
            }
        }
    }
    close_can_socket(sock);
    return 0;
}