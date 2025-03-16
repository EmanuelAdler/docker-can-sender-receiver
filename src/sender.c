#include "can_socket.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define CAN_INTERFACE ("vcan0")
#define CAN_ID        (0x123U)
#define CAN_DLC       (8U)
#define PERMISSIONS   (0666)
#define FIFO_PATH "/tmp/can_pipe"

void send_encrypted_message(int sock, const char *message) 
{
    struct can_frame frame;
    unsigned char encrypted_data[AES_BLOCK_SIZE] = {0};

    char padded_message[AES_BLOCK_SIZE + 16] = {0};
    int encrypted_len = 0;
    strncpy(padded_message, message, AES_BLOCK_SIZE);

    encrypt_data((unsigned char*)padded_message, encrypted_data, &encrypted_len);

    if (encrypted_len != AES_BLOCK_SIZE) 
    {
        printf("Unexpected encrypted data length: %d\n", encrypted_len);
        fflush(stdout);
        return;
    }

    frame.can_id = CAN_ID;
    frame.can_dlc = CAN_DLC;
    memcpy(frame.data, encrypted_data, CAN_DLC);
    send_can_frame(sock, &frame);

    frame.can_dlc = CAN_DLC;
    memcpy(frame.data, encrypted_data + CAN_DLC, CAN_DLC);
    send_can_frame(sock, &frame);
}

int main(void) 
{
    int sock = -1;  
    sock = create_can_socket(CAN_INTERFACE);
    if (sock < 0)
    {
        return EXIT_FAILURE;
    }

    mkfifo(FIFO_PATH, PERMISSIONS);

    char input[AES_BLOCK_SIZE + 1];
    int fifo_fd;

    printf("Type a message to send (max 16 chars) or 'exit' to terminate:\n");

    for(;;) 
    {
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        if (fifo_fd < 0) 
        {
            perror("Errror opening FIFO");
            continue;
        }

        memset(input, 0, sizeof(input));
        read(fifo_fd, input, AES_BLOCK_SIZE);
        close(fifo_fd);
        (void)printf("%s\n", input);
        (void)fflush(stdout);

        if (strcmp(input, "exit") == 0)
        {
            (void)printf("Exiting sender.\n");
            break;
        }
        if (strcmp(input, "") != 0) 
        {
            send_encrypted_message(sock, input);
        }
    }

    close_can_socket(sock);
    unlink(FIFO_PATH);
    (void)printf("Sender terminated successfully.\n");
    return EXIT_SUCCESS;
}