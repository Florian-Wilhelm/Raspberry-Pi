# Air humidity (RH) measurement

## Description

An IoT device comprising a Raspberry Pi Pico, three sensors and the ESP8266. Note: no SSL/TLS is or will be implemented (i.e. certificates), so you'll get only a HTTP connection, thus code is only meant for experimental use.

### Software

Below an example web browser output. 

With SW V2.00, it is possible to modify the WiFi credentials via Picotool:

*$ picotool config HIH4020-humidityMeasurement.uf2 -s SSID "my WiFi"*

*$ picotool config HIH4020-humidityMeasurement.uf2 -s PASS "001122334455"*

![Webserver-IoT](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/fcef041a-d983-4783-be21-395024e1c47a)

Note: WD - "Watchdog".

### Hardware

![Konfiguration--HIH4020-103AT2-SFH300--edited](https://github.com/user-attachments/assets/aa5d2b48-5d35-4ad3-a9d1-a353af97cdb0)
