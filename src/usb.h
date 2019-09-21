#ifndef USB_H
#define USB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libusb.h>

typedef unsigned char byte;
typedef uint16_t word;
#define PACKET_IN (1<<7)

unsigned char usb_get_configuration(libusb_device_handle* dev, bool print);
byte* usb_get_status(libusb_device_handle* dev, bool print, byte bmRequestType, byte wIndex);
bool usb_set_configuration(libusb_device_handle* dev, byte config);
bool usb_set_idle(libusb_device_handle* dev, word wValue, byte wIndex);
bool usb_set_report_request(libusb_device_handle* dev, word wValue, word wIndex, byte* data, word wLength);
#endif