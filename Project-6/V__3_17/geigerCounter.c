/**
 * Created 03/02/2023 by Florian Wilhelm Dirnberger
 *
 * Geiger counter program for running on a Raspberry Pi Pico with attached prototype PCB 
 *   
 * HV-generation by virtue of a PWM (for the boost-converter); triggers simple output devices (LEDs, buzzer etc.) in case of an interrupt (counting event)
 * In addition, an OLED will show basic data
 * 
 * Difference SW-versions 1.x -> 2.x/3.x is a control loop for the HV
 * Difference SW-versions 2.x -> 3.x is OLED display instead of an LCD
 * 
 * The system clock is reduced to 48 MHz thru this SW in order to reduce the power consumption
 *
 * Important note: pull-up resistors are ENabled for the IRQ GP and for the I2C (although the used 0.96'' SSD1306 OLED has its own pull-ups)
 * 
 */
 
 /* 
  this SW-version: see lookupTable.h 
  to be used with HW revision: HW3.1 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
// #include "pico/binary_info.h"
#include "pico/time.h"
#include "connections.h"
#include "lookupTable.h"

#include "hardware/pll.h"
#include "hardware/structs/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"

// OLED commands and instructions (generic)

// commands (see datasheet)
#define OLED_SET_CONTRAST _u(0x81)
#define OLED_SET_ENTIRE_ON _u(0xA4)
#define OLED_SET_NORM_INV _u(0xA6)
#define OLED_SET_DISP _u(0xAE)
#define OLED_SET_MEM_ADDR _u(0x20)
#define OLED_SET_COL_ADDR _u(0x21)
#define OLED_SET_PAGE_ADDR _u(0x22)
#define OLED_SET_DISP_START_LINE _u(0x40)
#define OLED_SET_SEG_REMAP _u(0xA0)
#define OLED_SET_MUX_RATIO _u(0xA8)
#define OLED_SET_COM_OUT_DIR _u(0xC0)
#define OLED_SET_DISP_OFFSET _u(0xD3)
#define OLED_SET_COM_PIN_CFG _u(0xDA)
#define OLED_SET_DISP_CLK_DIV _u(0xD5)
#define OLED_SET_PRECHARGE _u(0xD9)
#define OLED_SET_VCOM_DESEL _u(0xDB)
#define OLED_SET_CHARGE_PUMP _u(0x8D)
// #define OLED_SET_HORIZ_SCROLL _u(0x26)
#define OLED_SET_SCROLL _u(0x2E)

#define OLED_ADDR _u(0x3C)
#define OLED_HEIGHT _u(32)
#define OLED_WIDTH _u(128)
#define OLED_PAGE_HEIGHT _u(8)
#define OLED_NUM_PAGES OLED_HEIGHT / OLED_PAGE_HEIGHT
#define OLED_BUF_LEN (OLED_NUM_PAGES * OLED_WIDTH)

#define OLED_WRITE_MODE _u(0xFE)
#define OLED_READ_MODE _u(0xFF)

struct render_area {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
};

void fill(uint8_t buf[], uint8_t fill) {
    // fill entire buffer with the same byte
    for (int i = 0; i < OLED_BUF_LEN; i++) {
        buf[i] = fill;
    }
}

void calc_render_area_buflen(struct render_area *area) {
    // calculate how long the flattened buffer will be for a render area
    area->buflen = (area->end_col - area->start_col + 1) * (area->end_page - area->start_page + 1);
} 

void oled_send_cmd(uint8_t cmd) {
    // I2C write process expects a control byte followed by data
    // this "data" can be a command or data to follow up a command

    // Co = 1, D/C = 0 => the driver expects a command
    uint8_t buf[2] = {0x80, cmd};
    i2c_write_blocking(I2C_ID, (OLED_ADDR & OLED_WRITE_MODE), buf, 2, false);
}

void oled_send_buf(uint8_t buf[], int buflen) {    
    uint8_t *temp_buf = malloc(buflen + 1);

    for (int i = 1; i < buflen + 1; i++) {
        temp_buf[i] = buf[i - 1];
    }
    // Co = 0, D/C = 1 => the driver expects data to be written to RAM
    temp_buf[0] = 0x40;
    i2c_write_blocking(I2C_ID, (OLED_ADDR & OLED_WRITE_MODE), temp_buf, buflen + 1, false);

    free(temp_buf);
}

void oled_init() {

    oled_send_cmd(OLED_SET_DISP | 0x00); // set display off

    /* memory mapping */
    oled_send_cmd(OLED_SET_MEM_ADDR); // set memory address mode
    oled_send_cmd(0x00); // horizontal addressing mode

    /* resolution and layout */
    oled_send_cmd(OLED_SET_DISP_START_LINE); // set display start line to 0

    oled_send_cmd(OLED_SET_SEG_REMAP | 0x01); // set segment re-map
    // column address 127 is mapped to SEG0

    oled_send_cmd(OLED_SET_MUX_RATIO); // set multiplex ratio
    oled_send_cmd(OLED_HEIGHT - 1); // our display is only 32 pixels high

    oled_send_cmd(OLED_SET_COM_OUT_DIR | 0x08); // set COM (common) output scan direction
    // scan from bottom up, COM[N-1] to COM0

    oled_send_cmd(OLED_SET_DISP_OFFSET); // set display offset
    oled_send_cmd(0x00); // no offset

    oled_send_cmd(OLED_SET_COM_PIN_CFG); // set COM (common) pins hardware configuration
    oled_send_cmd(0x02); // manufacturer magic number

    /* timing and driving scheme */
    oled_send_cmd(OLED_SET_DISP_CLK_DIV); // set display clock divide ratio
    oled_send_cmd(0x80); // div ratio of 1, standard freq

    oled_send_cmd(OLED_SET_PRECHARGE); // set pre-charge period
    oled_send_cmd(0xF1); // Vcc internally generated on our board

    oled_send_cmd(OLED_SET_VCOM_DESEL); // set VCOMH deselect level
    oled_send_cmd(0x30); // 0.83xVcc

    /* display */
    oled_send_cmd(OLED_SET_CONTRAST); // set contrast control
    oled_send_cmd(0xFF);

    oled_send_cmd(OLED_SET_ENTIRE_ON); // set entire display on to follow RAM content

    oled_send_cmd(OLED_SET_NORM_INV); // set normal (not inverted) display

    oled_send_cmd(OLED_SET_CHARGE_PUMP); // set charge pump
    oled_send_cmd(0x14); // Vcc internally generated on our board

    oled_send_cmd(OLED_SET_SCROLL | 0x00); 

    oled_send_cmd(OLED_SET_DISP | 0x01); // turn display on
}

void render(uint8_t *buf, struct render_area *area) {
    // update a portion of the display with a render area
    oled_send_cmd(OLED_SET_COL_ADDR);
    oled_send_cmd(area->start_col);
    oled_send_cmd(area->end_col);

    oled_send_cmd(OLED_SET_PAGE_ADDR);
    oled_send_cmd(area->start_page);
    oled_send_cmd(area->end_page);

    oled_send_buf(buf, area->buflen);
}

// now the actual Geiger counter program

uint32_t deviceGP=DEVICEMASK; 
uint32_t countingEvent = 0;
uint32_t pulsesTimeframe = 0;

void pwm_set_freq_duty(uint32_t f_clk_sys, uint slice_num, uint chan, uint32_t f, int d) {
    uint32_t clock = f_clk_sys;    
    if (f < 2000)
       f = 2000;
    uint16_t wrap = clock/f; // 16bit - max value 65535 for the counter, i.e. result must not exceed this value (min f is 1907 Hz for f_clk_sys 125 MHz)
    pwm_set_wrap(slice_num, wrap); // again, max value is 65535
    pwm_set_chan_level(slice_num, chan, (wrap*d)/100); // determining the duty cycle   
}

void geiger_irq_handler(uint gpio, uint32_t events) {
    if (events == GPIO_IRQ_EDGE_FALL) {
        gpio_set_mask(deviceGP); 
        countingEvent += 1; 
        pulsesTimeframe += 1; }     
    else if (events == GPIO_IRQ_EDGE_RISE) 
        gpio_clr_mask(deviceGP);    
}

// updating the display
static uint8_t null[]  = {0x00,0x3e,0x7f,0x49,0x45,0x7f,0x3e,0x00}; 
static uint8_t one[]   = {0x00,0x40,0x44,0x7f,0x7f,0x40,0x40,0x00}; 
static uint8_t two[]   = {0x00,0x62,0x73,0x51,0x49,0x4f,0x46,0x00}; 
static uint8_t three[] = {0x00,0x22,0x63,0x49,0x49,0x7f,0x36,0x00}; 
static uint8_t four[]  = {0x00,0x18,0x18,0x14,0x16,0x7f,0x7f,0x10}; 
static uint8_t five[]  = {0x00,0x27,0x67,0x45,0x45,0x7d,0x39,0x00}; 
static uint8_t six[]   = {0x00,0x3e,0x7f,0x49,0x49,0x7b,0x32,0x00}; 
static uint8_t seven[] = {0x00,0x03,0x03,0x79,0x7d,0x07,0x03,0x00}; 
static uint8_t eight[] = {0x00,0x36,0x7f,0x49,0x49,0x7f,0x36,0x00}; 
static uint8_t nine[]  = {0x00,0x26,0x6f,0x49,0x49,0x7f,0x3e,0x00};

static uint8_t planuci[]  = {0x00,0x00,0x00,0x4f,0x4f,0x00,0x00,0x00}; 
static uint8_t studeny[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void show_digits(uint digit, uint a, uint b, uint c , uint d, uint s)
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
        
    struct render_area frame_area_s = {80, 87, 3, 3};
    calc_render_area_buflen(&frame_area_s);
    
    if (s == 0)
        render(studeny, &frame_area_s); 
    else if (s == 1)
        render(planuci, &frame_area_s);
}

int main() {  
    
    stdio_init_all();      
    
    // Change clk_sys to be 48MHz. The simplest way is to take this from PLL_USB
    // which has a source frequency of 48MHz
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);

    // not necessary for the function itself, but saves a further few mA
    pll_deinit(pll_sys);    
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    48 * MHZ,
                    48 * MHZ);
                    
    uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))*1000;
    
    stdio_init_all();   
    
    gpio_set_function(PWM_GP, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_GP);
    uint chan = pwm_gpio_to_channel(PWM_GP);        
    
    pwm_set_freq_duty(f_clk_sys, slice_num, chan, PWM_FQ, 5); // a little play-around with the duty-cycle
    pwm_set_enabled(slice_num, true);
    
    sleep_ms(100);
    
    // OLED connected to the I2C bus
    i2c_init(I2C_ID, 100 * 1000);
    gpio_set_function(DISPLAY_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_SDA_PIN);
    gpio_pull_up(DISPLAY_SCL_PIN);    
    
    oled_init();
    
    struct render_area area = {0, IMG_WIDTH - 1, 0, OLED_NUM_PAGES - 1};    
    uint8_t buf[OLED_BUF_LEN];    
    
    // zero the entire display
    calc_render_area_buflen(&area);
    fill(buf, 0x00);
    render(buf, &area);  
    
    // graphic scaffold, start screen
    struct render_area title_area = {0, IMG_WIDTH - 1, 0, 4};
    calc_render_area_buflen(&title_area);
    render(lookupTable, &title_area);
    
    sleep_ms(100);      
    
    gpio_init_mask(deviceGP);   
    gpio_set_dir_out_masked(deviceGP);
    
    gpio_init(X_GP);
    gpio_set_dir(X_GP, GPIO_IN);
    gpio_pull_up(X_GP);
    
    gpio_set_pulls(IRQ_GP, 1, 0);
    gpio_set_irq_enabled_with_callback(IRQ_GP, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &geiger_irq_handler);     
    
    uint n = 1;   
    float CPM = 0;
    float timeFrame = 4.0;
    
    uint s = 0;
               
    while (1) {
        
        // HV control loop
        if (!gpio_get(X_GP)) {
           pwm_set_freq_duty(f_clk_sys, slice_num, chan, PWM_FQ, 0); 
           pwm_set_enabled(slice_num, true);}     
        else {
           pwm_set_freq_duty(f_clk_sys, slice_num, chan, PWM_FQ, 60);           
           pwm_set_enabled(slice_num, true);}           
          
       uint32_t absolutZeit = (to_us_since_boot(get_absolute_time())/1000000);  
       
       if (((absolutZeit)/(timeFrame*n)) == 1) {         
         CPM = (pulsesTimeframe/timeFrame)*60.0;  
         pulsesTimeframe = 0;
         n+=1; 
         
         // indication if there is a radiation source nearby
         if (CPM > 75)
            s = 1;
         else
            s = 0;            
         
         // this is just a weird hack, but it works sufficiently for the moment
         // exploits properties of the int datatype
         uint firstDigit = CPM/100;
         uint secondDigit = (CPM - (100*firstDigit))/10;  
         uint interimValue = CPM/10;
         uint thirdDigit = CPM - interimValue*10;            
                  
         show_digits(firstDigit, 40, 47, 3, 3, s);         
         show_digits(secondDigit, 48, 55, 3, 3, s);        
         show_digits(thirdDigit, 56, 63, 3, 3, s);  
       }   
       
    }                 
    return 0;
}
