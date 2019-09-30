#ifndef USB_H
#define USB_H

#include <vector>
#include <string>
#include <iomanip>
#include <libusb-1.0/libusb.h>
#include "axis.h"

using namespace std;

/** \brief Stores all information about a specific USB device. */
class usb_device
{
public:
    usb_device();
    libusb_device_handle* Handle;
    libusb_device* Device;
    word idVendor;
    word idProduct;
    int identifier; /// uses device enum
    int id;
    vector<unsigned char> interfaces; /// List of interfaces the device uses
    unsigned char bus;
    unsigned char port;
    vector<Axis*> Axes; /// List of axes
    ~usb_device() { libusb_close(this->Handle); }
};

class usb_root
{
public:
    usb_root() { }
    void printPacket(uint8_t* packet, uint8_t len);
    int listDevices();
    unsigned char usbGetConfiguration(usb_device* Device, bool print);
    unsigned char* usbGetStatus(usb_device* Device, bool print, unsigned char bmRequestType, unsigned char wIndex);
    bool usbSetConfiguration(usb_device* Device, unsigned char config);
    bool usbSetIdle(usb_device* Device, word wValue, unsigned char wIndex);
    bool usbSetReportRequest(usb_device* Device, word wValue, word wIndex, uint8_t* data, word wLength);
    void setRGB(int deviceID, uint8_t red, uint8_t green, uint8_t blue);
    void claimDevice(usb_device* Device);
    void releaseDevice(usb_device* Device);
    libusb_context* Context;
    vector<usb_device*> DeviceList;
    vector<usb_device*> getDeviceList(){ return this->DeviceList; }
    void checkDevices()
    {
        for (auto dev : this->DeviceList)
        {
            cout << "id: " << dev->id
                << " address: " << dev->Device
                << " handle: " << dev->Handle << endl;
        }
    }
};




#endif
