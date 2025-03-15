#include "can_socket.h"

int create_can_socket(const char *interface) {
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Create CAN socket
    if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Set CAN interface
    strcpy(ifr.ifr_name, interface);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("Error getting interface index");
        close(sock);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket");
        close(sock);
        return -1;
    }

    return sock;
}

void close_can_socket(int sock) {
    close(sock);
}

int send_can_frame(int sock, struct can_frame *frame) {
    if (write(sock, frame, sizeof(*frame)) != sizeof(*frame)) {
        perror("Error sending frame");
        return -1;
    }
    return 0;
}

int receive_can_frame(int sock, struct can_frame *frame) {
    int nbytes = read(sock, frame, sizeof(*frame));
    if (nbytes < 0) {
        perror("Error receiving frame");
        return -1;
    }
    return 0;
}

void encrypt_data(const unsigned char *input, unsigned char *output) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, AES_USER_KEY, AES_USER_IV);
    EVP_EncryptUpdate(ctx, output, &len, input, AES_BLOCK_SIZE);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, output + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_data(const unsigned char *input, char *output) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, AES_USER_KEY, AES_USER_IV);
    EVP_DecryptUpdate(ctx, (unsigned char*)output, &len, input, AES_BLOCK_SIZE);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, (unsigned char*)output + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}