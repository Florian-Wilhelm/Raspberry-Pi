/**
 * Created 30/10/2022 by Florian Wilhelm Dirnberger
 *
 * Geiger counter program for running on a Raspberry Pi Pico with attached prototype PCB
 * 
 * this SW-version:
 * V1.1
 * to be used with HW revision:
 * HW2.0 and below
 *   
 * HV-generation by virtue of a PWM (for the boost-converter); triggers simple output devices (LEDs, buzzer etc.) in case of an interrupt (counting event)
 * In addition, an LCD will show counting events and the elapsed time (in lieu of CPM)
 * 
 * note: Pull-down/up resistors are disabled (push-pull mode) for the IRQ GP
 * 
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/time.h"

#define MASK (1<<8) | (1<<9) | (1<<25) // flexible output GPIOs (only for prototype stage); GPs not soldered to PCB, therefore available: GP0-GP15; user LED Pin 25

// instructions for the LCD (generic)

// commands
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

// now the actual Geiger counter program

// fixed GPIOs of prototype Rev 2.0
const uint userIRQgpio = 17;
const uint usedPWMgpio = 16;

// empirical PWM value
const uint usedPWMfreq = 1250;

// output GPIOs
const uint32_t mask=MASK; 

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d) {
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
      divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

uint countingEvent = 0;

void geiger_irq_handler(uint gpio, uint32_t events) {
    if (events == GPIO_IRQ_EDGE_FALL) {
        gpio_set_mask(mask); 
        countingEvent += 1; }     
    else if (events == GPIO_IRQ_EDGE_RISE) 
        gpio_clr_mask(mask);                   
    /*else {
        }*/
}

char snum[5];

int main() {   
         
    gpio_set_function(usedPWMgpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(usedPWMgpio);
    uint chan = pwm_gpio_to_channel(usedPWMgpio);
    
    pwm_set_freq_duty(slice_num, chan, usedPWMfreq, 45); // a little play-around with the duty-cycle
    pwm_set_enabled(slice_num, true);
    
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();
    
    lcd_clear();  
    lcd_set_cursor(0,0);  
    lcd_string("SW");
    lcd_set_cursor(0,8);  
    lcd_string("PWM (Hz)");
    lcd_set_cursor(1,0);
    lcd_string("V1.1"); 
    itoa(usedPWMfreq, snum, 10);
    lcd_set_cursor(1,8);
    lcd_string(snum);               
    
    sleep_ms(2000);
    
    lcd_clear();
    lcd_set_cursor(0,0);  
    lcd_string("Pulse");
    lcd_set_cursor(0,8);  
    lcd_string("Zeit(s)");    
    
    pwm_set_freq_duty(slice_num, chan, usedPWMfreq, 60);
    pwm_set_enabled(slice_num, true);
    
    stdio_init_all();
        
    gpio_init_mask(mask);   
    gpio_set_dir_out_masked(mask);
    
    gpio_disable_pulls(userIRQgpio);
    gpio_set_irq_enabled_with_callback(userIRQgpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &geiger_irq_handler); 
               
    while (1) {             
          
       uint AbsolutZeit = (to_us_since_boot(get_absolute_time())/1000000);            
                      
       itoa(countingEvent, snum, 10); 
       lcd_set_cursor(1,0);
       lcd_string(snum); 
            
       itoa(AbsolutZeit, snum, 10); 
       lcd_set_cursor(1,8);
       lcd_string(snum); 
                  
       //lcd_clear();             
       //sleep_ms(100);      
    }           
      
    return 0;
}
