/**
 * Created 07/01/2024 by Florian Wilhelm Dirnberger
 *
 * Playing around with and exploring the RP2040 Clocks (with or without an external clock source), so this code is mostly a quick&dirty construct
 * Connected to the Pico is a 16x2 LCD for convenience to display some of the clock frequencies
 * 
 * SW-V 1.1
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "hardware/pwm.h"

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/time.h"

#define DISPLAY_SDA_PIN 4
#define DISPLAY_SCL_PIN 5
#define I2C_ID i2c0

// LCD commands and instructions (generic)

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

// the actual program starts here

void measure_freqs(void) {
    
    char snum[6];
    
    gpio_put(25, 1);
    
    lcd_set_cursor(1,0);
    lcd_string("kHz!  V1.1");
    
    sleep_ms(1000);   
    
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS)); 
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
    uint f_pll_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY))/1000; // display in MHz      
    
    /*
    uint f_pll_usb = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY))/1000; // display in MHz
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);    
    */
    
    itoa(f_rosc, snum, 10); // ROSC will have slightly different value after every power up, as stated in the data sheet
    lcd_set_cursor(1,0);
    lcd_string(snum);
    
    itoa(f_clk_sys, snum, 10); 
    lcd_set_cursor(1,6);
    lcd_string(snum);
    
    uint n = 1; 
    
    gpio_put(25, 0);  
    
    while (1) {
        
       uint absolutZeit = (to_us_since_boot(get_absolute_time())/1000000); 
       
       if ((absolutZeit/n) == 1)
       {
          itoa(absolutZeit, snum, 10);
          lcd_set_cursor(1,13);
          lcd_string(snum);
          n+=1;
       }
       
   }

}

void pwm_set_freq_duty(uint32_t f_clk_sys_Hz, uint slice_num, uint chan, uint32_t f, int d) {
    if (f < 2000)
       f = 2000;
    uint16_t wrap = f_clk_sys_Hz/f; // 16bit - max value 65535 for the counter, i.e. result must not exceed this value (min f is 1907 Hz for f_clk_sys 125 MHz)
    pwm_set_wrap(slice_num, wrap); // again, max value is 65535
    pwm_set_chan_level(slice_num, chan, (wrap*d)/100); // determining the duty cycle   
}

int main() {
   
    stdio_init_all();
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    
    uint32_t mask=(1 << 10 | 1 << 13 | 1 << 14 | 1 << 14);
    
    gpio_init_mask(mask);
    gpio_set_dir_masked(mask, GPIO_IN);
    gpio_pull_up(10);
    gpio_pull_up(13); 
    gpio_pull_up(14);
    gpio_pull_up(17);
    
    // Using the I2C for the LCD
    i2c_init(I2C_ID, 100 * 1000);
    gpio_set_function(DISPLAY_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_SDA_PIN);
    gpio_pull_up(DISPLAY_SCL_PIN);    
    
    lcd_init();      
    lcd_clear(); 
    
    lcd_set_cursor(0,0);  
    lcd_string("ROSC");
    
    lcd_set_cursor(0,5);  
    lcd_string("SYS_CLK (s)");    
    
    sleep_ms(100); 
    
    // Choosing frequency for clk_sys thru a jumper
     
    if (!gpio_get(13)){
     
    // the System Clock will be permanently delivered by the Reference Clock, which is in turn generated by the crystal oscillator (module)
    // references:
    // - RP2040 datasheet chapter 2.16.1
    // - SDK datasheet chapter 4.1.4     
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF, 
                    0,          // zero because no Auxiliary Clock Source is necessary 
                    14.31*MHZ,     // "Frequency of the input clock source", supposedly a divider value (actually no idea what this value is really about)
                    14.31*MHZ);    // "Requested frequency", supposedly a divider value (actually no idea what this value is really about)
              
    /*uint32_t *RTC = (uint32_t *)0x40008000;
    
    *(RTC + 0x70 / 4) = 0xffffffff;*/
    
    
    }
                    
    else if (!gpio_get(14)){                    
                    
    // Change clk_sys to be 48MHz. The simplest way is to take this from PLL_USB which has a source frequency of 48MHz
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);
                    
    }
                    
    else if (!gpio_get(10)){
                    
    uint usedFreq = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC));
          
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, // Main Clock source; here: "Auxiliary" for the System Clock clk_sys
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_ROSC_CLKSRC, // Auxiliary Clock source; here: "ROSC"
                    usedFreq*KHZ, // see above
                    usedFreq*KHZ); // see above
                    
    uint32_t *ROSC = (uint32_t *)0x40060000; // ROSC_BASE (see RP2040 data sheet, chapter 2.17.8)
    
    *(ROSC + 0x10 / 4) = 0x00000aa0; // ROSC divider 
    
      // ROSC control
      if (!gpio_get(17)){    
        *(ROSC + 0x00 / 4) = 0x00fabfa7; // HIGH
      }
      
      else {          
        *(ROSC + 0x00 / 4) = 0x00fabfa5; // MEDIUM  
      }
    
    }
    
    else{
        
    uint32_t *XOSC = (uint32_t *)0x40024000; // XOSC_BASE    
    *(XOSC + 0x00 / 4) = 0x00fabaa0;  // Crystal Oscillator can be de-activated by using hex value 0x00d1eaa0; otherwise nothing really happening here
        
    }
    
    // experimenting with the PWM functionality, and whether the frequency is affected
    uint f_clk_sys_Hz = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))*1000;
    
    gpio_set_function(22, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(22);
    uint chan = pwm_gpio_to_channel(22);  
    
    pwm_set_freq_duty(f_clk_sys_Hz, slice_num, chan, 10000, 50);
    pwm_set_enabled(slice_num, true);
                    
    sleep_ms(100); 
    
    // Display
    measure_freqs();

    return 0;
}
