#ifndef SHARED_TYPES_H__
#define SHARED_TYPES_H__

#include <stdint.h>

//UPDATE THIS EVERY TIME THE FILE CHANGES!!!
#define SHARED_TYPES_VERSION 1


typedef enum
{
    UI_LED_RED,
    UI_LED_GREEN,
    UI_LED_BLUE,

    UI_LED_NUMBER, /* Sentinel*/

    UI_LED_NONE
} ui_channel_t;


//Control communication types
#define RESPONSE_BIT 0x80

//OWNER_SUBSYS_PROPERTY R-Read W-Write I-Indicate (Notify)
//To perform a property read: {OPCODE, 0}
//A response will be sent back in the form of { OPCODE | RESPONSE_BIT, length, data}
//To perform a property write: {OPCODE, length, data}
//To send a command: {OPCODE, 0}

#define PERIPHERAL_SEND_SIGFOX_TEST             0x40

#define PERIPHERAL_REFRESH_SENSORS              0x50

#endif //SHARED_TYPES_H__
