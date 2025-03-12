#include "can_socket.h"

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

    struct can_frame frame;

    // Receive CAN frame
    while (1) {
        int nbytes = read(sock, &frame, sizeof(frame));
        if (nbytes < 0) {
            perror("Error receiving frame");
            close_can_socket(sock);
            return 1;
        }
        printf("Received CAN ID: %X Data: %s\n", frame.can_id, frame.data);
        fflush(stdout);
    }

    close_can_socket(sock);
    return 0;
}