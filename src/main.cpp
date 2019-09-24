#include <iostream>
#include "usb.h"
#include "options.h"
#include <getopt.h>
#include <string>

using namespace std;

byte PACKET_END[63] = {0x01, 0x01};

void printHelp()
{
    cout << "Option summary" << endl;

    cout << "\tno options\t\t List supported devices" << endl;
    cout << "\t-h, --help\t\t Display help" << endl;
    cout << "\t-v, --verbose\t\t Display verbose output" << endl;
    cout << "\t-d [device]\t\t Select device" << endl;
    cout << "\t--rgb [R,G,B]\t\t Set RGB lighting color [0-255]" << endl;
    cout << "\t--get\t\t\t Display current configuration" << endl;
    cout << "\t-a [axis]\t\t Select axis or axes (comma separated)" << endl;
    cout << "\t-x [value]\t\t Set the x saturation value [0-1000]" << endl;
    cout << "\t-y [value]\t\t Set the y saturation value [0-1000]" << endl;
    cout << "\t-c [value]\t\t Set the curvature value [0-1000]" << endl;
    cout << "\t-z, --deadzone [value]\t Set the deadzone value [0-1000]" << endl;
    cout << "\t--curve-type [value]\t Set the curve type [j or s]" << endl;
    cout << "\t--defaults\t\t Reset configuration to default" << endl;
    cout << "\t--calibrate\t\t Recalibrate the hall effect sensors and reset config" << endl;
    exit(0);
}

void processArguments(int argc, char* argv[])
{
    static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"deadzone", required_argument, 0, 'z'},
        {"rgb", required_argument, 0, 3},
        {"get", no_argument, 0, 4},
        {"curve-type", required_argument, 0, 5},
        {"defaults", no_argument, 0, 6},
        {"calibrate", no_argument, 0, 7},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;
    int value = 0;

    while(c != -1)
    {
        c = getopt_long (argc, argv, "hva:x:y:d:c:z:", long_options, &option_index);
        switch(c)
        {
            case 'h':
                printHelp();
                break;
            case 'v':
                Options::opt_verbose = true;
                break;
            case 'z':
                value = stoi(string(optarg));
                if (value >= 0 && value <= 1000)
                {
                    Options::opt_z = value;
                    break;
                }
                else
                {
                    cerr << "Specified deadzone is out of range." << endl;
                    exit(-1);
                }
                break;
            case 3:
                cout << "Set RGB " << optarg << endl;
                break;
            case 4:
                cout << "Get configuration" << endl;
                break;
            case 5:
                cout << "Curve type " << optarg << endl;
                break;
            case 6:
                cout << "Defaults" << endl;
                break;
            case 7:
                cout << "Calibrate" << endl;
                break;
            case 'x':
                cout << "X axis " << optarg << endl;
                break;
            case 'y':
                cout << "y axis " << optarg << endl;
                break;
            case 'a':
                cout << "Axis select " << optarg << endl;
                break;
            case 'd':
                cout << "Device " << optarg << endl;
                break;
            case 'c':
                cout << "Curvature " << optarg << endl;
                break;
            case '?':
                break;
            default:
                exit(-1);
        }
    }
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
    processArguments(argc, argv);
    auto Usb = new usb_root;

    /*
        Excluding this until library version checking is added to build process.
        libusb_set_option(&Usb->Context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
        libusb_set_debug(&Usb->Context, LIBUSB_LOG_LEVEL_WARNING)
    */

    cout << "X-56 configuration utility" << endl;

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
