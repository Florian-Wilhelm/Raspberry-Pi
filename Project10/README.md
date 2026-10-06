# Raspberry Pi Pico and Pico-GPS-L76B GNSS module

### Software

File "Pico-GPS-L76B_Code2.zip" is copied from the waveshare wiki:

https://www.waveshare.com/wiki/Pico-GPS-L76B

Python scripts in this repo are modified versions of scripts in that .zip. Script is stored as "main.py" in the Pico file system, so it starts automatically.

### Hardware

There is no trouble putting the GPS module onto the standard Raspberry Pi Pico header. Push button, SD-Card board and 0.96'' OLED are off-the-shelf components for an example arrangement.

Schematic Micro SD-Card board:

[https://files2.elv.com/public/13/1315/131591/Internet/131591_msda1_schaltplan.pdf](https://media.elv.com/file/131591_msda1.pdf)

### Example arrangement and demo

GPS Data will be stored permanently on the SD-Card. It is also available as text output in the Thonny shell, as well as visible on the OLED display. Arrangement can be used stand-alone, then you have of course just the visual output on the display.

The GPS data on the second photo is generated while standing next to the Tabarettahütte (Ortlergebirge, South Tyrol), Pico and GPS module get supplied by a power bank (on the first photo you do not see much on the OLED display because there is this common problem as to the screen refresh rates and camera shutter speed).

Note: this project is purely experimental, so my hardware arrangements are always slightly different (and so are the scripts I'm using).

<img width="900" height="600" alt="Pico-GPS-L76B--config" src="https://github.com/user-attachments/assets/53c13a1d-342b-4fbb-ad9e-235637d50a13" />

<img width="900" height="506" alt="20260730_Tabarettahuette-GPS" src="https://github.com/user-attachments/assets/ea08d78b-b2c4-48e8-86d7-f09f837ae63f" />


