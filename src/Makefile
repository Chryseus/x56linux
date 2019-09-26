CC=g++
CFLAGS=-std=c++11 -Wall $(shell pkg-config --cflags libusb-1.0)
LDFLAGS= $(shell pkg-config --libs libusb-1.0)

x56: usb.cpp options.cpp main.cpp
		$(CC) $(CFLAGS) $? -o $@ $(LDFLAGS)

#x56-2: hidapi.c
#	$(CC) $(CFLAGS) $? -o $@ $(LDFLAGS)
