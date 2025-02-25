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

    // Set CAN frame
    frame.can_id = 0x123;
    frame.can_dlc = 8;
    strcpy((char *)frame.data, "Hi CAN");

    int i;
    for (i=1; i <= 5; i++) {
        // Enviar frame CAN
        if (write(sock, &frame, sizeof(frame)) != sizeof(frame)) {
            perror("Error sending frame");
            return 1;
        }
        sleep(5);
    }
    printf("CAN Frame sent\n");

    close(sock);
    return 0;
}
