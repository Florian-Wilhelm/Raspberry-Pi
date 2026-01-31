# Exploring the RP2040 clocks

See RP2040 datasheet chapter 2.15., 2.16., 2.17., and the SDK documentation chapter 4.1.4.

## 1. Measuring the MCU current 

There is a linear dependency between the MCU current and the system clock (abbreviated clk_sys), so called "dynamic power consumption". 

About 5 mA or less is the "static power consumption", which is always present and to a great deal due to FET leakage currents within the MCU (so say the literature sources). There are other causes for offsets, so take the graph below with a grain of salt.

![Current-over-clk_sys](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/1136ca16-1971-4c13-b5f3-f4026da0bb4b)

## 2. Soldering an external clock source

Desoldering R14, C16 and C17, and soldering a crystal oscillator module OUT Pin to Pico XIN (for my demo video used rather at random a 14.31 MHz crystal oscillator module). 

XOUT is not needed, but of course you need a common ground.

![XOSC-Pico-schematic](https://github.com/user-attachments/assets/e26b8412-464c-45ad-a165-2a458a1e6717)
