/**
 * Demonstration how to write several Raspberry Pi Pico GPIO ports at once by using the Single-cycle IO block (SIO);
 * See figure 2 and chapter 2.3.1 in the RP2040 datasheet;
 * "The SIO is connected to the single-cycle IOPORT bus of each processor, and provides GPIO access, two-way communications, and other core-local peripherals".
 *
 *  1267   Pins of CD4511BE
 *  BCDA   BCD (Binary Coded Decimal) input for CD4511BE
 *  2345   GPIO Number
 * 
 *  0000   0ul  ->0
 *  0001   8ul  ->1
 *  1000   1ul  ->2
 *  1001   9ul  ->3
 *  0100   2ul  ->4
 *  0101   10ul ->5
 *  1100   3ul  ->6
 *  1101   11ul ->7
 *  0010   4ul  ->8
 *  0011   12ul ->9
 * 
 * Important note: Reading from right (MSB) to left (LSB) to formulate the decimal unsigned long (ul) value by virtue of the binary code
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// # Time for displaying one digit before switching to the next
uint32_t displayTime = 1000;

int main()
{
       // Using a CD4511BE BCD-to-7-Segment Latch Decoder, setting GPIOs as output
       gpio_init(2);
       gpio_set_dir(2, GPIO_OUT);
       
       gpio_init(3);
       gpio_set_dir(3, GPIO_OUT);
       
       gpio_init(4);
       gpio_set_dir(4, GPIO_OUT);
       
       gpio_init(5);
       gpio_set_dir(5, GPIO_OUT);
       
       uint32_t *SIO = (uint32_t *)0xd0000000;
       while(true)
       {
           *(SIO + 0x014 / 4) = 0ul << 2; // GPIO_OUT_SET
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 0ul << 2; // GPIO_OUT_CLEAR          
           
           *(SIO + 0x014 / 4) = 8ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 8ul << 2;           
           
           *(SIO + 0x014 / 4) = 1ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 1ul << 2;           
           
           *(SIO + 0x014 / 4) = 9ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 9ul << 2;           
           
           *(SIO + 0x014 / 4) = 2ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 2ul << 2;
           
           *(SIO + 0x014 / 4) = 10ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 10ul << 2;           
           
           *(SIO + 0x014 / 4) = 3ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 3ul << 2;           
           
           *(SIO + 0x014 / 4) = 11ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 11ul << 2;           
           
           *(SIO + 0x014 / 4) = 4ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 4ul << 2;           
           
           *(SIO + 0x014 / 4) = 12ul << 2;
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 12ul << 2;
           
       }
   }
