#include <iostream>
#include "usb.h"
#include "options.h"

using namespace std;

int main(int argc, char* argv[])
{
    auto options = Options::getInstance();
    options->processArguments(argc, argv);

    auto Usb = new usb_root;

    cout << "X-56 configuration utility" << endl;

    if (libusb_init(&Usb->Context) < 0)
    {
        cerr << "Failed to initialise libusb." << endl;
        return -1;
    }

    // Excluding this until library version checking is added to build process.
    // libusb_set_option(Usb->Context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
    // libusb_set_debug(&Usb->Context, LIBUSB_LOG_LEVEL_WARNING)

    int count = 0;
    if ((count = Usb->listDevices(options->opt_list)) < 0)
    {
        cerr << "Failed to get device list." << endl;
        return -1;
    }

    if (count == 0)
    {
        cout << "Unable to find any compatible devices." << endl;
        return 0;
    }


    if (options->opt_setRGB)
    {
        cout << "set rgb" << endl;
        uint8_t red = options->opt_rgb[Color::COLOR_RED];
        uint8_t green = options->opt_rgb[Color::COLOR_GREEN];
        uint8_t blue = options->opt_rgb[Color::COLOR_BLUE];

            for(int i : options->opt_devices)
            {
                Usb->setRGB(i, red, green, blue);
            }
    }

    if(options->opt_get)
    {
        for(int i : options->opt_devices)
        {
            auto Dev = Usb->getDevice(i);
            Usb->claimDevice(Dev);
            for(auto Axis : Dev->Axes)
            {
                unsigned int data = Dev->getAxisData(Axis->axisID);
                cout << Axis->axisName << " position: " << data << endl;
            }
            Usb->releaseDevice(Dev);
        }
    }

}

