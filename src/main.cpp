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
    if ((count = Usb->listDevices()) < 0)
    {
        cerr << "Failed to get device list." << endl;
        return -1;
    }
    Usb->checkDevices();

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
