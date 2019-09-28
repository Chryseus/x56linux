#ifndef USB_H
#define USB_H

#include <vector>
#include <string>
#include <cstddef>
#include <libusb-1.0/libusb.h>
#include "axis.h"
using namespace std;



class usb_device
{
public:
    libusb_device_handle* Handle;
    libusb_device* Device;
    word idVendor;
    word idProduct;
    int identifier;
    int id;
    unsigned char interface;
    unsigned char bus;
    unsigned char port;
    vector<Axis*> Axes;
};

class usb_root
{
public:
    usb_root() { }
    int listDevices();
    unsigned char usbGetConfiguration(usb_device& Device, bool print);
    unsigned char* usbGetStatus(usb_device& Device, bool print, unsigned char bmRequestType, unsigned char wIndex);
    bool usbSetConfiguration(usb_device& Device, unsigned char config);
    bool usbSetIdle(usb_device& Device, word wValue, unsigned char wIndex);
    bool usbSetReportRequest(usb_device& Device, word wValue, word wIndex, unsigned char* data, word wLength);
    libusb_context* Context;
    vector<usb_device*> DeviceList;
};




#endif
