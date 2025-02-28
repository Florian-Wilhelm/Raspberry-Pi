# Exploring the RP2040 clocks

See RP2040 datasheet chapter 2.15., 2.16., 2.17., and the SDK documentation chapter 4.1.4.

## 1. Measuring the current 

### 1.1 Example: System Clock (clk_sys) 12 MHz

Supply thru Raspberry Pi Pin 2 (+5V). LCD connected for convenience, the LCD current (which is about 20 mA) goes into the measurement too.

![Current](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/4b91a263-de49-49b2-8b87-8083b41b6474)

### 1.2 Varying clk_sys

There is a linear dependency between the MCU current and clk_sys ("dynamic power consumption"). 

About 5 mA is the "static power consumption", which is always present and to a great deal due to FET leakage currents within the MCU (or so says the theory).

![Current-over-clk_sys](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/1136ca16-1971-4c13-b5f3-f4026da0bb4b)

## 2. External clock 

### 2.1 Crystal oscillator module

Desoldering R14, C16 and C17, and soldering the oscillator module OUT Pin to Pico XIN (here used rather at random a 14.31 MHz crystal oscillator module).

![XOSC-Pico-schematic](https://github.com/user-attachments/assets/e26b8412-464c-45ad-a165-2a458a1e6717)

External oscillator module consumes a few mA (about 1.8 mA). 
Time bases are off now, so the SW ought to be modified. Hence values on the LCD are not correct, the implemented seconds counter (the value on the right) counts too fast (i. e. 14.31/12.00 too fast).

![Current-external-XOSC-14-31MHz](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/988c4feb-8c54-445c-bd63-ad66318ac39c)

