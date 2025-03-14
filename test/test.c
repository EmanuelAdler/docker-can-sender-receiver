#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <CUnit/Basic.h>
#include "can_socket.h"

#define TEST_INTERFACE "vcan0"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void test_create_can_socket() {
    int sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    if (sock >= 0) close_can_socket(sock);
}

void test_send_receive_can_frame() {
    int sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    if (sock < 0) return;

    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.can_id = 0x123;
    frame.can_dlc = 2;
    frame.data[0] = 0xAB;
    frame.data[1] = 0xCD;

    CU_ASSERT(send_can_frame(sock, &frame) == 0);
    
    struct can_frame received_frame;
    CU_ASSERT(receive_can_frame(sock, &received_frame) == 0);
    
    close_can_socket(sock);
}

void test_close_can_socket() {
    int sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    if (sock >= 0) close_can_socket(sock);
}

int main() {
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    pSuite = CU_add_suite("CAN Socket Test Suite", init_suite, clean_suite);
    if (!pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(pSuite, "Test CAN socket creation", test_create_can_socket) ||
        !CU_add_test(pSuite, "Test send and receive CAN frame", test_send_receive_can_frame) ||
        !CU_add_test(pSuite, "Test close CAN socket", test_close_can_socket)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}