# Pico and Pico-GPS-L76B GNSS module

#### Software:

File "Pico-GPS-L76B_Code2.zip" is copied from the waveshare wiki:

https://www.waveshare.com/wiki/Pico-GPS-L76B

Python scripts in this repo are slightly modified versions of scripts in that .zip.  

#### Hardware:

There is no trouble putting the GPS module onto the standard Raspberry Pi Pico header. Button, SD-Card board and OLED are off-the-shelf components.

## Example Configuration

### Push button, Micro SD-Card and OLED SSD1306

(Note: not necessarily carried out with the scripts in this repo, those are just examples).

Demonstration and testing via thonny. GPS Data will be stored permanently on the SD-Card, and is also available as text output in the Thonny shell. 

Schematic Micro SD-Card board:

[https://files2.elv.com/public/13/1315/131591/Internet/131591_msda1_schaltplan.pdf](https://media.elv.com/file/131591_msda1.pdf)

![Pico-GPS-L76B--config](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/2cae6269-d276-4e12-a081-d1e77e937b67)

GPS data generated on a mountain in Bavaria, Pico and GPS module get supplied by a power bank.

**Note 1:** script is saved as "main.py" in the Pico file system, so it starts automatically.

**Note 2:** black bars only appear on the photos, not on the OLED itself; DSLR and mobile phone cam have both CMOS sensors which have a problem with the OLED resfresh rates obviously (especially in bright daylight).

![Grosser-Traithen](https://github.com/user-attachments/assets/3602c53a-52ee-4d6e-8dd3-2c80f50a1b9f)
