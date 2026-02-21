# Soil moisture measurement device

## Description

Soil moisture measurement device showing a double-digit value on two 7-Segment-Displays. A probe (either resistive or capacitive type) delivers an analog value (i. e. voltage) representing the soil moisture, the RP2040 ADC converts the sensor signal into a 16-bit (actually a 12-bit) digital value, this value is then mapped to 0% - 99% soil moisture by means of a simple algorithm (linearity assumed).

## Test Software

"MoistureMeasurement-CSens-RSens-OLED.py", to be saved as "main.py" to run automatically. Sensor type (capacitive or resistive) is detected automatically.

## Hardware

Custom made PCB, for details see hackaday.io project page.

![IMG_4593](https://github.com/user-attachments/assets/03f5d652-503a-43cc-9c1c-243ff662a956)
