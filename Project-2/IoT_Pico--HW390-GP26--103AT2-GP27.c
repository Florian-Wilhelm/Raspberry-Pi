/**
 * Created 20/08/2022 by Florian Wilhelm Dirnberger
 *
 * code is in large parts copied from Harry Fairheads book, page 347
 *   
 * it's still sort of an IoT demo code; it doesn't do very much except sending some sensor measurement data thru the internet
 * 
 * HW-390 capacitive soil moisture sensor - ADC0 (GP26)
 * NTC temperature sensor 103AT2 (as part of a voltage divider) - ADC1 (GP27)
 * 
 * note: WLAN-parameters have to be modified here (name, password)
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <string.h>
#include "pico/time.h"
#include "hardware/adc.h"

#define Debug true

int initWiFi()
{
    uart_init(uart1, 115200);
    gpio_set_function(4, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);
    uart_set_format(uart1, 8, 1, UART_PARITY_NONE);

    uart_set_translate_crlf(uart0, true);
    sleep_ms(100);
    return 0;
}

int getBlock(uint8_t buf[], int len)
{
    int count = 0;
    while (count < len - 1)
    {
        if (uart_is_readable_within_us(uart1, 10000))
        {
            buf[count++] = uart_getc(uart1);
            if (Debug)
                uart_putc(uart0, buf[count - 1]);
        }
        else
        {
            break;
        }
    }
    buf[count] = 0;
    return count;
}

int ATWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT\r\n";
    uart_write_blocking(uart1, SendData, 4);
    return getBlock(buf, len);
}

int getVersionWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT+GMR\r\n";
    uart_write_blocking(uart1, SendData, 8);
    return getBlock(buf, len);
}

int resetWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT+RST\r\n";
    uart_write_blocking(uart1, SendData, 8);
    return getBlock(buf, len);
}

int setUARTWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT+UART_CUR=115200,8,1,0,0\r\n";
    uart_write_blocking(uart1, SendData, 28);
    return getBlock(buf, len);
}

int modeWiFi(uint8_t buf[], int len, int mode)
{
    uint8_t command[32];
    int count = snprintf(command, 32, "AT+CWMODE_CUR=%d\r\n", mode);
    uart_write_blocking(uart1, command, count);
    return getBlock(buf, len);
}

int getBlocks(uint8_t buf[], int len, int num, char target[])
{
    for (int i = 0; i < num; i++)
    {
        if (uart_is_readable_within_us(uart1, 1000 * 1000))
        {
            getBlock(buf, len);
            if (strstr(buf, target))
                return i;
        }
    }
    return -1;
}

int scanWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT+CWLAP\r\n";
    uart_write_blocking(uart1, SendData, 18);
    return getBlocks(buf, len, 20, "OK");
}

int connectWiFi(uint8_t buf[], int len, char ssid[], char pass[])
{
    uint8_t command[128];
    int count = snprintf(command, 128, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", ssid, pass);
    uart_write_blocking(uart1, command, count);
    return getBlocks(buf, len, 20, "OK");
}

int getIPWiFi(uint8_t buf[], int len)
{
    uint8_t SendData[] = "AT+CIFSR\r\n";
    uart_write_blocking(uart1, SendData, 10);
    return getBlocks(buf, len, 20, "OK");
}

int getWebPageWiFi(uint8_t buf[], int len, char URL[], char page[])
{
    uint8_t command[128];
    int count = snprintf(command, 128, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n", URL);
    uart_write_blocking(uart1, command, count);
    if (getBlocks(buf, len, 20, "OK") < 0)
        return -1;
    char http[150];
    sprintf(http, "GET %s HTTP/1.1\r\nHost:%s\r\n\r\n", page, URL);
    count = snprintf(command, 128, "AT+CIPSEND=%d\r\n", strlen(http));
    uart_write_blocking(uart1, command, count);
    if (getBlocks(buf, len, 20, ">") < 0)
        return -1;
    uart_write_blocking(uart1, http, strlen(http));
    return getBlocks(buf, len, 20, "</html>");
}

int startServerWiFi(uint8_t buf[], int len)
{
    
    gpio_put(25, 1);
    sleep_ms(1000);
    
    char temp[256];
    char id[10];
    uart_write_blocking(uart1, "AT+CIPMUX=1\r\n", 13);
    if (getBlocks(buf, len, 10, "OK") < 0)
        return -1;
    uart_write_blocking(uart1, "AT+CIPSERVER=1,80\r\n", 19);
    if (getBlocks(buf, len, 10, "OK") < 0)
        return -1;   
        
    gpio_put(25, 0);
    sleep_ms(1000);

    for (;;) // infinite polling loop (typical construct for an IoT application)      
    {   
        if (getBlocks(buf, len, 1, "+IPD") < 0)
            continue;

        char *b = strstr(buf, "+IPD");
        b += 5;
        strncpy(temp, b, sizeof(temp));
        char *e = strstr(temp, ",");
        int d = e - temp;
        memset(id, '\0', sizeof(id));
        strncpy(id, temp, d);
        
        adc_select_input(0);
        
        float readCapSensor = 6125-(4.95*(adc_read()/3.0)); // maths for the capacitive soil moisture sensor HW-390       
        uint16_t outputCapSensor = (uint16_t)readCapSensor; // produces an int raw value
        
        adc_select_input(1);
        
        uint16_t vorErg = adc_read(); // raw value (max 4095)
        float zwErg = (3.3/4096)*vorErg; // representing voltage U
        float readTempSensor = 0.7*pow(zwErg,4)-7.8*pow(zwErg,3)+32.5*pow(zwErg,2)-76.5*zwErg+70.6; // maths for the NTC temperature sensor 103AT2 T=f(U); formula conceived by virtue of an interpolation    

        char data[2048];
        
        sprintf(data, "HTTP/1.0 200 OK\r\nServer: Pico\r\nContent-type: text/html\r\n\r\n<html><head><title>Moisture</title></head><body><p>Soil moisture raw value (0-4095): %u; Temperature: %2.1f oC; (c)ome-eng.net</p></body></html>\r\n", outputCapSensor, readTempSensor);
        
        uint8_t command[128];
        int count = snprintf(command, 128, "AT+CIPSEND=%s,%d\r\n", id, strlen(data));
        uart_write_blocking(uart1, command, count);
        if (getBlocks(buf, len, 10, ">") < 0)
            return -1;

        uart_write_blocking(uart1, data, strlen(data));
        if (getBlocks(buf, len, 10, "OK") < 0)
            return -1;
        count = snprintf(command, 128, "AT+CIPCLOSE=%s\r\n", id);
        uart_write_blocking(uart1, command, count);

        if (getBlocks(buf, len, 10, "OK") < 0)
            return -1;
    }
    return 0;
}

int main()
{
    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);    
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    
    // there is "some" start-up time needed for the ESP8266
    gpio_put(25, 1);
    sleep_ms(4000);
    gpio_put(25, 0);
    sleep_ms(1000);
    
    stdio_init_all();
    uint8_t buf[512];
    initWiFi();
    modeWiFi(buf, 512, 1);
    
    gpio_put(25, 1);
    sleep_ms(1000);
    
    connectWiFi(buf, 512, "WLAN-ABC", "12345"); // modify WLAN settings here
    
    gpio_put(25, 0);
    sleep_ms(1000);
    
    // getIPWiFi(buf, 512);    
    
    startServerWiFi(buf, 512);       
}
