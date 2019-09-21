#ifndef USB_H
#define USB_H

#include <vector>
#include <libusb.h>
using namespace std;

enum device { DEV_X56_JOYSTICK, DEV_X56_THROTTLE };

typedef unsigned char byte;
typedef uint16_t word;
#define PACKET_IN (1<<7)

const uint16_t vendor_id = 0x738;

class usb_device
{
public:
    libusb_device_handle* handle;
    libusb_device* device;
    word idVendor;
    word idProduct;
    int identifier;
    int id;
    byte interface;
};

class usb_root
{
public:
    usb_root() { }
    int list_devices();
    unsigned char usb_get_configuration(libusb_device_handle* dev, bool print);
    libusb_context* context;
    vector<usb_device*> device_list;
};



byte* usb_get_status(libusb_device_handle* dev, bool print, byte bmRequestType, byte wIndex);
bool usb_set_configuration(libusb_device_handle* dev, byte config);
bool usb_set_idle(libusb_device_handle* dev, word wValue, byte wIndex);
bool usb_set_report_request(libusb_device_handle* dev, word wValue, word wIndex, byte* data, word wLength);
#endif
