#include "can_socket.h"
#include <stdio.h>
#include <string.h>

#define AES_BLOCK_SIZE 16

void process_received_frame(int sock) {
    struct can_frame frame;
    unsigned char encrypted_data[AES_BLOCK_SIZE];
    char decrypted_message[AES_BLOCK_SIZE];
    int received_bytes = 0;

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
                    decrypted_message[AES_BLOCK_SIZE - 1] = '\0';
                    printf("Decrypted message: %s\n", decrypted_message);
                    received_bytes = 0;
                }
            } else {
                printf("⚠️ Warning: Unexpected frame size (%d bytes). Ignoring.\n", frame.can_dlc);
            }
        }
    }
}

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

    printf("Listening for CAN frames...\n");

    // Receive CAN frame
    process_received_frame(sock);

    close_can_socket(sock);
    return 0;
}