/**
 * 
 * Test code as of September 25, 2025, created by FWD
 * 
 * MLX90642 IR array serves as main sensor
 * TSL25911 ambient light sensor is an additional component for reference
 * 
 * OLED for thermal display, some data output on the serial port as I see fit
 * SW 0.2x -> OLED display
 * SW 0.4x -> IoT
 *  
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// physical connections
#include "connections.h"

// OLED SSD1306
#include "oled.h"
#include "lookupTable.h"

void initUART()
{
  uart_init(uart0, 9600);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
}  

void initI2C()
{  
  i2c_init(I2C_ID, 400 * 1000); // as to the MLX90642, up to 1MHz would be possible; for refresh rate 8Hz we need at least 400kHz   
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  
  // using external 3.9 kOhm resistors, internal actually not needed
  //gpio_pull_up(SDA_PIN);
  //gpio_pull_up(SCL_PIN);   
}

void renderImage(uint digit, uint a, uint b, uint c , uint d)
{
    struct render_area frame_area = {a, b, c, d};
    calc_render_area_buflen(&frame_area);
    
    if (digit == 0)
        render(bright, &frame_area);
    else if (digit == 1)
        render(clr, &frame_area);
    else if (digit == 2)
        render(sober, &frame_area);
               
}

void clear_startScreen()
{    
    // clearing display start screen after a few seconds
    
    int m = 8;
    
    for (int i=0; i < 15; i++)
    {
      struct render_area delete_area_1 = {0 + (m*i), 7 + (m*i), 1, 1};
      
      calc_render_area_buflen(&delete_area_1);    
      render(clr, &delete_area_1);
      
      struct render_area delete_area_2 = {0 + (m*i), 7 + (m*i), 0, 0};
      calc_render_area_buflen(&delete_area_2);    
      render(clr, &delete_area_2);
      
    }    
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
    
    sleep_ms(2000);             
    clear_startScreen();
            
    char readValue[32];
    
    //uint32_t absoluteTime = (to_ms_since_boot(get_absolute_time())/1000); 
    //uint32_t absoluteTime_ms = to_ms_since_boot(get_absolute_time());
    
    /*
    Initialize the MLX90642 sensor
    */ 
    
    // the default refresh rate is 8Hz, default mode is continuous mode (measurements are constantly running) 
    
    //uint8_t set_refresh_rate[6] = {0x3A, 0x2E, 0x11, 0xF0, 0x00, 0x03}; // 4Hz
    uint8_t set_refresh_rate[6] = {0x3A, 0x2E, 0x11, 0xF0, 0x00, 0x04}; // 8Hz
    //uint8_t set_refresh_rate[6] = {0x3A, 0x2E, 0x11, 0xF0, 0x00, 0x05}; // 16Hz
    
    i2c_write_blocking(I2C_ID, 0x66, set_refresh_rate, 6, false); 
    
    sleep_ms(500);
     
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
    
    sleep_ms(1);
    
     // test I2C commands for TSL25911
      uint8_t TSL25911_LB_CH0[] = {0xB4};    
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_LB_CH0, 1, false);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_LB_CH0, 1, false);
      
      uint8_t TSL25911_HB_CH0[] = {0xB5};
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_HB_CH0, 1, false);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_HB_CH0, 1, false);   
    
      sprintf(readValue, "TSL25911 CH0: %x %x \r\n", TSL25911_HB_CH0[0], TSL25911_LB_CH0[0]);  
      uart_write_blocking(UART_ID, readValue, strlen(readValue));    
   
      
      // test I2C commands for MLX90642 
       
      // *the array consists of 768 IR sensors
      // *each IR sensor delivers 2 Byte of data
      // *image data starts at address 0x342C (Pixel [1:1]), and ends at address 3A2A (Pixel [24:32])
      // *raw data starts at address 0x2E2A (Pixel [1:1]), and ends at address 3A2A (Pixel [24:32])      
      
      //uint16_t ImageDataStartAddress = 0x342C;
      //uint16_t ImageDataEndAddress = 0x3A2A;
      //uint16_t RawDataStartAddress = 0x2E2A;
      //uint16_t RawDataEndAddress = 0x312A;    
      
      uint16_t upperPixelAddress;
      uint16_t lowerPixelAddress;
      
      uint16_t decimalValueTemperature;
      
      uint16_t pixelAddress;
      
      uint16_t thresholdUp = 30; // Temperature
      uint16_t thresholdDown = 26; // Temperature
      
      char temperature[32];
       
      sprintf(temperature, "Upper temperature threshold %d degrees Celsius \r\n", thresholdUp); 
      uart_write_blocking(UART_ID, temperature, strlen(temperature));
      
      sleep_ms(1);
      
      sprintf(temperature, "Lower temperature threshold %d degrees Celsius \r\n", thresholdDown); 
      uart_write_blocking(UART_ID, temperature, strlen(temperature));
   
      sleep_ms(1);
    
      while(true){   
                 
           // still a bit unclear how to address individual pixels, but even addresses must be used
                            
           // the following could be done shorter with a 2nd loop 
                
           for (int i=0x01; i <= 0x0F; i++) 
           
           { 
             
             if (i == 1)
               pixelAddress = 0x34BC;
               //pixelAddress = 0x34DC; 
      
             upperPixelAddress = (pixelAddress & 0xFF00) >> 8;
             lowerPixelAddress = pixelAddress & 0x00FF;
                 
             uint8_t pixelValue[4] = {upperPixelAddress, lowerPixelAddress};
    
             i2c_write_blocking(I2C_ID, 0x66, pixelValue, 2, true);      
             i2c_read_blocking(I2C_ID, 0x66, pixelValue, 2, false); 
           
             decimalValueTemperature = ((pixelValue[0] << 8) + pixelValue[1]) / 50;  // see data sheet for the formula     
           
             sleep_ms(2);
              // 1st display line        
              if (decimalValueTemperature > thresholdUp)
                renderImage(0, (0+i*8), (7+i*8), 0, 0); 
              else if (decimalValueTemperature < thresholdDown)
                renderImage(1, (0+i*8), (7+i*8), 0, 0);
              else
                renderImage(2, (0+i*8), (7+i*8), 0, 0); 
               
              pixelAddress = pixelAddress + 0x02;
            
              sleep_ms(2);
           
           }
           
           for (int i=0x01; i <= 0x0F; i++) 
           
           { 
             
             if (i == 1)
               pixelAddress = 0x34FC;
               //pixelAddress = 0x351C; 
      
             upperPixelAddress = (pixelAddress & 0xFF00) >> 8;
             lowerPixelAddress = pixelAddress & 0x00FF;
                 
             uint8_t pixelValue[4] = {upperPixelAddress, lowerPixelAddress};
    
             i2c_write_blocking(I2C_ID, 0x66, pixelValue, 2, true);      
             i2c_read_blocking(I2C_ID, 0x66, pixelValue, 2, false); 
           
             decimalValueTemperature = ((pixelValue[0] << 8) + pixelValue[1]) / 50;  // see data sheet for the formula     
           
             sleep_ms(2);
              // 2nd display line        
              if (decimalValueTemperature > thresholdUp)
                renderImage(0, (0+i*8), (7+i*8), 1, 1); 
              else if (decimalValueTemperature < thresholdDown)
                renderImage(1, (0+i*8), (7+i*8), 1, 1);
              else
                renderImage(2, (0+i*8), (7+i*8), 1, 1); 
               
              pixelAddress = pixelAddress + 0x02;
            
              sleep_ms(2);
           
           }
           
           for (int i=0x01; i <= 0x0F; i++) 
           
           { 
             
             if (i == 1)
               pixelAddress = 0x353C;
               //pixelAddress = 0x355C; 
      
             upperPixelAddress = (pixelAddress & 0xFF00) >> 8;
             lowerPixelAddress = pixelAddress & 0x00FF;
                 
             uint8_t pixelValue[4] = {upperPixelAddress, lowerPixelAddress};
    
             i2c_write_blocking(I2C_ID, 0x66, pixelValue, 2, true);      
             i2c_read_blocking(I2C_ID, 0x66, pixelValue, 2, false); 
           
             decimalValueTemperature = ((pixelValue[0] << 8) + pixelValue[1]) / 50;  // see data sheet for the formula     
           
             sleep_ms(2);
              // 3rd display line       
              if (decimalValueTemperature > thresholdUp)
                renderImage(0, (0+i*8), (7+i*8), 2, 2); 
              else if (decimalValueTemperature < thresholdDown)
                renderImage(1, (0+i*8), (7+i*8), 2, 2);
              else
                renderImage(2, (0+i*8), (7+i*8), 2, 2); 
               
              pixelAddress = pixelAddress + 0x02;
            
              sleep_ms(2);
           
           }
           
           for (int i=0x01; i <= 0x0F; i++)  
           
           { 
             
             if (i == 1)
               pixelAddress = 0x357C;
               //pixelAddress = 0x359C;
      
             upperPixelAddress = (pixelAddress & 0xFF00) >> 8;
             lowerPixelAddress = pixelAddress & 0x00FF;
                 
             uint8_t pixelValue[4] = {upperPixelAddress, lowerPixelAddress};
    
             i2c_write_blocking(I2C_ID, 0x66, pixelValue, 2, true);      
             i2c_read_blocking(I2C_ID, 0x66, pixelValue, 2, false); 
           
             decimalValueTemperature = ((pixelValue[0] << 8) + pixelValue[1]) / 50;      
           
             sleep_ms(2);
              // 4th display line       
              if (decimalValueTemperature > thresholdUp)
                renderImage(0, (0+i*8), (7+i*8), 3, 3); 
              else if (decimalValueTemperature < thresholdDown)
                renderImage(1, (0+i*8), (7+i*8), 3, 3);
              else
                renderImage(2, (0+i*8), (7+i*8), 3, 3); 
               
              pixelAddress = pixelAddress + 0x02;
            
              sleep_ms(2);
           
           }      
         
   }
          
    return 0;       
}
