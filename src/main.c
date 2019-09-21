#include "usb.h"

byte* get_color_packet(byte red, byte green, byte blue)
{
    byte* rgb_packet = malloc(sizeof(byte)*64);
    //rgb_packet[0] = 0x00;
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
    const uint16_t vendor_id = 0x738;
    const uint16_t product_id_stick =  0x2221;
    const uint16_t product_id_throttle = 0xa221;

    byte packet_end[63] = {0x01, 0x01};

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
    libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);

    // Find the matching usb devices
    if ((device_count = libusb_get_device_list(context, &device_list)) < 0)
    {
        fprintf(stderr, "Failed to get device list.\n");
        return -1;
    }

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

    byte* color = get_color_packet(0,255,0);
    print_packet(color);
    print_packet(packet_end);

    if (stick_handle)
    {
        libusb_detach_kernel_driver(stick_handle, 0);
        libusb_detach_kernel_driver(stick_handle, 2);

        if (libusb_claim_interface(stick_handle, 2) != 0)
        {
            fprintf(stderr,"Could not claim interface 1.\n");
            return -1;
        }

        usb_set_report_request(stick_handle, 0x309, (2<<8), color, 64);
        usb_set_report_request(stick_handle, 0x300, (2<<8), packet_end, 64);

        libusb_release_interface(stick_handle, 2);
        libusb_attach_kernel_driver(stick_handle, 0);
        libusb_attach_kernel_driver(stick_handle, 2);
    }

    free(color);
    if (stick_handle) { libusb_close(stick_handle); }
    if (throttle_handle) { libusb_close(throttle_handle); }
}
