#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main() {
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    // Create CAN socket
    if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Set CAN interface
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(sock, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error bindind socket");
        return 1;
    }

    // Receive CAN frame
    while (1) {
        int nbytes = read(sock, &frame, sizeof(frame));
        if (nbytes < 0) {
            perror("Error receiving frame");
            return 1;
        }
        printf("Received CAN ID: %X Data: %s\n", frame.can_id, frame.data);
        fflush(stdout);
    }

    close(sock);
    return 0;
}
