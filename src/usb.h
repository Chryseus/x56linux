#ifndef USB_H
#define USB_H

#include <vector>
#include <libusb-1.0/libusb.h>
using namespace std;

enum device { DEV_X56_JOYSTICK, DEV_X56_THROTTLE };

typedef uint8_t byte;
typedef uint16_t word;
#define PACKET_IN (1<<7)

const uint16_t VENDOR_ID = 0x738;

class usb_device
{
public:
    libusb_device_handle* Handle;
    libusb_device* Device;
    word idVendor;
    word idProduct;
    int identifier;
    int id;
    byte interface;
    byte bus;
    byte port;
};

class usb_root
{
public:
    usb_root() { }
    int listDevices();
    unsigned char usbGetConfiguration(usb_device& Device, bool print);
    byte* usbGetStatus(usb_device& Device, bool print, byte bmRequestType, byte wIndex);
    bool usbSetConfiguration(usb_device& Device, byte config);
    bool usbSetIdle(usb_device& Device, word wValue, byte wIndex);
    bool usbSetReportRequest(usb_device& Device, word wValue, word wIndex, byte* data, word wLength);
    libusb_context* Context;
    vector<usb_device*> DeviceList;
};




#endif
