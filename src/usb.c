#include "usb.h"

unsigned char usb_get_configuration(libusb_device_handle* dev, bool print)
{
    if(dev)
    {
        unsigned char configuration = 0xFF;
        int bytes = libusb_control_transfer(dev, 0x80, 8, 0,0, &configuration, 1, 4000);
        if (bytes <= 0 || configuration == 0xFF)
        {
            fprintf(stderr, "GET_CONFIGURATION failed.\n");
            exit(-1);
        }
        if(print)
        {
            printf("GET_CONFIGURATION received: %d\n",(unsigned int)configuration);
        }
        return configuration;
    }
    else
    {
        fprintf(stderr, "Invalid device handle.\n");
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
            fprintf(stderr, "SET_CONFIGURATION failed.\n");
            exit(-1);
        }
        return false;
    }
    else
    {
        fprintf(stderr, "Invalid device handle.\n");
        exit(-1);
    }
    return true;
}

byte* usb_get_status(libusb_device_handle* dev, bool print, byte bmRequestType, byte wIndex)
{
    byte* status = malloc(sizeof(byte)*2);
    if(dev)
    {
        int bytes = libusb_control_transfer(dev, bmRequestType, 0, 0, wIndex, status, 2, 4000);
        if (bytes <= 0)
        {
            fprintf(stderr, "GET_STATUS failed.\n");
            exit(-1);
        }
        if(print)
        {
            printf("GET_STATUS received: %01x %01x\n",status[0], status[1]);
        }
        //return configuration;
    }
    else
    {
        fprintf(stderr, "Invalid device handle.\n");
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
            fprintf(stderr, "SET_IDLE failed.\n");
            exit(-1);
        }
        return false;
    }
    else
    {
        fprintf(stderr, "Invalid device handle.\n");
        exit(-1);
    }
    return true;
}

bool usb_set_report_request(libusb_device_handle* dev, word wValue, word wIndex, byte* data, word wLength)
{
    if(dev)
    {
        // https://www.usb.org/sites/default/files/documents/hid1_11.pdf
        int bytes = libusb_control_transfer(dev, 0x21, 0x09, wValue, wIndex, data, wLength, 4000);
        if (bytes < 0)
        {
            fprintf(stderr, "Control transfer failed.\n");
            exit(-1);
        }
        return false;
    }
    else
    {
        fprintf(stderr, "Invalid device handle.\n");
        exit(-1);
    }
    return true;

}
