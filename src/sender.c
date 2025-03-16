#include "can_socket.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read_message(char *input) {
    printf("> ");
    memset(input, 0, AES_BLOCK_SIZE + 1);
    fgets(input, AES_BLOCK_SIZE + 1, stdin);
    input[strcspn(input, "\n")] = 0;
}

void send_encrypted_message(int sock, const char *message) {
    struct can_frame frame;
    unsigned char encrypted_data[AES_BLOCK_SIZE] = {0};

    char padded_message[AES_BLOCK_SIZE] = {0};
    strncpy(padded_message, message, AES_BLOCK_SIZE);

    encrypt_data((unsigned char*)padded_message, encrypted_data);

    frame.can_id = 0x123;
    frame.can_dlc = 8;
    memcpy(frame.data, encrypted_data, 8);
    send_can_frame(sock, &frame);

    frame.can_dlc = 8;
    memcpy(frame.data, encrypted_data + 8, 8);
    send_can_frame(sock, &frame);
}

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

    char input[AES_BLOCK_SIZE + 1] = "";

    printf("Type a message to send (max 16 chars) or 'exit' to terminate:\n");

    while (1) {
        read_message(input);
        if (strcmp(input, "exit") == 0) {
            printf("Exiting sender.\n");
            break;
        }
        if (strcmp(input, "") != 0) {
            send_encrypted_message(sock, input);
            strncpy(input, "", AES_BLOCK_SIZE);
        }
    }

    close_can_socket(sock);
    printf("Sender terminated successfully.\n");
    return 0;
}
