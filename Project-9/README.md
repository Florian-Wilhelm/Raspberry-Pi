## Current measurement 

### clk_sys 12 MHz

Supply via Raspberry Pi Pin 2 (+5V). Current of the LCD (which is about 20 mA) goes into the measurement as well here.

![Messaufbau](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/cfaaa8ab-902c-4408-befd-30ed284436bb)

### Varying clk_sys

As you can see, there is a linear dependency between the MCU current and clk_sys ("dynamic power consumption"). About 6 mA is the "static power consumption".

![Current--CLK_SYS](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/ec2af30f-42b6-4dd7-bc87-e586dad7b960)
