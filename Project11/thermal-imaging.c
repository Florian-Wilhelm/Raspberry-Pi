/**
 * 
 * Test code as of September 16 2025, not yet to be used
 * 
 * MLX90642 IR array serves as main sensor
 * TSL25911 ambient light sensor is just an additional component to stress the I2C a little 
 * 
 * OLED for thermal display, but before this works sufficiently data output on the serial port
 *  
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
//#include "hardware/clocks.h"
//#include "hardware/structs/clocks.h"

#include "connections.h"

//#include "oled.h"
//#include "lookupTable.h"

void initUART()
{
  uart_init(uart0, 9600);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
}  

void initI2C()
{  
  i2c_init(I2C_ID, 100 * 1000);    
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  
  // using external 3.9 kOhm resistors, internal not needed
  //gpio_pull_up(SDA_PIN);
  //gpio_pull_up(SCL_PIN);   
}

int main() 
{  
    stdio_init_all();
    initUART();
    initI2C();
    
    // uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))*1000;  
           
    char readValue[32];
    
    /*
    Initialize the MLX90642 sensor
    */ 
     
    // reading Firmware (FW) versions, Output on the serial port
    uint8_t FW_major[4] = {0xFF, 0xF8};
    uint8_t FW_minor[4] = {0xFF, 0xFA};
    
    i2c_write_blocking(I2C_ID, 0x66, FW_major, 2, true);      
    i2c_read_blocking(I2C_ID, 0x66, FW_major, 2, false);
    
    i2c_write_blocking(I2C_ID, 0x66, FW_minor, 2, true);      
    i2c_read_blocking(I2C_ID, 0x66, FW_minor, 2, false);    
        
    sprintf(readValue, "MLX90642 FW Version %d.%d.%d\r\n", FW_major[0], FW_minor[1], FW_minor[0]);  
    uart_write_blocking(UART_ID, readValue, strlen(readValue));      
           
    /*
     Initialize the TSL25911 sensor
     */
    
    // see data sheet, it is a bit tricky with the registers  
     
    uint8_t TSL25911_init_0[] = {0xA0};  // COMMAND register, normal operation, adressing ENABLE register   
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_0, 1, true);  
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_0, 1, false) < 0)  // repeats until the slave device sends ACK
    {
      sleep_ms(1);
    }
  
    uint8_t TSL25911_init_1[] = {0x03}; // ENABLE register, PON and AEN 
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_1, 1, true);
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_1, 1, false) < 0)  
    {
      sleep_ms(1);
    }   
  
    uint8_t TSL25911_init_2[] = {0xA1};  // COMMAND register, normal operation, adressing CONFIG/CONTROL register 
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_2, 1, true); 
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_2, 1, false) < 0)  
    {
      sleep_ms(1);
    }
  
    uint8_t TSL25911_init_3[] = {0xB4};  //  CONFIG/CONTROL register, 0xB4 -> maximum gain mode and integration time 500ms
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_3, 1, true);  
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_3, 1, false) < 0)  
    {
      sleep_ms(1);
    } 
    
    sleep_ms(100); 
    
    while(true){
      
      // test I2C commands for MLX90642 
       
      // *the array consists of 768 IR sensors
      // *each IR sensor delivers 2 Byte of data
      // *image data starts at address 0x342C (Pixel [1:1]), and ends at address 3A2A (Pixel [24:32]), so there are 1534 Bytes RAM for image data      
      
      uint8_t pixel_1_1[4] = {0x34, 0x2C};
    
      i2c_write_blocking(I2C_ID, 0x66, pixel_1_1, 2, true);      
      i2c_read_blocking(I2C_ID, 0x66, pixel_1_1, 2, false);      
    
      sprintf(readValue, "MLX90642 Pixel [1:1] %x %x \r\n", pixel_1_1[0], pixel_1_1[1]);  
      uart_write_blocking(UART_ID, readValue, strlen(readValue));
      
      uint8_t pixel_24_32[4] = {0x3A, 0x2A};
    
      i2c_write_blocking(I2C_ID, 0x66, pixel_24_32, 2, true);      
      i2c_read_blocking(I2C_ID, 0x66, pixel_24_32, 2, false);      
    
      sprintf(readValue, "MLX90642 Pixel [24:32] %x %x \r\n", pixel_24_32[0], pixel_24_32[1]);  
      uart_write_blocking(UART_ID, readValue, strlen(readValue));  
       
          
      // test I2C commands for TSL25911
      uint8_t TSL25911_LB_CH0[] = {0xB4};    
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_LB_CH0, 1, false);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_LB_CH0, 1, false);
      
      uint8_t TSL25911_HB_CH0[] = {0xB5};
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_HB_CH0, 1, false);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_HB_CH0, 1, false);   
    
      sprintf(readValue, "TSL25911 CH0: %x %x \r\n", TSL25911_HB_CH0[0], TSL25911_LB_CH0[0]);  
      uart_write_blocking(UART_ID, readValue, strlen(readValue));    
         
      sleep_ms(2000);    
      
   }
          
    return 0;       
}
