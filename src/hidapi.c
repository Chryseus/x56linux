#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <hidapi.h>
#include <libusb.h>

typedef unsigned char byte;
typedef uint16_t word;

byte* get_color_packet(byte red, byte green, byte blue)
{
    byte* rgb_packet = malloc(sizeof(byte)*64);
    rgb_packet[0] = 0x09;
    rgb_packet[1] = 0x00;
    rgb_packet[2] = 0x03;
    rgb_packet[3] = red;
    rgb_packet[4] = green;
    rgb_packet[5] = blue;
    for (int i = 6; i <= 63; i++)
    {
        rgb_packet[i] = 0x00;
    }
    return rgb_packet;
}

int main()
{
    byte* color = get_color_packet(0,255,0);
    hid_device* handle;
    int r = hid_init();
    handle = hid_open(0x738, 0x2221, NULL);
    hid_send_feature_report(handle, color, 64);
    r = hid_exit();
    free(color);
    return 0;
}
