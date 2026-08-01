# Raspberry Pi Pico and Pico-GPS-L76B GNSS module

### Software:

File "Pico-GPS-L76B_Code2.zip" is copied from the waveshare wiki:

https://www.waveshare.com/wiki/Pico-GPS-L76B

Python scripts in this repo are slightly modified versions of scripts in that .zip. Script is stored as "main.py" in the Pico file system, so it starts automatically.

### Hardware:

There is no trouble putting the GPS module onto the standard Raspberry Pi Pico header. Push button, SD-Card board and 0.96'' OLED are off-the-shelf components for an example arrangement.

Schematic Micro SD-Card board:

[https://files2.elv.com/public/13/1315/131591/Internet/131591_msda1_schaltplan.pdf](https://media.elv.com/file/131591_msda1.pdf)

### Example arrangement and demo

GPS Data will be stored permanently on the SD-Card, and is also available as text output in the Thonny shell as well as visible on the OLED display. Arrangement can be used stand-alone, then you have just the output on the display obviously.

The GPS data on the second photo is generated while standing next to the Tabarettahütte (Ortlergebirge, South Tyrol), Pico and GPS module get supplied by a power bank.

Note: this project is purely experimental, so my test arrangements are always slightly different.

![344368826-2cae6269-d276-4e12-a081-d1e77e937b67](https://github.com/user-attachments/assets/c9d366d2-c749-4118-a355-9b1bc5232a29)

<img width="900" height="506" alt="20260730_Tabarettahuette-GPS" src="https://github.com/user-attachments/assets/ea08d78b-b2c4-48e8-86d7-f09f837ae63f" />


