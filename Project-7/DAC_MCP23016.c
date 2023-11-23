/**
 * Created 03/01/2023 by Florian Wilhelm Dirnberger
 *
 * 6bit R-2R ladder DAC hardwired to the MCP23016 digital output Pins GP0.x (this IC is an I/O expansion for I2C bus) 
 * MCP23016 is in turn connected to one of the two RP2040 I2C controllers (i.e. I2C master) for receiving bus data
 * 
 * Realized with this program is a simple saw tooth generator
 *  
 * In the data sheet for the MCP23016 IC, all relevant information regarding the registers can be found  
 * Hardwired is I2C address 0x20 (A0, A1, A2 to GND)
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
  
  sleep_ms(100);

  i2c_init(i2c1, 50*1000); // second parameter: I2C frequency; for example, if it's 1000kHz the duration for one SCL cycle is 1ms

  gpio_set_function(2, GPIO_FUNC_I2C); // SDA
  gpio_set_function(3, GPIO_FUNC_I2C); // SCL

  // using two external 3.9kOhm resistors, so the following two lines are obsolete; with the RP2040 internal ones, it probably won't work at all
  /* gpio_pull_up(2);
     gpio_pull_up(3); */
  
  sleep_ms(200);
  
  uint8_t buf[2];
  
  buf[0] = 0x06; // I/O direction register
  buf[1] = 0x00; // GP0 is output
  
  i2c_write_blocking(i2c1, 0x20, buf, 2, false);   // last two parameters: buffer length = 2 bytes; nostop = false, that means final stop bit is sent
    
  sleep_ms(200);
  
  int sawtooth;
  
  while(true){
         
   for (sawtooth = 0x3F; sawtooth>=0x00; sawtooth--) // a 6bit DAC, so we count down from 00111111 which is 0x3F, or 63d (inverted input for the DAC)
    {
  
    buf[0] = 0x00; // data port register
    buf[1] = sawtooth; // modified data output
     
    i2c_write_blocking(i2c1, 0x20, buf, 2, false);  
  
    //sleep_us(1000); // a value here would manipulate the sawtooth frequency, along with the I2C frequency
    
    }  
  }         
}
