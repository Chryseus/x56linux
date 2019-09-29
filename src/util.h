#ifndef UTIL_H
#define UTIL_H

enum class device : int { DEV_X56_JOYSTICK, DEV_X56_THROTTLE };


typedef uint16_t word;
#define PACKET_IN (1<<7)

const uint16_t VENDOR_ID = 0x738;

#endif
