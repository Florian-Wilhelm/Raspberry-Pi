/**
 * Created 06/07/2022 by Florian Wilhelm Dirnberger
 *
 * Purely a demonstration code how to write several Raspberry Pi Pico GPIO ports at once by using the Single-cycle IO block (SIO)
 * See figure 2 and chapter 2.3.1 in the RP2040 datasheet
 * "The SIO is connected to the single-cycle IOPORT bus of each processor, and provides GPIO access, two-way communications, and other core-local peripherals".
 *
 * Program counts from 0 to 9 and starts over again, digits are displayed on a 7-Segment-Display (e.g. SC56-11GWA)
 * necessary additional electronic components to see something happen are one CD4511BE BCD-to-7-Segment Latch Decoder Integrated Circuit and seven resistors.
 *
 *  1267   Pins of CD4511BE (you can easily google the datasheet)
 *  BCDA   BCD (Binary Coded Decimal) input for CD4511BE
 *  2345   used GPIO Pins of the Pico
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

// # Time in ms for displaying one digit before switching to the next
uint32_t displayTime = 1000;

int main()
{
       // setting GPIOs as output
       gpio_init(2);
       gpio_set_dir(2, GPIO_OUT);       
       gpio_init(3);
       gpio_set_dir(3, GPIO_OUT);       
       gpio_init(4);
       gpio_set_dir(4, GPIO_OUT);       
       gpio_init(5);
       gpio_set_dir(5, GPIO_OUT);
       
       uint32_t *SIO = (uint32_t *)0xd0000000; // on address 0xd0000000 (that equals the SIO_BASE in the SDK) there is a uint32_t variable
       while(true)
       {
           // note 1: for we work with an integer value uint32_t (4 byte) we have to divide by 4 (pointer arithmetic)
           // note 2: left shifting "<< 2" because we start on GPIO 2
           // additional note: explicit calculation of pointer values is not compliant with MISRA-C
              
           *(SIO + 0x014 / 4) = 0ul << 2; // 0x014 is GPIO_OUT_SET
           sleep_ms(displayTime);
           *(SIO + 0x018 / 4) = 0ul << 2; // 0x018 is GPIO_OUT_CLEAR 
           
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
