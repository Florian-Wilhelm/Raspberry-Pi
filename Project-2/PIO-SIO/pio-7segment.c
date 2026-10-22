/**
 * Created 22/08/2022 by Florian Wilhelm Dirnberger, modified 22/10/2026
 *
 * Purely a demonstration code how to write several Raspberry Pi Pico GPIO ports at once by means of the:   
 *  A. Programmable Input/Output (PIO)
 *  B. Single-cycle IO block (SIO)
 * 
 * See the RP2040 datasheet for more details.
 *
 * Program counts from 0 to 9 and starts over again, digits are displayed on two 7-Segment-Display (e.g. BC04-11SEKWA is usable);
 * necessary additional electronic components to see something happen are two CD4511BE BCD-to-7-Segment Latch Decoder, and fourteen resistors (no multiplexing implemented).
 *
 *  Mapping:
 *  1267   Pins of either CD4511BE (you can easily google the datasheet)
 *  BCDA   BCD (Binary Coded Decimal) input for CD4511BE
 *  2345   used GPIO Pins of the Pico (PIO Output)
 *  6789   used GPIO Pins of the Pico (SIO Output)
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
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/gpio.h"

#include "ABCD.pio.h"

void initUART()
{
  uart_init(uart0, 115200);
  gpio_set_function(0, GPIO_FUNC_UART);
  gpio_set_function(1, GPIO_FUNC_UART);
  uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
  // shell command: "minicom -b 115200 -D /dev/serial0"
}  

int main()
{
	stdio_init_all();
	
	uint32_t displayTime = 1000;
	
	int n;
	char UART_Output[64]; // pay attention to the correct size of the array, undefined behaviour may ensue otherwise
	
	uint ZeroToNineBCD[10] = {0x0, 0x8, 0x1, 0x9, 0x2, 0xA, 0x3, 0xB, 0x4, 0xC}; // does not matter whether written down as hex or as unsigned long
		
    sprintf(UART_Output, "Controlling 7-Segment-Displays via the PIO and SIO\r\n");  
    uart_write_blocking(uart0, UART_Output, strlen(UART_Output)); 
	
	// PIO	
	uint offset = pio_add_program(pio1, &ABCD_program);
		
	uint sm = pio_claim_unused_sm(pio1, true);
	pio_sm_config c = ABCD_program_get_default_config(offset);	
	sm_config_set_set_pins(&c, 2, 4);
	sm_config_set_out_pins(&c, 2, 4);	
	pio_gpio_init(pio1, 2);
	pio_gpio_init(pio1, 3);
	pio_gpio_init(pio1, 4);
	pio_gpio_init(pio1, 5);
	
	//sm_config_set_clkdiv_int_frac(&c, 255, 0);	
	pio_sm_init(pio1, sm, offset, &c);	
	pio_sm_set_enabled(pio1, sm, true);
	
	// SIO 	
	uint32_t *SIO = (uint32_t *)0xd0000000; // on address 0xd0000000 (that equals the SIO_BASE in the SDK) there is a uint32_t variable

	void sio_output(int BCD)
	{    
		*(SIO + 0x014 / 4) = BCD << 6; // 0x014 is GPIO_OUT_SET        
	}
	void sio_clear_output(int BCD)
	{    
		*(SIO + 0x018 / 4) = BCD << 6; // 0x018 is GPIO_OUT_CLEAR   
	}
	
	// setting GPIOs as output
    gpio_init(6);
    gpio_set_dir(6, GPIO_OUT);       
    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);       
    gpio_init(8);
    gpio_set_dir(8, GPIO_OUT);       
    gpio_init(9);
    gpio_set_dir(9, GPIO_OUT);  
	
	// DISPLAYING
	while(true)
	   {		 
		 for (n=0; n<10; n++)
			{
	        pio_sm_put_blocking(pio1, sm, ZeroToNineBCD[n]);
	        sio_output(ZeroToNineBCD[n]);
	        	        
	        sleep_ms(displayTime);
	        sio_clear_output(ZeroToNineBCD[n]); 	        
			}
       }
	return 0;
}
