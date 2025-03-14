#include "can_socket.h"
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

const unsigned char AES_KEY[16] = "0123456789abcdef";

void decrypt_data(const unsigned char *input, char *output) {
    AES_KEY dec_key;
    AES_set_decrypt_key(AES_KEY, 128, &dec_key);
    AES_decrypt(input, (unsigned char*)output, &dec_key);
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
                static int received_bytes = 0;
                static unsigned char buffer[AES_BLOCK_SIZE];

                memcpy(buffer + received_bytes, frame.data, 8);
                received_bytes += 8;

                if (received_bytes == AES_BLOCK_SIZE) {
                    decrypt_data(buffer, decrypted_message);
                    decrypted_message[AES_BLOCK_SIZE - 1] = '\0';
                    printf("Decrypted message: %s\n", decrypted_message);
                    received_bytes = 0;
                }
            }
        }
    }

    close_can_socket(sock);
    return 0;
}