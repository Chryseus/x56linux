CC=gcc
CFLAGS=-std=c11 -Wall -g -Og $(shell pkg-config --cflags libusb-1.0)
LDFLAGS=-lpthread $(shell pkg-config --libs libusb-1.0)

x56: main.c
		$(CC) $(CFLAGS) $(LDFLAGS) $? -o $@
