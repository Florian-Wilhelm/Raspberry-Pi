### Description

A fifty-year old Digital-Analog-Converter (DAC) design with two CD4007 CMOS ICs, but still good enough to get a grasp how these devices work.

The output (which is inverted) should act according to the input bit pattern.

Examples with 3.3V (CD4007UBE specified for 3V-18V):
(msb->lsb)
111111 (representing 0d) = 0.0V
011111 (representing 2^5 = 32d) = 1.6V
001111 (representing 2^5 + 2^4 = 48d) = 2.5V

The better the tolerances of the resistors (you need a lot :-D), the better the results. 
A fifty-year old Digital-Analog-Converter (DAC) design with two CD4007 CMOS ICs, but still good enough to get a grasp how these devices work. 

The output (which is inverted) should act according to the input bit pattern.

Examples with 3.3V (CD4007UBE specified for 3V-18V):
(msb->lsb)
111111 (representing 0d) = 0.0V
011111 (representing 2^5 = 32d) = 1.6V
001111 (representing 2^5 + 2^4 = 48d) = 2.5V

The better the tolerances of the resistors (you need a lot :-D), the better the results. 
Input can be provided by e.g. a MCP23016 or MCP23017 I/O expansion. Scaling the device to 9bit, 12bit, and so forth would not be a big deal.

![DAC-MCP23016](https://github.com/Florian-Wilhelm/Raspberry-Pi/assets/77980708/25328ac9-d363-49ba-abef-9791dcfebde6)
