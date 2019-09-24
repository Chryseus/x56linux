# CLI Program Arguments#

**No arguments**  
Lists compatible devices in the format: device_id: device_name, bus \<usb_bus> device \<usb_device>

**-h, --help**  
Displays a summary of command line arguments

**-v, --verbose**  
Verbose output

**-d** \<device_id>  
Select device by ID

**--rgb=**\<color>  
Sets the lighting color, correct format is R,G,B where the value of each is between 0 and 255, I.E --rgb=255,100,255

**--get**  
Gets the current axes configuration

**-a** \<axis_id>  
Select the axis by id, multiple may be selected by comma separating them I.E (1,2,3)

**-x** \<value>  
Set the x saturation for the selected axes (0-1000)

**-y** \<value>  
Set the y saturation for the selected axes (0-1000)

**-d** \<value>  
Set the deadzone for the selected axes (0-1000)

**-c** \<value>  
Set the curvature value for the selected axes (0-1000)

**--curve-type=**\<type>  
Set the curve type (j or s)

**--defaults**  
Sets all axes to default values (x=1000, y=1000, d=0, c= 500, curve=s)

**--calibrate**  
Performs hall effect sensor calibration and resets defaults, only works on joystick device

## Examples

-d 1 --get  
Get the current configuration for device 1

-d 1 -a 1,2 -d 50 -c 450  
Sets axis 1 and 2 deadzone to 50 and curvature to 450 for device 1
