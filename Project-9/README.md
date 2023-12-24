## 1. Measuring the current 

### 1.1 Example: System Clock (clk_sys) 12 MHz

Supply thru Raspberry Pi Pin 2 (+5V). Current of the LCD (which is about 20 mA) goes into the measurement as well.

![Current](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/4b91a263-de49-49b2-8b87-8083b41b6474)

### 1.2 Varying clk_sys

There is a linear dependency between the MCU current and clk_sys ("dynamic power consumption"). 

About 6 mA is the "static power consumption", which is always present and mostly due to FET leakage currents.

![Current-over-clk_sys](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/1136ca16-1971-4c13-b5f3-f4026da0bb4b)

## 2. External clock 

### 2.1 Crystal oscillator module

Desoldering R15, C16 and C17 and soldering the oscillator module OUT Pin to Pico XIN (here used rather at random: 14.31 MHz crystal oscillator module)

C16 and C17 desoldered, the white dot on the PCB marks XIN: 

![C16-C17-desoldered](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/5ee4b716-3b39-4c45-8445-ca2fb8c15b44)

External oscillator module consumes a few mA, so we end up also with about 30 mA though clk_sys is much lower now. 
Time bases are off now (i. e. 14.31/12.00 off), so the SW ought to be modified. Hence values on the LCD are not correct.

![Messaufbau-external-XOSC](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/34edceb0-7f22-4186-aea1-b09cffbaf87f)
