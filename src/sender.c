#include "can_socket.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CAN_INTERFACE ("vcan0")
#define NUM_SENDS     (5U)
#define SLEEP_TIME    (5U)
#define CAN_ID        (0x123U)
#define CAN_DLC       (8U)
#define DATA_STRING   "Hi CAN"

int main(void)
{
    int sock = -1;
    struct can_frame frame;
    uint8_t i;

    sock = create_can_socket(CAN_INTERFACE);
    if (sock < 0)
    {
        return EXIT_FAILURE;
    }

    (void)memset(&frame, 0, sizeof(frame));
    frame.can_id = CAN_ID;
    frame.can_dlc = CAN_DLC;
    (void)memcpy(frame.data, DATA_STRING, sizeof(DATA_STRING) - 1U);

    for (i = 0U; i < NUM_SENDS; i++)
    {
        if (send_can_frame(sock, &frame) < 0)
        {
            (void)close_can_socket(sock);
            return EXIT_FAILURE;
        }

        (void)sleep(SLEEP_TIME);
    }

    (void)printf("CAN Frame sent\n");
    (void)close_can_socket(sock);

    return EXIT_SUCCESS;
}