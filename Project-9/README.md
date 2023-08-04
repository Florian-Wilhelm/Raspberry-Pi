## Current measurement 

### clk_sys 12 MHz

Supply via Raspberry Pi Pin 2 (+5V). Current of the LCD (which is about 20 mA) goes into the measurement as well.

![CurrentMeasurement-Pico](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/6b3b72e1-c765-49df-a89c-3788803f1686)

### Varying clk_sys

As you can see, there is a linear dependency between the MCU current and clk_sys ("dynamic power consumption"). About 6 mA is the "static power consumption".

![Current--CLK_SYS](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/ec2af30f-42b6-4dd7-bc87-e586dad7b960)
