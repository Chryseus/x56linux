#include <iostream>
#include "usb.h"

using namespace std;

byte PACKET_END[63] = {0x01, 0x01};

void processArguments(int argc, char** argv)
{

}


byte* getColorPacket(byte red, byte green, byte blue)
{
    byte* colorPacket = new byte[63];
    colorPacket[0] = 0x09;
    colorPacket[1] = 0x00;
    colorPacket[2] = 0x03;
    colorPacket[3] = red;
    colorPacket[4] = green;
    colorPacket[5] = blue;
    for (int i = 6; i <= 63; i++)
    {
        colorPacket[i] = 0x00;
    }
    return colorPacket;
}

void setColor(usb_root &Usb, usb_device &Device, byte red, byte green, byte blue)
{
    auto Color = getColorPacket(red, green, blue);
    Usb.usbSetReportRequest(Device, 0x309, (2<<8), Color, 64);
    Usb.usbSetReportRequest(Device, 0x300, (2<<8), PACKET_END, 64);
    delete[] Color;
}

void printPacket(byte* packet)
{
    for (auto i = 0; i < 64; i++)
    {
        cout << packet[i] << " ";
        if (i+1 > 1 && (i+1) % 8 == 0) { cout << endl; }
    }
    cout << endl;
}

int main(int argc, char* argv[])
{
    auto Usb = new usb_root;

    /*
        Excluding this until library version checking is added to build process.
        libusb_set_option(&Usb->Context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
        libusb_set_debug(&Usb->Context, LIBUSB_LOG_LEVEL_WARNING)
    */

    if (libusb_init(&Usb->Context) < 0)
    {
        cerr << "Failed to initialise libusb." << endl;
        return -1;
    }


    int count = 0;
    if ((count = Usb->listDevices()) < 0)
    {
        cerr << "Failed to get device list." << endl;
        return -1;
    }

    if (count == 0)
    {
        cout << "Unable to find any compatible devices." << endl;
    }

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
