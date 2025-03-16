#include "can_socket.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/can_pipe"

void send_encrypted_message(int sock, const char *message) {
    struct can_frame frame;
    unsigned char encrypted_data[AES_BLOCK_SIZE] = {0};

    char padded_message[AES_BLOCK_SIZE + 16] = {0};
    int encrypted_len = 0;
    strncpy(padded_message, message, AES_BLOCK_SIZE);

    encrypt_data((unsigned char*)padded_message, encrypted_data, &encrypted_len);

    if (encrypted_len != AES_BLOCK_SIZE) {
        printf("Unexpected encrypted data length: %d\n", encrypted_len);
        fflush(stdout);
        return;
    }

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

    mkfifo(FIFO_PATH, 0666);

    char input[AES_BLOCK_SIZE + 1];
    int fifo_fd;

    printf("Type a message to send (max 16 chars) or 'exit' to terminate:\n");

    while (1) {
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        if (fifo_fd < 0) {
            perror("Errror opening FIFO");
            continue;
        }

        memset(input, 0, sizeof(input));
        read(fifo_fd, input, AES_BLOCK_SIZE);
        close(fifo_fd);
        printf("%s\n", input);
        fflush(stdout);

        if (strcmp(input, "exit") == 0) {
            printf("Exiting sender.\n");
            break;
        }
        if (strcmp(input, "") != 0) {
            send_encrypted_message(sock, input);
        }
    }

    close_can_socket(sock);
    unlink(FIFO_PATH);
    printf("Sender terminated successfully.\n");
    return 0;
}
