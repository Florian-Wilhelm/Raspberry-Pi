# Air humidity (RH) measurement

## Description

An IoT device comprising a Raspberry Pi Pico, three sensors (including the HIH4020 air humidity sensor) and the ESP8266. Note: no SSL/TLS is or will be implemented (i.e. certificates), so you'll get only a HTTP connection, thus code is only meant for experimental use.

## Test Software

SW V2.00 -> below an example web browser output (note: WD means "Watchdog")  

![Screenshot_20260221_082506_Chrome](https://github.com/user-attachments/assets/9e813d88-88c5-4820-94d9-8a30f8fa005e)

You can hardcode them, but with SW V2.00 it is possible to modify the WiFi credentials via Picotool (the .uf2 file has to be located in the respective directory):

*$ picotool config HIH4020-humidityMeasurement.uf2 -s SSID "my WiFi"*

*$ picotool config HIH4020-humidityMeasurement.uf2 -s PASS "001122334455"*

## Hardware

An ad-hoc arrangement for experimental use.

![Konfiguration--HIH4020-103AT2-SFH300--edited](https://github.com/user-attachments/assets/aa5d2b48-5d35-4ad3-a9d1-a353af97cdb0)
