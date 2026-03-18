/**
 * 
 * TSL25911 ambient light sensor test code as of March 18, 2026
 * 
 * raw ambient light data output on an OLED display (High Byte and Low Byte of Channel 1 and Channel 0), and on the serial port
 * see data sheet for more information
 *   
 */

#define TSL25911_I2C_ADRESS 0x29

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"

#include "connections.h"

// OLED SSD1306 w/ SSD1306 controller
#include "oled.h"
#include "lookupTable.h"

void initUART()
{
  uart_init(UART_ID, 115200);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
  // shell command: Pi 3 "minicom -b 115200 -D /dev/serial0", Pi 5 "(...) /dev/ttyAMA0"
}  

void initI2C()
{  
  i2c_init(I2C_ID, 400 * 1000);  
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  
  // using external 3.9 or 4.7 kOhm resistors, internal actually not needed
  //gpio_pull_up(SDA_PIN);
  //gpio_pull_up(SCL_PIN);   
}


void OLED_showDigits(uint32_t digit, uint32_t a, uint32_t b, uint32_t c , uint32_t d)
{
    struct render_area frame_area = {a, b, c, d};
    calc_render_area_buflen(&frame_area);
    
    if (digit == 0)
        render(null, &frame_area); 
    else if (digit == 1)
        render(one, &frame_area);
    else if (digit == 2)
        render(two, &frame_area);
    else if (digit == 3)
        render(three, &frame_area);
    else if (digit == 4)
        render(four, &frame_area);
    else if (digit == 5)
        render(five, &frame_area);
    else if (digit == 6)
        render(six, &frame_area);
    else if (digit == 7)
        render(seven, &frame_area);
    else if (digit == 8)
        render(eight, &frame_area);
    else if (digit == 9)
        render(nine, &frame_area);        
    
}

void renderNumbers(uint8_t AmbientLight, uint32_t c, uint32_t d, uint32_t e){      
            
    // this is just a weird hack, but it works sufficiently for the moment
    // exploits properties of the int datatype
    uint32_t digit[3];
    digit[0] = AmbientLight/100;
    digit[1] = (AmbientLight - (100*digit[0]))/10;  
    uint32_t interimValue = AmbientLight/10;
    digit[2] = AmbientLight - interimValue*10;
    
    for (int i=0; i < 3; i++)
    {          
      OLED_showDigits(digit[i], (48+(i*8)+e), (55+(i*8)+e), c, d);
    }
            
}

void TSL25911_ReadWriteI2C(uint8_t command[], uint32_t length){
	
	i2c_write_blocking(I2C_ID, TSL25911_I2C_ADRESS, command, length, false); // this device obviously doesn't care about Start and Stop Bit, so "true" is also fine (i. e. the final stop bit is not sent, data transfer continues as part of the same transaction)
    while (i2c_read_blocking(I2C_ID, TSL25911_I2C_ADRESS, command, length, false) < 0)  // repeats until the slave device sends ACK
    {
      sleep_ms(1);
    };  	
}

int main() 
{  
    stdio_init_all();
    initUART();
    initI2C();  
    
    // graphic scaffold, start screen
    oled_init();
    struct render_area title_area = {0, 127, 0, 4};
    calc_render_area_buflen(&title_area);
    render(startScreen, &title_area);    
        
    sleep_ms(1000);  
           
    /*
    Initialize the TSL25911 sensor
    */
    
    // see data sheet, it's a bit tricky with the registers  
     
    uint8_t TSL25911_init[1];
    
    TSL25911_init[0] = 0xA0; // COMMAND register, normal operation, addressing ENABLE register 
    TSL25911_ReadWriteI2C(TSL25911_init, 1);
    
    TSL25911_init[0] = 0x83; // ENABLE register, (0x83 would be NPIEN, AEN, PON = 1)
    TSL25911_ReadWriteI2C(TSL25911_init, 1);
    
    TSL25911_init[0] = 0xA1; // COMMAND register, normal operation, addressing CONFIG/CONTROL register 
    TSL25911_ReadWriteI2C(TSL25911_init, 1);
    
    uint32_t mask = (1 << DIP_SWITCH_PIN1 | 1 << DIP_SWITCH_PIN2);
    
    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, GPIO_IN);
    gpio_pull_up(DIP_SWITCH_PIN1);
    gpio_pull_up(DIP_SWITCH_PIN2);
    
    //  CONFIG/CONTROL register, modifying gain mode and integration time
    
    uint8_t ATIME = 0x04; // 0x04 means integration time 500ms
    
    if (!gpio_get(DIP_SWITCH_PIN1) && gpio_get(DIP_SWITCH_PIN2)){
       TSL25911_init[0] = 0x20+ATIME; // high gain        
    }        
    else if (gpio_get(DIP_SWITCH_PIN1) && !gpio_get(DIP_SWITCH_PIN2)){
       TSL25911_init[0] = 0x10+ATIME; // medium gain 
    }        
    else if (!gpio_get(DIP_SWITCH_PIN1) && !gpio_get(DIP_SWITCH_PIN2)){
       TSL25911_init[0] = 0x30+ATIME; // maximum gain      
    }     
    else{
       TSL25911_init[0] = 0x00+ATIME; // low gain      
    }         
    TSL25911_ReadWriteI2C(TSL25911_init, 1);
    
    while(true){ 
		
      char readValue[32];
    
      // Acquiring the TSL25911 ambient light data; the function TSL25911_ReadWriteI2C() could be used as well, this way I find it a little more readable though
     
      uint8_t TSL25911_LB_CH0[] = {0xB4};    
    
      i2c_write_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_LB_CH0, 1, false);   
      i2c_read_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_LB_CH0, 1, false);
      
      uint8_t TSL25911_HB_CH0[] = {0xB5};
    
      i2c_write_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_HB_CH0, 1, false);    
      i2c_read_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_HB_CH0, 1, false);   
            
      uint8_t TSL25911_LB_CH1[] = {0xB6};    
    
      i2c_write_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_LB_CH1, 1, false);    
      i2c_read_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_LB_CH1, 1, false);
      
      uint8_t TSL25911_HB_CH1[] = {0xB7};
    
      i2c_write_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_HB_CH1, 1, false);    
      i2c_read_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_HB_CH1, 1, false); 
      
      uint8_t TSL25911_STATUS[] = {0xB3};
    
      i2c_write_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_STATUS, 1, false);    
      i2c_read_blocking(I2C_ID, TSL25911_I2C_ADRESS, TSL25911_STATUS, 1, false);   
    
      sprintf(readValue, "CH0: %d (0x%x) %d (0x%x) \r\n", TSL25911_HB_CH0[0], TSL25911_HB_CH0[0], TSL25911_LB_CH0[0], TSL25911_LB_CH0[0]);  // values from 0 to 255
      uart_write_blocking(UART_ID, readValue, strlen(readValue));   
      sprintf(readValue, "CH1: %d (0x%x) %d (0x%x) (Infrared)\r\n", TSL25911_HB_CH1[0], TSL25911_HB_CH1[0], TSL25911_LB_CH1[0], TSL25911_LB_CH1[0]);  // values from 0 to 255
      uart_write_blocking(UART_ID, readValue, strlen(readValue));
      sprintf(readValue, "Status Register: %x \r\n", TSL25911_STATUS[0]);  // values from 0 to 255
      uart_write_blocking(UART_ID, readValue, strlen(readValue));       
     
      renderNumbers(TSL25911_HB_CH0[0], 2, 2, 0);
      renderNumbers(TSL25911_LB_CH0[0], 2, 2, 32);
      
      renderNumbers(TSL25911_HB_CH1[0], 3, 3, 0);       
      renderNumbers(TSL25911_LB_CH1[0], 3, 3, 32);  
      
	  sleep_ms(1000); 
	  
	} 
          
    return 0;       
}
