#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>
#include <usbdrvce.h>

#define CONTROLLER_DEADZONE (10000)

typedef enum {
    XBC_DPAD_U       = 1 << 0,
    XBC_DPAD_D       = 1 << 1,
    XBC_DPAD_L       = 1 << 2,
    XBC_DPAD_R       = 1 << 3,
    XBC_START        = 1 << 4,
    XBC_BACK         = 1 << 5,
    XBC_LSTICK_PRESS = 1 << 6,
    XBC_RSTICK_PRESS = 1 << 7,

    XBC_LB           = 1 << 8,
    XBC_RB           = 1 << 9,
    XBC_LOGO         = 1 << 10,
    // bit 3 of byte 2 unused
    XBC_A            = 1 << 12,
    XBC_B            = 1 << 13,
    XBC_X            = 1 << 14,
    XBC_Y            = 1 << 15
 
} xbc_digital_button_t;


typedef struct {

    uint8_t message_type;     // should always be 0x00
    uint8_t length;           // should always be 0x14 (20)
    uint16_t digital_buttons; // packed bits
    uint8_t lt;               // analog triggers (0 = not pressed, 244 = fully pressed)
    uint8_t rt;
    int16_t lx;               // left analog stick (signed)
    int16_t ly;
    int16_t rx;               // right analog stick
    int16_t ry;
    uint8_t extra[10];

} xbc_control_packet_t;

extern xbc_control_packet_t xbc_packet;

#endif
