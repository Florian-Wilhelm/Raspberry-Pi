# Raspberry Pi Pico and Pico-GPS-L76B GNSS module

#### Software:

File "Pico-GPS-L76B_Code2.zip" is copied from the waveshare wiki:

https://www.waveshare.com/wiki/Pico-GPS-L76B

Python scripts in this repo are slightly modified versions of scripts in that .zip.  

#### Hardware:

There is no trouble putting the GPS module onto the standard Raspberry Pi Pico header. Button, SD-Card board and OLED are off-the-shelf components.

## Example Configuration

### Push button, Micro SD-Card and OLED SSD1306

Demonstration and testing via thonny. GPS Data will be stored permanently on the SD-Card, and is also available as text output in the Thonny shell. 

(note: tests are not necessarily carried out with the scripts in this repo, those are just examples).

Schematic Micro SD-Card board:

[https://files2.elv.com/public/13/1315/131591/Internet/131591_msda1_schaltplan.pdf](https://media.elv.com/file/131591_msda1.pdf)

![344368826-2cae6269-d276-4e12-a081-d1e77e937b67](https://github.com/user-attachments/assets/c9d366d2-c749-4118-a355-9b1bc5232a29)

GPS data generated on a mountain in Bavaria, Pico and GPS module get supplied by a power bank.

Script is saved as "main.py" in the Pico file system, so it starts automatically.
