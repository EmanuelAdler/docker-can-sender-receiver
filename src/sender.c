#include "can_socket.h"

int main() {
    int sock = create_can_socket("vcan0");
    if (sock < 0) return 1;

    // Set CAN frame
    struct can_frame frame;
    frame.can_id = 0x123;
    frame.can_dlc = 8;
    strcpy((char *)frame.data, "Hi CAN");

    for (int i = 0; i < 5; i++) {
        // Send CAN frame
        if (write(sock, &frame, sizeof(frame)) != sizeof(frame)) {
            perror("Error sending frame");
            close_can_socket(sock);
            return 1;
        }
        sleep(5);
    }

    printf("CAN Frame sent\n");
    close_can_socket(sock);
    return 0;
}