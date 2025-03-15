#include "can_socket.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAN_INTERFACE      ("vcan0")
#define SUCCESS_CODE       (0)
#define ERROR_CODE         (1)
#define RECEIVE_SUCCESS    (1)

int main(void)
{
    int sock = -1;
    struct can_frame frame;
    int32_t receive_result;
    
    sock = create_can_socket(CAN_INTERFACE);
    if (sock < 0)
    {
        return ERROR_CODE;
    }

    (void)memset(&frame, 0, sizeof(frame));

    for(;;)
    {
        receive_result = receive_can_frame(sock, &frame);
        if (receive_result == RECEIVE_SUCCESS)
        {
            (void)printf("Received CAN ID: %04X Data: ", (unsigned int)frame.can_id);
            for(uint8_t i = 0U; i < frame.can_dlc; i++)
            {
                (void)printf("%02X ", frame.data[i]);
            }
            (void)printf("\n");
            (void)fflush(stdout);
        }
        else if (receive_result < 0)
        {
            break;
        }
    }

    close_can_socket(sock);
    return SUCCESS_CODE;
}