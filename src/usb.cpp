#include <iostream>
#include "usb.h"

using namespace std;

usb_device::usb_device()
{

}

void usb_root::printPacket(uint8_t* packet, uint8_t len)
{
    if(len <=0)
    {
        cerr << "Print packet error, zero length specified." << endl;
        return;
    }
    for (auto i = 0; i < len; i++)
    {
        cout << hex << setfill('0') << setw(2) << static_cast<int>(packet[i]) << " ";
        if (i+1 > 1 && (i+1) % 8 == 0) { cout << endl; } // newline every 8 bytes
    }
    cout << endl;
}

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
                Device->identifier = static_cast<int>(device::DEV_X56_JOYSTICK);
                Device->interfaces.push_back(2);
                Device->interfaces.push_back(0);
                if(libusb_open(Device->Device, &Device->Handle) !=  0)
                {
                    cerr << "Failed to get device handle, are you root?." << endl;
                    return -1;
                }
                else
                {
                    cout << "Opened device " << Device->Device << " handle " << Device->Handle << endl;
                }
                Device->bus = libusb_get_bus_number(Temp);
                Device->port = libusb_get_device_address(Temp);
                Device->id = this->DeviceList.size()+1;

                auto axisX = new Axis(30, string("X axis"), 1000, 1000, 0, 500, string("s"), true, 0);
                auto axisY = new Axis(31, string("Y axis"), 1000, 1000, 0, 500, string("s"), true, 4);
                auto axisZ = new Axis(35, string("Z axis"), 1000, 1000, 0, 500, string("s"), false, 8);
                Device->Axes.push_back(axisX);
                Device->Axes.push_back(axisY);
                Device->Axes.push_back(axisZ);

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
                Device->identifier = static_cast<int>(device::DEV_X56_THROTTLE);
                Device->interfaces.push_back(2);
                Device->interfaces.push_back(0);
                if(libusb_open(Device->Device, &Device->Handle) !=  0)
                {
                    cerr << "Failed to get device handle, are you root?." << endl;
                    return -1;
                }
                else
                {
                    cout << "Opened device " << Device->Device << " handle " << Device->Handle << endl;
                }
                Device->id = this->DeviceList.size()+1;

                auto axisLThrottle = new Axis(30, string("Throttle left axis"), 1000, 1000, 0, 500, string("j"), true);
                auto axisRThrottle = new Axis(31, string("Throttle right axis"), 1000, 1000, 0, 500, string("j"), true);
                auto axisRot1 = new Axis(32, string("Rotary 1 axis"), 1000, 1000, 0, 500, string("s"), false);
                auto axisRot2 = new Axis(35, string("Rotary 2 axis"), 1000, 1000, 0, 500, string("s"), false);
                auto axisRot3 = new Axis(37, string("Rotary 3 axis"), 1000, 1000, 0, 500, string("s"), false);
                auto axisRot4 = new Axis(36, string("Rotary 4 axis"), 1000, 1000, 0, 500, string("s"), false);
                Device->Axes.push_back(axisLThrottle);
                Device->Axes.push_back(axisRThrottle);
                Device->Axes.push_back(axisRot1);
                Device->Axes.push_back(axisRot2);
                Device->Axes.push_back(axisRot3);
                Device->Axes.push_back(axisRot4);

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

unsigned char usb_root::usbGetConfiguration(usb_device* Device, bool print)
{
    if(Device->Handle)
    {
        unsigned char configuration = 0xFF;
        int bytes = libusb_control_transfer(Device->Handle, 0x80, 8, 0,0, &configuration, 1, 4000);
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

bool usb_root::usbSetConfiguration(usb_device* Device, unsigned char config)
{
    if(Device->Handle)
    {
        int bytes = libusb_control_transfer(Device->Handle, 0x00, 9, config, 0, NULL, 0, 4000);
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

unsigned char* usb_root::usbGetStatus(usb_device* Device, bool print, unsigned char bmRequestType, unsigned char wIndex)
{
    unsigned char* status = new(unsigned char[1]);
    if(Device->Handle)
    {
        int bytes = libusb_control_transfer(Device->Handle, bmRequestType, 0, 0, wIndex, status, 2, 4000);
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

bool usb_root::usbSetIdle(usb_device* Device, word wValue, unsigned char wIndex)
{
    if(Device->Handle)
    {
        int bytes = libusb_control_transfer(Device->Handle, 0x21, 0x0A, wValue, wIndex, NULL, 0, 4000);
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

bool usb_root::usbSetReportRequest(usb_device* Device, word wValue, word wIndex, uint8_t* data, word wLength)
{
    printPacket(data, 64);
    if(Device->Handle)
    {
        // https://www.usb.org/sites/default/files/documents/hid1_11.pdf
        libusb_control_transfer(Device->Handle, 0x21, 0x09, wValue, wIndex, data, wLength, 4000);
        return false;
    }
    else
    {
        cerr << "Invalid device handle." << endl;
        exit(-1);
    }
    return true;

}

void usb_root::setRGB(int deviceID, uint8_t red, uint8_t green, uint8_t blue)
{
    unsigned char packetEnd[63] = {0x01, 0x01};
    uint8_t colorPacket[63];
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

    for (auto dev : this->DeviceList)
    {
        if (dev->id == deviceID)
        {
            this->claimDevice(dev);
            this->usbSetReportRequest(dev, 0x309, (2<<8), colorPacket, 64);
            this->usbSetReportRequest(dev, 0x300, (2<<8), packetEnd, 64);
            this->releaseDevice(dev);
        }
    }


}

void usb_root::claimDevice(usb_device* Device)
{
    for(auto iface : Device->interfaces)
    {
        if (libusb_detach_kernel_driver(Device->Handle, iface) == 0)
        {
            // cout << "Detached kernel driver." << endl;
        }
        if (libusb_claim_interface(Device->Handle, iface) != 0)
        {
            cerr << "Could not claim device " << Device->id << endl;
            exit(-1);
        }
    }
}

void usb_root::releaseDevice(usb_device* Device)
{
    for(auto iface : Device->interfaces)
    {
        libusb_release_interface(Device->Handle, iface);
        libusb_attach_kernel_driver(Device->Handle, iface);
    }
}

usb_device* usb_root::getDevice(int deviceID)
{
    for (auto dev : this->DeviceList)
    {
        if (dev->id == deviceID)
        {
            return dev;
        }
    }
    return nullptr;
}

unsigned int usb_device::getAxisData(unsigned char axisID)
{
    uint8_t* data = new  uint8_t[63];
    unsigned int position = 0;
    stringstream ss;
    for(int i = 0; i <= 63; i++)
    {
        data[i] = 0;
    }
    for (auto axis : this->Axes)
    {
        if(axis->axisID == axisID)
        {
            libusb_interrupt_transfer(this->Handle, 0x81, data, 64, nullptr, 0); // endpoint 1
            usb_root::printPacket(data, 64);
            ss << (data[1] << 8) + data[0];
            ss >> hex >> position;
            delete[] data;
            return position;
        }
    }
    return 0;
}
