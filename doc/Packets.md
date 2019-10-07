# Packet Reference

All control transfers are sent on interface 2.

## Packets

### packet 1 (unknown) [feature set]
* byte 1-2 = constant (0b 03)
* byte 3-4 = axis id

### packet 2 (unknown) [feature set]
* byte 1-2 = constant (0b 04)
* byte 3-4 = axis id

### packet 3 (configuration set) [feature set]
* byte 1-3 = constant (0b 01 00)
* byte 4 = axis id
* byte 5-7 = constant (00 01 00)
* bytes 1-8 = axis selection, axis id at byte 4 and 8
* bytes 9-10 = xsat (0 to 1000)
* bytes 11-12 = ysat (0 to 1000)
* bytes 15-16 = curvature (0 to 1000)
* bytes 13-14 = deadband (0 to 1000)
* byte 17 = profile (01 = J, 00 = S)
* bytes 18-19 = hall effect zero calibration, zero for other axis

### packet 4 (unknown, end of config ?) [feature set]
bytes 1-2 = constant (01 01)

### packet 5 (RGB color set) [feature set]
* bytes 1-3 = constant (09 00 03)
* bytes 4-6 = RGB color 8bit each

### packet 6 (get current configuration - first axis only) [feature request]
* bytes 1-17 = axid id (repeated)
* bytes 28-29 = xsat (0 to 1000)
* bytes 30-31 = ysat (0 to 1000)
* bytes 32-33 = deadband (0 to 1000)
* bytes 34-35 = curvature (0 to 100)
* byte 36 = profile (01 = J, 00 = S)
* bytes 37-38 = hall effect zero calibration (if applicable)

### packet 7 (request current configuration) [feature set]
* bytes 1-3 = constant (0b 02 00)
* byte 4 = axis id

### packet 8 (current configuration) [interrupt in]
* byte 1 = constant (ff)
* bytes 2-4 = constant (0b 02 00)
* byte 5 = axis id
* bytes 10-11 = xsat (0 to 1000)
* bytes 12-13 = ysat (0 to 1000)
* bytes 14-15 = deadband (0 to 1000)
* bytes 16-17 = curvature (0 to 1000)
* byte 18 = profile (01 = J, 00 = S)
* bytes 19-20 = hall effect zero calibration (if applicable)

## Default settings
* xsat = 1000
* ysat = 1000
* curve = 500
* deadband = 0
* profile = S or J

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

X and Y axis position needs to be sampled and averaged, average written to bytes 18-19.

## Configuration Process (joystick & throttle)
1. Select axis (packet 1 and 2)
2. Apply configuration packet

## RGB Lighting (joystick & throttle)

Send RGB color packet followed by packet 4.

## Get current configuration (joystick & throttle)
Feature request with report id 8 can be used to obtain the first axis configuration only.

Use packet 7 to get configuration for any axis, configuration is returned by interrupt on endpoint 2 (see packet 8).
