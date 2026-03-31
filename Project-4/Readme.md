# Pressure and Temperature Measurement

## Description

Barometric pressure and ambient temperature measurement with the ubiquitous BMP280 sensor (here in the form of a breakout board). 

Values can be used to calculate the current altitude above mean sea level, but depending on what formulas you are using the result can be very inaccurate for the lack of knowledge about real conditions on sea level, the underlying physics are complicated.

## Test Code

Data is provided on an OLED display and optionally on the serial port.

## Hardware

I've created the PCB for the burglar detection project originally, but it is a generic design for I2C projects. The extra Pull-Up resistors you see on the photo are not wired. 

![20260323_124544](https://github.com/user-attachments/assets/b176fd69-7eac-4c25-b1f6-7e76dabb88bb)

![20260323_181942](https://github.com/user-attachments/assets/c870c289-847d-4764-9f57-6e861f141076)
