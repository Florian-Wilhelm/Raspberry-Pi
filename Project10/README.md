File "Pico-GPS-L76B_Code2.zip" is copied from the waveshare wiki:

https://www.waveshare.com/wiki/Pico-GPS-L76B

There is no trouble putting the GPS module onto the standard Raspberry Pi Pico header.

## Example Configuration

### Push button, Micro SD-Card and OLED SSD1306

Demonstration and testing via thonny. GPS Data is stored permanently on the SD-Card, and is also available as text output in the Thonny shell. 

Schematic Micro SD-Card board:

https://files2.elv.com/public/13/1315/131591/Internet/131591_msda1_schaltplan.pdf

![Pico-GPS-L76B--config](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/2cae6269-d276-4e12-a081-d1e77e937b67)

### Push button and OLED SSD1306

GPS data generated on a mountain in Bavaria, Pico and GPS module get supplied by a power bank.

Note 1: script is saved as "main.py", so it starts automatically.

Note 2: black bars only appear on the photos, not on the OLED itself; DSLR and mobile phone cam have both CMOS sensors which have a problem with the OLED resfresh rates obviously (especially in bright daylight).

![Grosser-Traithen](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/63c44ae1-4846-423e-bff4-a84508238fd0)
