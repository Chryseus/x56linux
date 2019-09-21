#include <iostream>
#include "usb.h"

using namespace std;

int usb_root::list_devices()
{
    libusb_device** device_list;
    ssize_t device_count;

    device_count = libusb_get_device_list(this->context, &device_list);
    if ( device_count < 0 )
    {
        cerr <<  "Failed to get device list." << endl;
        return -1;
    }

    /* Iterate through all devices and add matching ones to a list */
    for (auto i = 0; i < device_count; i++)
    {
        struct libusb_device_descriptor descriptor;
        libusb_device* temp = device_list[i];

        if (libusb_get_device_descriptor(temp, &descriptor) < 0 )
        {
            cerr << "Failed to get device descriptor." << endl;
            return -1;
        }

        if (descriptor.idVendor == vendor_id)
        {
            if(descriptor.idProduct == 0x2221)
            {
                auto dev = new usb_device;
                dev->device = temp;
                dev->idVendor = descriptor.idVendor;
                dev->idProduct = descriptor.idProduct;
                dev->identifier = DEV_X56_JOYSTICK;
                dev->interface = 2;
                if(libusb_open(dev->device, &dev->handle) !=  0)
                {
                    cerr << "Failed to get joystick handle, are you root?." << endl;
                    return -1;
                }
                dev->id = this->device_list.size()+1;
                this->device_list.push_back(dev);
                cout << this->device_list.size() << ": X-56 Joystick" << endl;
            }
            if(descriptor.idProduct == 0xa221)
            {
                auto dev = new usb_device;
                dev->device = temp;
                dev->idVendor = descriptor.idVendor;
                dev->idProduct = descriptor.idProduct;
                dev->identifier = DEV_X56_THROTTLE;
                dev->interface = 2;
                if(libusb_open(dev->device, &dev->handle) !=  0)
                {
                    cerr << "Failed to get throttle handle, are you root?." << endl;
                    return -1;
                }
                dev->id = this->device_list.size()+1;
                this->device_list.push_back(dev);
                cout << this->device_list.size() << ": X-56 Throttle" << endl;

            }
        }
    }
    libusb_free_device_list(device_list, 1);
    return 0;
}



unsigned char usb_root::usb_get_configuration(libusb_device_handle* dev, bool print)
{
    if(dev)
    {
        unsigned char configuration = 0xFF;
        int bytes = libusb_control_transfer(dev, 0x80, 8, 0,0, &configuration, 1, 4000);
        if (bytes <= 0 || configuration == 0xFF)
        {
            cerr << "GET_CONFIGURATION failed." << endl;
            exit(-1);
        }
        if(print)
        {
            cout << "GET_CONFIGURATION received: " << configuration << endl;
        }
        return configuration;
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
}


bool usb_set_configuration(libusb_device_handle* dev, byte config)
{
    if(dev)
    {
        int bytes = libusb_control_transfer(dev, 0x00, 9, config, 0, NULL, 0, 4000);
        if (bytes <= 0)
        {
            cerr << "SET_CONFIGURATION failed." << endl;
            exit(-1);
        }
        return false;
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
    return true;
}

byte* usb_get_status(libusb_device_handle* dev, bool print, byte bmRequestType, byte wIndex)
{
    byte* status = new(byte[1]);
    if(dev)
    {
        int bytes = libusb_control_transfer(dev, bmRequestType, 0, 0, wIndex, status, 2, 4000);
        if (bytes <= 0)
        {
            cerr << "GET_STATUS failed." << endl;
            exit(-1);
        }
        if(print)
        {
            cout << "GET_STATUS received: " << status[0] << " " << status[1] << endl;
        }
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
    return status;
}

bool usb_set_idle(libusb_device_handle* dev, word wValue, byte wIndex)
{
    if(dev)
    {
        int bytes = libusb_control_transfer(dev, 0x21, 0x0A, wValue, wIndex, NULL, 0, 4000);
        if (bytes < 0)
        {
            cerr << "SET_IDLE failed." << endl;
            exit(-1);
        }
        return false;
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
    return true;
}

bool usb_set_report_request(libusb_device_handle* dev, word wValue, word wIndex, byte* data, word wLength)
{
    if(dev)
    {
        // https://www.usb.org/sites/default/files/documents/hid1_11.pdf
        libusb_control_transfer(dev, 0x21, 0x09, wValue, wIndex, data, wLength, 4000);
        return false;
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
    return true;

}
