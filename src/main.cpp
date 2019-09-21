#include <iostream>
#include "usb.h"

using namespace std;







byte packet_end[63] = {0x01, 0x01};

void list_devices(usb_root& usb)
{

}

void process_arguments(int argc, char** argv)
{



}


byte* get_color_packet(byte red, byte green, byte blue)
{
    byte* rgb_packet = new byte[63];
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

void set_rgb(libusb_device_handle* dev, byte red, byte green, byte blue)
{
    if (dev)
    {
        auto color = get_color_packet(red, green, blue);
        usb_set_report_request(dev, 0x309, (2<<8), color, 64);
        usb_set_report_request(dev, 0x300, (2<<8), packet_end, 64);
        delete[] color;
    }
}

void print_packet(byte* packet)
{
    for (int i = 0; i < 64; i++)
    {
        cout << packet[i] << " ";
        if (i+1 > 1 && (i+1) % 8 == 0) { cout << endl; }
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    auto usb = new usb_root;

    if (libusb_init(&usb->context) != 0)
    {
        cerr << "Failed to initialise libusb." << endl;
        return -1;
    }
    libusb_set_option(usb->context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);

    usb->list_devices();

    // Find the matching usb devices


    /*
    if (stick_handle)
    {
        libusb_detach_kernel_driver(stick_handle, 2);

        if (libusb_claim_interface(stick_handle, 2) != 0)
        {
            cerr << "Could not claim joystick interface 2." << endl;
            return -1;
        }

        set_rgb(stick_handle, 0, 255 ,0);

        libusb_release_interface(stick_handle, 2);
        libusb_attach_kernel_driver(stick_handle, 2);
    }

    if (throttle_handle)
    {
        libusb_detach_kernel_driver(throttle_handle, 2);

        if (libusb_claim_interface(throttle_handle, 2) != 0)
        {
            cerr << "Could not claim throttle interface 2." << endl;
            return -1;
        }

        set_rgb(throttle_handle, 255, 0 ,0);

        libusb_release_interface(throttle_handle, 2);
        libusb_attach_kernel_driver(throttle_handle, 2);
    }

    if (stick_handle) { libusb_close(stick_handle); }
    if (throttle_handle) { libusb_close(throttle_handle); }*/
}
