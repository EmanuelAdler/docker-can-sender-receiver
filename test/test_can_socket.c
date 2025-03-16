#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "../src/can_socket.h"

#define TEST_INTERFACE ("vcan0")
#define INVALID_INTERFACE ("any_if")

#define CAN_ID  (0x123U)
#define CAN_DLC (2U)
#define DATA_0  (0XABU)
#define DATA_1  (0xCDU)

static int init_suite(void) 
{ 
    return 0; 
}

static int clean_suite(void) 
{ 
    return 0; 
}

// Struct to store receptor data
typedef struct {
    int sock;
    struct can_frame received_frame;
    int received;
} receiver_data_t;

void* receiver_thread(void* arg) {
    receiver_data_t* data = (receiver_data_t*) arg;
    
    if (receive_can_frame(data->sock, &data->received_frame) == 0) {
        data->received = 1;
    }
    
    return NULL;
}

/* Integration test */
static void test_send_receive_can_frame(void) 
{
    int sock = -1;
    int sock2 = -1;
    struct can_frame frame;
    pthread_t recv_thread;
    receiver_data_t recv_data;

    // Create socket CAN
    sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    if (sock < 0) return;

    sock2 = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock2 >= 0);
    if (sock2 < 0) return;

    // Initialize receptor struct
    recv_data.sock = sock2;
    recv_data.received = 0;
    memset(&recv_data.received_frame, 0, sizeof(struct can_frame));

    // Create receptor thread
    pthread_create(&recv_thread, NULL, receiver_thread, &recv_data);
    usleep(500000);

    // Create and send CAN message
    memset(&frame, 0, sizeof(struct can_frame));
    frame.can_id = CAN_ID;
    frame.can_dlc = CAN_DLC;
    frame.data[0] = DATA_0;
    frame.data[1] = DATA_1;
    
    CU_ASSERT(send_can_frame(sock, &frame) == 0);

    int wait_time = 50;
    while (wait_time-- > 0 && recv_data.received == 0) {
        usleep(100000);
    }

    // Validate if message was received
    CU_ASSERT(recv_data.received == 1);
    CU_ASSERT(recv_data.received_frame.can_id == CAN_ID);
    CU_ASSERT(recv_data.received_frame.can_dlc == CAN_DLC);
    CU_ASSERT(recv_data.received_frame.data[0] == DATA_0);
    CU_ASSERT(recv_data.received_frame.data[1] == DATA_1);

    // Finish receptor thread
    pthread_join(recv_thread, NULL);

    close_can_socket(sock);
    close_can_socket(sock2);
}

static void test_create_can_socket(void) 
{
    int sock = -1;
    
    sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    if (sock >= 0) 
    {
        close_can_socket(sock);
    }
}

static void test_create_can_socket_invalid(void) 
{
    const int sock = create_can_socket(INVALID_INTERFACE);
    CU_ASSERT(sock == -1);
}

static void test_close_can_socket(void) 
{
    int sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    if (sock >= 0) 
    {
        close_can_socket(sock);
    }
}

static void test_send_can_frame_invalid_socket(void) 
{
    struct can_frame frame;
    
    (void)memset(&frame, 0, sizeof(frame));
    frame.can_id = CAN_ID;
    frame.can_dlc = CAN_DLC;
    frame.data[0] = DATA_0;
    frame.data[1] = DATA_1;
    
    CU_ASSERT(send_can_frame(-1, &frame) == -1);
}

static void test_receive_can_frame_invalid_socket(void) 
{
    struct can_frame frame;
    CU_ASSERT(receive_can_frame(-1, &frame) == -1);
}

static void test_receive_can_frame_closed_socket(void) 
{
    int sock = create_can_socket(TEST_INTERFACE);
    CU_ASSERT(sock >= 0);
    
    if (sock >= 0)
    {
        close_can_socket(sock);
    }
    
    struct can_frame frame;
    CU_ASSERT(receive_can_frame(sock, &frame) == -1);
}

int main(void)
{
    CU_pSuite pSuite = NULL;
    CU_ErrorCode error_code = CUE_SUCCESS;
    bool tests_added = true;

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        CU_cleanup_registry();
        return (int)CU_get_error();
    }

    pSuite = CU_add_suite("CAN Socket Test Suite", init_suite, clean_suite);
    if (pSuite == NULL)
    {
        CU_cleanup_registry();
        return (int)CU_get_error();
    }

    /* Add individual tests */
    tests_added = tests_added && (CU_add_test(pSuite, "Test CAN socket creation", test_create_can_socket) != NULL);
    tests_added = tests_added && (CU_add_test(pSuite, "Test CAN socket creation with invalid interface", test_create_can_socket_invalid) != NULL);
    tests_added = tests_added && (CU_add_test(pSuite, "Test send and receive CAN frame", test_send_receive_can_frame) != NULL);
    tests_added = tests_added && (CU_add_test(pSuite, "Test send CAN frame with invalid socket", test_send_can_frame_invalid_socket) != NULL);
    tests_added = tests_added && (CU_add_test(pSuite, "Test receive CAN frame with invalid socket", test_receive_can_frame_invalid_socket) != NULL);
    tests_added = tests_added && (CU_add_test(pSuite, "Test receive CAN frame with closed socket", test_receive_can_frame_closed_socket) != NULL);
    tests_added = tests_added && (CU_add_test(pSuite, "Test close CAN socket", test_close_can_socket) != NULL);

    if (!tests_added)
    {
        CU_cleanup_registry();
        return (int)CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    (void)CU_basic_run_tests();
    error_code = CU_get_error();
    CU_cleanup_registry();

    return (int)error_code;
}