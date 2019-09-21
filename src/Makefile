CC=gcc
CFLAGS=-std=c11 -Wall -g -Og $(shell pkg-config --cflags libusb-1.0 hidapi-libusb)
LDFLAGS=-lpthread $(shell pkg-config --libs libusb-1.0 hidapi-libusb)

x56: usb.c main.c
		$(CC) $(CFLAGS) $(LDFLAGS) $? -o $@

#x56-2: hidapi.c
#	$(CC) $(CFLAGS) $(LDFLAGS) $? -o $@
