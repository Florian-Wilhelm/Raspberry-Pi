/**
 * Created 19/09/2022 by Florian Wilhelm Dirnberger; modified 10/08/2023 (watchdog implementation)
 *
 * Sensor to ADC connections (configurations include additional resistors to create voltage dividers, see comments further below):
 * HIH4020 (relative humidity) - ADC0 (GP26)
 * NTC 103AT2 (ambient temperature) - ADC1 (GP27)
 * Phototransistor SFH 300 (ambient light) - ADC2 (GP28)
 * 
 * note 1: WiFi-credentials have to be modified (name and password, command "connectWiFi"); without that there is no point compiling this source code
 * note 2: as you may have guessed this is NOT a Pico W project; this assembly uses the classic Pico and an Espressif ESP8266 MCU
 *        Wiring ESP8266 - Pico: https://hackaday.io/project/178522-soil-moisture-measurement-device/log/207966-iot-capabilities
 * 
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <string.h>
#include "pico/time.h"
#include "hardware/adc.h"
#include "hardware/watchdog.h"

#include "hardware/pll.h"
#include "hardware/structs/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"

#define Debug true

// following values have to be preserved after soft reset via watchdog
int WDCounter __attribute__((section(".unitialized_data")));

int initUARTWiFi()
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

int startServerWiFi(uint8_t buf[], int len, int WDindicator)
{    
    int hitCounter = 0;
    
    gpio_put(25, 1);
    sleep_ms(1000);
    
    char temp[256];
    char id[10];
    uart_write_blocking(uart1, "AT+CIPMUX=1\r\n", 13);
    if (getBlocks(buf, len, 10, "OK") < 0)
        return -1;
        
    if (!gpio_get(10))
       uart_write_blocking(uart1, "AT+CIPSERVER=1,79\r\n", 19);
    else if (!gpio_get(13))
       uart_write_blocking(uart1, "AT+CIPSERVER=1,80\r\n", 19);
    else 
       uart_write_blocking(uart1, "AT+CIPSERVER=1,78\r\n", 19); // default port 78
       
    if (getBlocks(buf, len, 10, "OK") < 0)
        return -1;   
        
    gpio_put(25, 0);
    sleep_ms(1000);
    
    watchdog_enable(10000, 0);

    for (;;) // infinite polling loop (typical construct for an IoT application)      
    {   
                
        if (getBlocks(buf, len, 1, "+IPD") < 0) {
            watchdog_update();
            continue;
        }    
        
        hitCounter += 1; 
       
        char *b = strstr(buf, "+IPD");
        b += 5;
        strncpy(temp, b, sizeof(temp));
        char *e = strstr(temp, ",");
        int d = e - temp;
        memset(id, '\0', sizeof(id));
        strncpy(id, temp, d);
        
        uint absoluteTimeMinutes = (to_ms_since_boot(get_absolute_time())/1000)/60;
        float absoluteTimeHours = absoluteTimeMinutes/60.0;        
        
        adc_select_input(1); // ADC1 on Pin 32
        
        uint16_t rawTempSensor = adc_read(); // raw value (max 4095)
        float VoltTempSensor = (3.3/4096)*rawTempSensor; // voltage on the ADC input (=voltage across the NTC) 
        float physicalTempSensor = 0.7*pow(VoltTempSensor,4)-7.8*pow(VoltTempSensor,3)+32.5*pow(VoltTempSensor,2)-76.5*VoltTempSensor+70.6; // maths for the temperature sensor 103AT2 T=f(U); supply voltage 5V; formula conceived by virtue of an interpolation, parameters determined by the chosen voltage divider resistor (here: 50k) and the values from the data sheet; note: bridge arrangement might be the better choice

        adc_select_input(0); // ADC0 on Pin 31
        
        uint16_t rawRHSensor = adc_read(); // raw value (max 4095)
        float physicalRHSensor = ((((rawRHSensor-815)/2.6)*3.3)/4095)*100; // maths for the RH sensor HIH4020 (including an offset), essentially for 25°C; voltage divider with 10k+10k and 100k on sensor output; further explanation see data sheet       
        uint16_t physicalRHSensorT = (uint16_t)(physicalRHSensor/(1.0546-0.00216*physicalTempSensor)); // with temperature compensation
        
        adc_select_input(2); // ADC2 on Pin 34
        
        uint16_t rawLightSensor = adc_read(); // raw value (max 4095); I use one SFH 300 phototransistor as sensor and 5k+10k resistors as voltage dividers (supply voltage VSYS 5V)
        if (rawLightSensor > 4095) // in rare cases can exceed the max value
            rawLightSensor = 4095; 

        char data[2048];

        // the HTTP protocol is essentially a set of text headers
        sprintf(data, "HTTP/1.0 200 OK\r\nServer: Pico\r\nContent-type: text/html\r\n\r\n<html><head><title>Environment data</title><link rel=icon href=data:,></head><body><h2><font color=green>Environment data</font></h2><table><tr><td>Relative Humidity</td> <td> %u percent</th></tr> <tr><td>Ambient Temperature</td> <td> %2.1f degrees Celsius</td></tr> <tr><td>Ambient Light (12-bit raw)</td> <td> %u</td></tr> </table><p><font color=blue>Time since startup: %u minutes (%2.2f hours)</p><p>WD resets %u; Server hits: %u</font></p><p>SW V1.61; (c) ome-eng.net 2023</p></body></html>\r\n", physicalRHSensorT, physicalTempSensor, rawLightSensor, absoluteTimeMinutes, absoluteTimeHours, WDCounter, hitCounter);
        
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
        
        watchdog_update();     
        
    }
    return 0;
}

int main()
{
    /*
    system clock settings
    */  
    uint usedFreq = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY))*1000; 
     
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, // Main Clock source; here: "Auxiliary" for the System Clock clk_sys
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, // Auxiliary Clock source; here: "USB PLL"
                    usedFreq, // "Frequency of the input clock source"
                    usedFreq); // "Requested frequency"

    pll_deinit(pll_sys);    
    clock_configure(clk_peri, 
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    usedFreq,
                    usedFreq);
                    
    uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))*1000;
    
    stdio_init_all();
    
    uint32_t mask=(1 << 10 | 1 << 13);
    
    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, GPIO_IN);
    gpio_pull_up(10);
    gpio_pull_up(13);
    
    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);  
    adc_gpio_init(28);
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    
    int WDindicator = 0;
    
    if (watchdog_caused_reboot()) {
       WDindicator = 1;
       WDCounter += 1; 
    }
    else       
       WDCounter = 0;
    
    
    // there is "some" start-up time needed for the ESP8266
    gpio_put(25, 1);
    sleep_ms(3000);
    gpio_put(25, 0);
    sleep_ms(1000);    
    
    uint8_t buf[512];
    initUARTWiFi();
    modeWiFi(buf, 512, 1);
    
    gpio_put(25, 1);
    sleep_ms(1000);
    
    connectWiFi(buf, 512, "WiFi-123", "1234567890"); // to be modified
    
    gpio_put(25, 0);
    sleep_ms(1000);
    
    // getIPWiFi(buf, 512);    
    
    startServerWiFi(buf, 512, WDindicator);        
}
