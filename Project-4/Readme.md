# Pressure and Temperature Measurement

## Description

Barometric pressure and ambient temperature measurement with the ubiquitous BMP280 sensor (here in the shape of a breakout board, manufacturer "joy-it"). 

## Test Code

Data is provided on an OLED display and optionally on the serial port.

## Hardware

I've created the PCB for the burglar detection project originally, but it is a generic design for I2C projects. The extra 3.9k Pull-Up resistors you see on the photo are not wired. 

![20260323_124544](https://github.com/user-attachments/assets/b176fd69-7eac-4c25-b1f6-7e76dabb88bb)

![20260323_181942](https://github.com/user-attachments/assets/c870c289-847d-4764-9f57-6e861f141076)

Values can be used for an ad-hoc calculation of the current altitude above mean sea level (when you are moving about in the outdoors for instance), yet the result will be very inaccurate for the lack of knowledge about "real" conditions on sea level. The underlying physics are pretty involved, and discussing the formulas (see source code) is beyond the scope of this project.

In a fixed weather station where the actual altitude above mean sea level is known, the pressure on sea level can be calculated thru the barometric formula. 

Comparing calculated data with actual GPS data (as to the GPS module, see project 10):

<img width="800" height="480" alt="GPS-BMP280-Pico" src="https://github.com/user-attachments/assets/2776844a-04a1-4d13-8f5a-6a3e884189be" />
