# Packet Reference

All of these packets are sent to interface 2 via control transfer Set Report Request (0x21).

## Joystick Packets

### packet 1 (unknown)
* byte 1-2 = constant (0b 03)
* byte 3-4 = axis id

### packet 2 (unknown)
* byte 1-2 = constant (0b 04)
* byte 3-4 = axis id

### packet 3 (configuration)
* byte 1-3 = constant (0b 01 00)
* byte 4 = axis id
* byte 5-7 = constant (00 01 00)
* bytes 1-8 = axis selection, axis id at byte 4 and 8
* bytes 9-10 = xsat (0 to 1000)
* bytes 11-12 = ysat (0 to 1000)
* bytes 15-16 = curve (0 to 1000)
* bytes 13-14 = deadband (0 to 1000)
* byte 17 = profile (01 = J, 00 = S)
* bytes 18-19 = hall effect zero calibration, zero for other axis

### packet 4 (unknown, end of config ?)
bytes 1-2 = constant (01 01)

### packet 5 (RGB color set)
* bytes 1-3 = constant (09 00 03)
* bytes 4-6 = RGB color 8bit each

## Default settings
* xsat = 1000
* ysat = 1000
* curve = 500
* deadband = 0
* profile = S

## Axis id (Joystick)
* X = 30
* Y = 31
* Z = 35

## Axis id (Throttle)
* throttle 1 = 30
* throttle 2 = 31
* rotary 1 = 32
* rotary 2 = 35
* rotary 3 = 37
* rotary 4 = 36

## X-Y Calibration Process (Joystick)
1. Select X axis (packet 1 and 2)
2. Configuration packet, bytes 4-8 are zero, bytes 18-19 are zero, rest defaults
3. Select Y axis (packet 1 and 2)
4. Configuration packet, bytes 4-8 are zero, bytes 18-19 are zero, rest defaults

5. Select X axis (packet 1 and 2)
6. Apply configuration packet
7. Select Y axis (packet 1 and 2)
8. Apply configuration packet

Notes:

This must be sampling the X and Y axis, the values in bytes 18-19 are very
close to what would be expected from the zero position of the joystick.

## Calibration Process (Both)
1. Select axis (packet 1 and 2)
2. Apply configuration packet

## RGB Lighting (Both)

Send RGB color packet followed by packet 4.

## Data packets (interrupt endpoint 1)

### Joystick
* bytes 1-2 = X axis position (0 to 0xffff)
* bytes 3-4 = Y axis position (0 to 0xffff)
* bytes 5-6 = Z axis position (0 to 0xfff)
* Remaining bytes assumed to be buttons, needs checking

### Throttle
* **todo**
