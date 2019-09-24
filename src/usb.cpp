#include <iostream>
#include "usb.h"

using namespace std;

int usb_root::listDevices()
{
    libusb_device** DeviceList;
    ssize_t deviceCount;
    int found_devices = 0;

    deviceCount = libusb_get_device_list(this->Context, &DeviceList);
    if ( deviceCount < 0 )
    {
        cerr <<  "Failed to get device list." << endl;
        return -1;
    }
    cout << "Looking for compatible devices." << endl;

    /* Iterate through all devices and add matching ones to a list */
    for (auto i = 0; i < deviceCount; i++)
    {
        struct libusb_device_descriptor Descriptor;
        auto Temp = DeviceList[i];

        if (libusb_get_device_descriptor(Temp, &Descriptor) < 0 )
        {
            cerr << "Failed to get device descriptor." << endl;
            return -1;
        }

        if (Descriptor.idVendor == VENDOR_ID)
        {
            if(Descriptor.idProduct == 0x2221)
            {
                auto Device = new usb_device;
                Device->Device = Temp;
                Device->idVendor = Descriptor.idVendor;
                Device->idProduct = Descriptor.idProduct;
                Device->identifier = DEV_X56_JOYSTICK;
                Device->interface = 2;
                if(libusb_open(Device->Device, &Device->Handle) !=  0)
                {
                    cerr << "Failed to get device handle, are you root?." << endl;
                    return -1;
                }
                Device->bus = libusb_get_bus_number(Temp);
                Device->port = libusb_get_device_address(Temp);
                Device->id = this->DeviceList.size()+1;
                this->DeviceList.push_back(Device);
                cout << this->DeviceList.size() << ": X-56 Joystick, bus "
                    << static_cast<int>(libusb_get_bus_number(Temp)) << " device "
                    << static_cast<int>(libusb_get_device_address(Temp)) << endl;
                found_devices++;
            }
            if(Descriptor.idProduct == 0xa221)
            {
                auto Device = new usb_device;
                Device->Device = Temp;
                Device->idVendor = Descriptor.idVendor;
                Device->idProduct = Descriptor.idProduct;
                Device->identifier = DEV_X56_THROTTLE;
                Device->interface = 2;
                if(libusb_open(Device->Device, &Device->Handle) !=  0)
                {
                    cerr << "Failed to get device handle, are you root?." << endl;
                    return -1;
                }
                Device->id = this->DeviceList.size()+1;
                this->DeviceList.push_back(Device);
                cout << this->DeviceList.size() << ": X-56 Throttle, bus "
                    << static_cast<int>(libusb_get_bus_number(Temp)) << " device "
                    << static_cast<int>(libusb_get_device_address(Temp)) << endl;
                found_devices++;
            }
        }
    }
    libusb_free_device_list(DeviceList, 1);
    return found_devices;
}



unsigned char usb_root::usbGetConfiguration(usb_device& Device, bool print)
{
    if(Device.Handle)
    {
        unsigned char configuration = 0xFF;
        int bytes = libusb_control_transfer(Device.Handle, 0x80, 8, 0,0, &configuration, 1, 4000);
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


bool usb_root::usbSetConfiguration(usb_device& Device, byte config)
{
    if(Device.Handle)
    {
        int bytes = libusb_control_transfer(Device.Handle, 0x00, 9, config, 0, NULL, 0, 4000);
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

byte* usb_root::usbGetStatus(usb_device& Device, bool print, byte bmRequestType, byte wIndex)
{
    byte* status = new(byte[1]);
    if(Device.Handle)
    {
        int bytes = libusb_control_transfer(Device.Handle, bmRequestType, 0, 0, wIndex, status, 2, 4000);
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

bool usb_root::usbSetIdle(usb_device& Device, word wValue, byte wIndex)
{
    if(Device.Handle)
    {
        int bytes = libusb_control_transfer(Device.Handle, 0x21, 0x0A, wValue, wIndex, NULL, 0, 4000);
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

bool usb_root::usbSetReportRequest(usb_device &Device, word wValue, word wIndex, byte* data, word wLength)
{
    if(Device.Handle)
    {
        // https://www.usb.org/sites/default/files/documents/hid1_11.pdf
        libusb_control_transfer(Device.Handle, 0x21, 0x09, wValue, wIndex, data, wLength, 4000);
        return false;
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
    return true;

}
