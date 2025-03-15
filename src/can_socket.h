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
#include <openssl/aes.h>
#include <openssl/evp.h>

#define AES_BLOCK_SIZE 16

const unsigned char AES_KEY[16] = "0123456789abcdef";
const unsigned char AES_IV[16] = "abcdef9876543210";

//define common functions for sender and receiver
int create_can_socket(const char *interface);
void close_can_socket(int sock);

//define function to send one CAN frame
int send_can_frame(int sock, struct can_frame *frame);

//define function to receive one CAN frame
int receive_can_frame(int sock, struct can_frame *frame);

void encrypt_data(const unsigned char *input, unsigned char *output);
void decrypt_data(const unsigned char *input, char *output);
#endif