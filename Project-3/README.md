# Simple weather station

## Description

This is just a simple weather station with off-the-shelf components, consisting essentially of a BMP280 temperature-and-pressure-sensor and an LCD (Liquid Crystal Display).

Either component is connected to the I2C bus, i.e. Pin 2 (SDA) and 3 (SCL) of a Raspberry Pi.

Weather data is published via MQTT, and/or displayed on the LCD.

![LCD-assembly--scaled](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/a83b0d83-194f-4272-b989-b6af7ab5d724)
