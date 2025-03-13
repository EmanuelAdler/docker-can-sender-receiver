#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

//define common functions for sender and receiver
int create_can_socket(const char *interface);
void close_can_socket(int sock);

//define function to send one CAN frame
int send_can_frame(int sock, struct can_frame *frame);

//define function to receive one CAN frame
int receive_can_frame(int sock, struct can_frame *frame);

#endif