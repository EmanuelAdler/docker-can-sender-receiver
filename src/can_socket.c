#include "can_socket.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can.h>

#define SOCKET_ERROR         (-1)
#define OPERATION_SUCCESS    (0)
#define MAX_INTERFACE_LEN    (IFNAMSIZ - 1U)
#define CAN_FRAME_SIZE       (sizeof(struct can_frame))

static int32_t validate_interface(const char *interface)
{
    const size_t len = strlen(interface);
    return (len > 0U) && (len <= MAX_INTERFACE_LEN) ? OPERATION_SUCCESS : SOCKET_ERROR;
}

int32_t create_can_socket(const char *interface)
{
    int32_t sock = SOCKET_ERROR;
    struct sockaddr_can addr;
    struct ifreq ifr;

    if (validate_interface(interface) != OPERATION_SUCCESS)
    {
        (void)fprintf(stderr, "Invalid interface name\n");
        return SOCKET_ERROR;
    }

    /* Create CAN socket */
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0)
    {
        perror("Error creating socket");
        return SOCKET_ERROR;
    }

    /* Configure interface */
    (void)memset(&ifr, 0, sizeof(ifr));
    (void)strncpy(ifr.ifr_name, interface, MAX_INTERFACE_LEN);
    ifr.ifr_name[MAX_INTERFACE_LEN] = '\0';

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("Error getting interface index");
        (void)close(sock);
        return SOCKET_ERROR;
    }

    /* Bind socket */
    (void)memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error binding socket");
        (void)close(sock);
        return SOCKET_ERROR;
    }

    return sock;
}

void close_can_socket(int32_t sock)
{
    if (sock >= 0)
    {
        (void)close(sock);
    }
}

int send_can_frame(int sock, const struct can_frame *frame)
{
    const ssize_t sent_bytes = write(sock, frame, CAN_FRAME_SIZE);
    
    if (sent_bytes != (ssize_t)CAN_FRAME_SIZE)
    {
        perror("Error sending frame");
        return SOCKET_ERROR;
    }
    
    return OPERATION_SUCCESS;
}

int32_t receive_can_frame(int32_t sock, struct can_frame *frame)
{
    const ssize_t received_bytes = read(sock, frame, CAN_FRAME_SIZE);
    
    if (received_bytes < 0)
    {
        perror("Error receiving frame");
        return SOCKET_ERROR;
    }
    
    if (received_bytes != (ssize_t)CAN_FRAME_SIZE)
    {
        (void)fprintf(stderr, "Incomplete frame received\n");
        return SOCKET_ERROR;
    }
    
    return OPERATION_SUCCESS;
}