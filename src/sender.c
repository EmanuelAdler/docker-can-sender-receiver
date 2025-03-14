#include "can_socket.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

const unsigned char AES_KEY[16] = "0123456789abcdef";

void encrypt_data(const char *input, unsigned char *output) {
    AES_KEY enc_key;
    AES_set_encrypt_key(AES_KEY, 128, &enc_key);
    AES_encrypt((const unsigned char*)input, output, &enc_key);
}

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

    struct can_frame frame;
    char input[AES_BLOCK_SIZE];
    unsigned char encrypted_data[AES_BLOCK_SIZE];

    printf("Type a message to send (max 16 chars) or 'exit' to terminate:\n");

    while (1) {
        printf("> ");
        memset(input, 0, AES_BLOCK_SIZE);
        fgets(input, AES_BLOCK_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Exiting sender.\n");
            break;
        }

        encrypt_data(input, encrypted_data);

        frame.can_id = 0x123;
        frame.can_dlc = 8;
        memcpy(frame.data, encrypted_data, 8);
        send_can_frame(sock, &frame);

        frame.can_dlc = 8;
        memcpy(frame.data, encrypted_data + 8, 8);
        send_can_frame(sock, &frame);
    }

    close_can_socket(sock);
    printf("Sender terminated successfully.\n");
    return 0;

}

