#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libusb.h>

typedef unsigned char byte;

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

void print_packet(byte* packet)
{
    for (int i = 0; i < 64; i++)
    {
        printf("%02x ", packet[i]);
        if (i+1 > 1 && (i+1) % 8 == 0) { printf("\n"); }
    }
    printf("\n");
}

int main(int argc, char* argv[])
{

    const uint16_t vendor_id = 1848; // 0x738
    const uint16_t product_id_stick =  8737; // 0xa221
    const uint16_t product_id_throttle = 41505; // 0x2221
    // const unsigned char endpoint = 0;
    const uint8_t bmRequestType = 0x21;
    const uint8_t bRequest = 9;
    const uint16_t wValue = 0x030b;
    const uint16_t wValue_end_packet = 0x0301;
    const uint16_t wValue_color_packet = 0x0309;
    const uint16_t wIndex = 2;
    const uint16_t wLength = 64;
    const unsigned int timeout = 2000;



    byte packet_end[63] = {0x01, 0x01};
    byte req_a[63] = {0x09, 0x02, 0x02};
    byte req_b[63] = {0x09, 0x02, 0x03};
    byte req_c[63] = {0x0b, 0x02, 0x00, 0x30};
    byte req_d[63] = {0x0b, 0x02, 0x00, 0x31};
    byte req_e[63] = {0x0b, 0x02, 0x00, 0x35};
    byte req_f[63] = {0x0b, 0x02, 0x00, 0x34};
    byte req_g[63] = {0x0b, 0x02, 0x00, 0x33};


    libusb_context* context = NULL;
    libusb_device_handle* stick_handle = NULL;
    libusb_device_handle* throttle_handle = NULL;
    libusb_device** device_list;
    ssize_t device_count;


    if (libusb_init(&context) != 0)
    {
        fprintf(stderr, "Failed to initialise libusb.\n");
        return -1;
    }

    libusb_set_option(context, LIBUSB_LOG_LEVEL_INFO);

    if ((device_count = libusb_get_device_list(context, &device_list)) < 0)
    {
        fprintf(stderr, "Failed to get device list.\n");
        return -1;
    }
    printf("Found %d devices.\n", (int)device_count);

    // Find the matching usb devices
    for (int i = 0; i < device_count; i++)
    {
        struct libusb_device_descriptor descriptor;
        libusb_device* temp = device_list[i];
        if (libusb_get_device_descriptor(temp, &descriptor) < 0 )
        {
            fprintf(stderr, "Failed to get device descriptor.\n");
            return -1;
        }
        if (descriptor.idVendor == vendor_id)
        {
            if (descriptor.idProduct == product_id_stick)
            {
                printf("Found X-56 joystick.\n");
                if(libusb_open(temp, &stick_handle) !=  0)
                {
                    fprintf(stderr, "Failed to get joystick handle, are you root?.\n");
                    return -1;
                }
            }
            if (descriptor.idProduct == product_id_throttle)
            {
                printf("Found X-56 throttle.\n");
                if(libusb_open(temp, &throttle_handle) !=  0)
                {
                    fprintf(stderr, "Failed to get throttle handle, are you root?.\n");
                    return -1;
                }
            }
        }
    }
    libusb_free_device_list(device_list, 1);
    if (libusb_set_auto_detach_kernel_driver(stick_handle, 1) != LIBUSB_SUCCESS)
    { printf("Auto detach not available.\n"); return -1; }
    libusb_set_auto_detach_kernel_driver(throttle_handle, 1);

    byte* color = get_color_packet(255,20,20);
    print_packet(color);
    print_packet(packet_end);
    printf("%d\n", sizeof(color));
    if (stick_handle)
    {
        int bytes = 0;
        //libusb_detach_kernel_driver(stick_handle, 0);
        if (libusb_claim_interface(stick_handle, 2) != 0)
        {
            fprintf(stderr,"Could not claim interface.\n");
            return -1;
        }
        libusb_set_configuration(stick_handle, 1);
        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x0309,
            wIndex, (unsigned char*)req_a, wLength, timeout);

        libusb_control_transfer(stick_handle, 0xa1, 1, 0x0308,
            wIndex, 0, 0, timeout);

        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x0309,
            wIndex, (unsigned char*)req_b, wLength, timeout);
        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x030b,
            wIndex, (unsigned char*)req_c, wLength, timeout);
        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x030b,
            wIndex, (unsigned char*)req_d, wLength, timeout);
        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x030b,
            wIndex, (unsigned char*)req_e, wLength, timeout);
        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x030b,
            wIndex, (unsigned char*)req_f, wLength, timeout);
        libusb_control_transfer(stick_handle, bmRequestType, bRequest, 0x030b,
            wIndex, (unsigned char*)req_g, wLength, timeout);
        bytes = libusb_control_transfer(stick_handle, bmRequestType, bRequest, wValue_color_packet,
            wIndex, (unsigned char*)color, wLength, timeout);
        printf("%d bytes sent.\n", bytes);
        bytes = libusb_control_transfer(stick_handle, bmRequestType, bRequest, wValue_end_packet,
            wIndex, packet_end, wLength, timeout);
        printf("%d bytes sent.\n", bytes);
        //libusb_bulk_transfer(stick_handle, 0x03, color, wLength, NULL, timeout);
        //libusb_bulk_transfer(stick_handle, 0x03, packet_end, wLength, NULL, timeout);
        libusb_release_interface(stick_handle, 2);
    }


    // Cleanup
    free(color);
    if (stick_handle) { libusb_close(stick_handle); }
    if (throttle_handle) { libusb_close(throttle_handle); }



}
