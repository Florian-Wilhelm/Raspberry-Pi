# Air humidity (RH) measurement

## Description

An IoT device comprising a Raspberry Pi Pico, three sensors (including the HIH4020 air humidity sensor), additional passive electronic components and the ESP8266 for WiFi. 

## Test Software

SW V2.XX -> below an example web browser output (WD means "Watchdog"). 

Port forwarding within the Local Area Network is not necessary; if you wanted to access this particular device from the internet, the router would have to forward port 78 and you would need to know the router's public IP address.  

Since no SSL/TLS is or will be implemented (connection is HTTP not HTTPS), I would advise against making such an IoT device accessible to the wider world.

![Screenshot_20260221_082506_Chrome](https://github.com/user-attachments/assets/9e813d88-88c5-4820-94d9-8a30f8fa005e)

You can hardcode the WiFi credentials in your own build, but it is also possible to modify them in the here uploaded .uf2-file via Picotool. Download the file to a respective directory, change to that directory (or use absolute paths) on the shell via cd command, then type the following:

*$ picotool config HIH4020-humidityMeasurement.uf2 -s SSID "WiFi-123"*

*$ picotool config HIH4020-humidityMeasurement.uf2 -s PASS "001122334455"*

The .uf2-file can also be accessed in the device's BOOTSEL mode (then omit the filename when typing the commands). The downside with this method is that the WiFi credentials are now plaintext in the .uf2-file and readable in a HEX-Editor.

![plaintext](https://github.com/user-attachments/assets/3e5d2040-7ced-4214-88fd-daaddf9500a5)

With all that being said, you'll probably see that the given code example is only meant and good enough for experimental use.

## Hardware

An ad-hoc arrangement. Wiring sensors to ADC1-ADC3 see source code. RH measurement cannot be very accurate, for a multitude of reasons (resistor tolerances, fluctuating supply voltage, inadequate ADCs, inaccurate temperature compensation).

Wiring ESP8266 to Pico (all other pins not connected):

* Pin 1 - GND

* Pin 2 (TXO) - GP5 (UART1 RX)

* Pin 4 - 3.3V

* Pin 7 (RXI) - GP4 (UART1 TX)

* Pin 8 - 3.3V

![Konfiguration--HIH4020-103AT2-SFH300--edited](https://github.com/user-attachments/assets/311992f0-0284-49af-97a9-ccfbcf264207)
