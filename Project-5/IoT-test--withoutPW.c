/**
 * Created 20/05/2022 by Florian Wilhelm Dirnberger
 *
 * For testing purposes with a DiP-Pi PIoT; this program is basically functional
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define Debug true

int initWiFi()
{
    uart_init(uart0, 115200);
    gpio_set_function(12, GPIO_FUNC_UART);
    gpio_set_function(13, GPIO_FUNC_UART);
    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
    sleep_ms(100);
    return(0);    
}

int getBlock(uint8_t buf[], int len)
{
    int count = 0;
    while (count < len - 1)
    {
        if (uart_is_readable_within_us(uart0, 10000))
        {
            buf[count++] = uart_getc(uart0);
            if (Debug)
            uart_putc(uart0,buf[count-1]);
        }
        else
        {
            break;
        }
    }
    buf[count] = 0;
    return count;
}

int getBlocks(uint8_t buf[], int len, int num, char target[])
{
    for (int i = 0; i < num; i++)
    {
        if (uart_is_readable_within_us(uart0, 1000*1000)); 
          getBlock(buf, len);
        if (strstr(buf, target))
          return i;
    }
    return -1;
}

int ATWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT\r\n";
    uart_write_blocking(uart0, SendData, 4);
    return getBlock(buf, len);
}

int modeWiFi(uint8_t buf[], int len, int mode)
{
    uint8_t command[32];
    int count = snprintf(command, 32, "AT+CWMODE_CUR=%d\r\n", mode);
    uart_write_blocking(uart0, command, count);
    return getBlock(buf, len);
}

int connectWiFi(uint8_t buf[], int len, char ssid[], char pass[])
{
    uint8_t command[128];
    int count = snprintf(command, 128,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", ssid, pass);
    uart_write_blocking(uart0, command, count);
    
    return getBlocks(buf, len, 20, "OK");
}

int main()    
{
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    
    gpio_put(25, 1); 
    
    sleep_ms(1000);   
    
    stdio_init_all();
    initWiFi();    
    
    uint8_t buf[512];
    modeWiFi(buf, 512, 1);  
    
    connectWiFi(buf, 512, "WLAN-ABC", "12345"); // insert the correct parameters of your WiFi here
    
    gpio_put(25, 0);
    
    /* while (1){
    ATWiFi(buf, 512);
    sleep_ms(10000);
     }    */
}
