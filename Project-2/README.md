# Soil moisture measurement device

## Description

Soil moisture measurement device showing a double-digit value on two 7-Segment-Displays. A probe (either resistive or capacitive type) delivers an analog value (i. e. voltage) representing the soil moisture, the RP2040 ADC converts the sensor signal into a 16-bit (actually a 12-bit) digital value, this value is then mapped to 0% - 99% soil moisture by means of a simple algorithm (linearity assumed).

## Test Software

"MoistureMeasurement-CSens-RSens-OLED.py", to be saved as "main.py" in order to run once the supply voltage is connected.
Sensor type (capacitive or resistive) is detected automatically.

If you are looking for a C-Code or an IoT example, please check out the Project-5 folder.

For knowing how to write several GPIO ports at once elegantly, please check out the PIO-SIO folder.

## Hardware

Custom made PCB (for details see hackaday.io project page):

![IMG_4593](https://github.com/user-attachments/assets/03f5d652-503a-43cc-9c1c-243ff662a956)

With the PIO/SIO Code running on a prototype board arrangement:

<img width="800" height="520" alt="IMG_0473" src="https://github.com/user-attachments/assets/5313bb80-49e3-4323-ad41-7d2e5cae94a9" />
