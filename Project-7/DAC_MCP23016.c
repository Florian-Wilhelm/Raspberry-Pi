/**
 * Created 03/01/2023 by Florian Wilhelm Dirnberger
 *
 * 6bit DAC connected to MCP23016 I/O expansion for I2C bus (GP0 in this case)
 * realized here is a simple saw tooth generator
 *  
 * In the data sheet for the MCP23016 IC, all relevant information regarding the registers can be found  
 * IC is hardwired to I2C address 0x20 (A0, A1, A2 to GND)
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h" 
#include "pico/time.h"
#include "hardware/gpio.h"

int main()
{       
  
  stdio_init_all();
  
  sleep_ms(1000);

  i2c_init(i2c1, 400*1000);

  gpio_set_function(2, GPIO_FUNC_I2C);
  gpio_set_function(3, GPIO_FUNC_I2C);

  // using two external 3.9kOhm resistors, so the following two lines are obsolete
  /* gpio_pull_up(2);
     gpio_pull_up(3); */
  
  sleep_ms(200);
  
  uint8_t buf[2];
  
  buf[0] = 0x06; // I/O direction register
  buf[1] = 0x00; // GP0 is output
  
  i2c_write_blocking(i2c1, 0x20, buf, 2, false);   
    
  sleep_ms(200);
  
  int sawtooth;
  
  while(true){
         
   for (sawtooth = 0x3F; sawtooth>=0x00; sawtooth--) // only a 6bit DAC, so we count down from 0011111 which is 0x3F (inverted input for the DAC)
    {
  
    buf[0] = 0x00; // data port register
    buf[1] = sawtooth; // modified data output
     
    i2c_write_blocking(i2c1, 0x20, buf, 2, false);  
  
    sleep_us(100); // will manipulate the sawtooth frequency, together with the baud rate
    
    }  
  }         
}