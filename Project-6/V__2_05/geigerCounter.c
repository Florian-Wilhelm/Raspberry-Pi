/**
 * Created 16/01/2023 by Florian Wilhelm Dirnberger
 *
 * Geiger counter program for running on a Raspberry Pi Pico with attached prototype PCB 
 *   
 * HV-generation by virtue of a PWM (for the boost-converter); triggers simple output devices (LEDs, buzzer etc.) in case of an interrupt (counting event)
 * In addition, an LCD will show counting events, elapsed time and CPM
 * 
 * Difference versions 1.x -> 2.x is a control loop for the HV
 * 
 * note: pull-up resistors are ENabled (push-pull mode) for the IRQ GP and for the I2C (external may be better for a robust design)
 * 
 */
 
 // this SW-version (will be displayed on the LCD):
 char SW_Version[5] = "V2.05";
 /* to be used with HW revision:
 * HW3.1 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
// #include "pico/binary_info.h"
#include "pico/time.h"
#include "connections.h"

/*#include "hardware/pll.h"
#include "hardware/structs/pll.h"*/
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"

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

// now the actual Geiger counter program

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

const uint32_t deviceGP=DEVICEMASK; 
uint32_t countingEvent = 0;
uint32_t pulsesTimeframe = 0;
char snum[6];

void geiger_irq_handler(uint gpio, uint32_t events) {
    if (events == GPIO_IRQ_EDGE_FALL) {
        gpio_set_mask(deviceGP); 
        countingEvent += 1; 
        pulsesTimeframe += 1; }     
    else if (events == GPIO_IRQ_EDGE_RISE) 
        gpio_clr_mask(deviceGP);    
}

int main() {   
    
    //stdio_init_all(); 
    
    // changing the system clock frequency (handle with care), will change the PWM freq
    
    //uint frequency = measure_freqs()/1000; 
    
    /*clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);

    // Turn off PLL sys for good measure
    pll_deinit(pll_sys);

    // CLK peri is clocked from clk_sys so need to change clk_peri's freq
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    48 * MHZ,
                    48 * MHZ);*/
    
    uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))/1000;

    stdio_init_all();   
    
    gpio_set_function(PWM_GP, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_GP);
    uint chan = pwm_gpio_to_channel(PWM_GP);        
    
    pwm_set_freq_duty(slice_num, chan, PWM_FQ, 5); // a little play-around with the duty-cycle
    pwm_set_enabled(slice_num, true);
    
    sleep_ms(200);
    
    // Using the I2C for the LCD
    i2c_init(I2C_ID, 100 * 1000);
    gpio_set_function(DISPLAY_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_SDA_PIN);
    gpio_pull_up(DISPLAY_SCL_PIN);
    // Make the I2C pins available to picotool
    //bi_decl(bi_2pins_with_func(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN, GPIO_FUNC_I2C));
    
    lcd_init();      
    lcd_clear();  
    
    lcd_set_cursor(0,6);  
    lcd_string("PWM (Hz)");    
    lcd_set_cursor(1,6);
    itoa(PWM_FQ, snum, 10);
    lcd_string(snum);  
     
    lcd_set_cursor(0,0);  
    lcd_string("SW");
    lcd_set_cursor(1,0);
    lcd_string(SW_Version); 
    
    lcd_set_cursor(0,15);  
    lcd_string("s");    
    lcd_set_cursor(1,13);
    itoa(f_clk_sys, snum, 10);
    lcd_string(snum);               
    
    sleep_ms(1600);
    
    lcd_clear();
    lcd_set_cursor(0,0);  
    lcd_string("Pulse");
    lcd_set_cursor(0,7);  
    lcd_string("T(s)"); 
    lcd_set_cursor(0,12);  
    lcd_string("CPM");      
    
    sleep_ms(200);
        
    gpio_init_mask(deviceGP);   
    gpio_set_dir_out_masked(deviceGP);
    
    gpio_init(X_GP);
    gpio_set_dir(X_GP, GPIO_IN);
    gpio_pull_up(X_GP);
    
    gpio_set_pulls(IRQ_GP, 1, 0);
    gpio_set_irq_enabled_with_callback(IRQ_GP, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &geiger_irq_handler); 
    
    uint n = 1;   
    uint CPM = 0; 
    float timeFrame = 4.0;
               
    while (1) {
        
        if (!gpio_get(X_GP)) {
           pwm_set_freq_duty(slice_num, chan, PWM_FQ, 0); 
           pwm_set_enabled(slice_num, true);}     
        else {
           pwm_set_freq_duty(slice_num, chan, PWM_FQ, 60);           
           pwm_set_enabled(slice_num, true);}           
          
       uint absolutZeit = (to_us_since_boot(get_absolute_time())/1000000);  
       
       if (((absolutZeit)/(timeFrame*n)) == 1) {          
         lcd_set_cursor(1,12);
         lcd_string("    ");
         CPM = (pulsesTimeframe/timeFrame)*60.0;  
         pulsesTimeframe = 0;
         n+=1; }                         
                                 
       itoa(countingEvent, snum, 10); 
       lcd_set_cursor(1,0);
       lcd_string(snum); 
            
       itoa(absolutZeit, snum, 10); 
       lcd_set_cursor(1,7);
       lcd_string(snum); 
       
       itoa(CPM, snum, 10); 
       lcd_set_cursor(1,12);
       lcd_string(snum);             
            
    }           
      
    return 0;
}
