# Simple weather station

## Description

This is just a simple weather station with off-the-shelf components, consisting essentially of a BMP280 temperature-and-pressure-sensor and an LCD (Liquid Crystal Display).

Either component is connected to the I2C bus, i.e. Pin 2 (SDA) and 3 (SCL) of a Raspberry Pi.

Weather data is published via MQTT, and/or displayed on the LCD.

![LCD-assembly](https://github.com/user-attachments/assets/52c9048b-37ad-4093-b30d-df6ea92e4f77)
