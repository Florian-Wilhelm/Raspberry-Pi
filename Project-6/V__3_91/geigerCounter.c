/**
 * Created 24/05/2023 by Florian Wilhelm Dirnberger
 *
 * Geiger counter program for running on a Raspberry Pi Pico with attached prototype PCB 
 *   
 * HV-generation by virtue of a PWM (for the boost-converter); triggers simple output devices (LEDs, buzzer etc.) in case of an interrupt (counting event)
 * In addition, an OLED will show basic data (in order to save power for every activated pixel consumes some current, only a bare minimum of data is displayed)
 * 
 * Difference SW-versions 1.x -> 2.x/3.x is a control loop for the HV
 * Difference SW-versions 2.x -> 3.x is OLED display instead of an LCD
 *
 * The system clock is diminished to 3 MHz thru this SW in order to reduce the power consumption
 * SW-version 3.9x includes a simple battery indicator (additional external resistors needed, not compatible with HW revision 3.x)
 * 
 * Important note: pull-up resistors are ENabled for the IRQ GP and for the I2C (although the used 0.96'' SSD1306 OLED has its own pull-ups)
 * 
 */
 
 /* 
  this SW-version: see lookupTable.h 
  to be used with HW revision: HW3.2
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
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
    oled_send_cmd(OLED_HEIGHT - 1); 

    oled_send_cmd(OLED_SET_COM_OUT_DIR | 0x08); // set COM (common) output scan direction
    // scan from bottom up, COM[N-1] to COM0

    oled_send_cmd(OLED_SET_DISP_OFFSET); // set display offset
    oled_send_cmd(0x00); // no offset

    oled_send_cmd(OLED_SET_COM_PIN_CFG); // set COM (common) pins hardware configuration
    oled_send_cmd(0x02); // manufacturer magic number

    /* timing and driving scheme */
    oled_send_cmd(OLED_SET_DISP_CLK_DIV); // set display clock divide ratio
    oled_send_cmd(0x10); 

    oled_send_cmd(OLED_SET_PRECHARGE); // set pre-charge period
    oled_send_cmd(0xF1); // Vcc internally generated on our board

    oled_send_cmd(OLED_SET_VCOM_DESEL); // set VCOMH deselect level
    oled_send_cmd(0x30); // 0.83xVcc

    /* display */    
    oled_send_cmd(OLED_SET_CONTRAST); // set contrast control
    if (!gpio_get(OLED_CONTRASTCONTROL_GP)) 
        oled_send_cmd(0xFF);  // high brightness (=higher current) 
    else 
        oled_send_cmd(0x0A);  // low brightness (=lower current)

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
uint32_t setMask_TimeStamp_ms = 0;

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
        setMask_TimeStamp_ms = to_ms_since_boot(get_absolute_time()); 
        countingEvent += 1; 
        pulsesTimeframe += 1; }     
    else if (events == GPIO_IRQ_EDGE_RISE) 
        gpio_clr_mask(deviceGP);    
}

void show_digits(uint digit, uint a, uint b, uint c , uint d)
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

void show_highRadIndication(uint indicate, uint c , uint d)
{    
    struct render_area frame_area = {88, 95, c, d};
    calc_render_area_buflen(&frame_area);
    
    if (indicate == 0)
        render(clr_digit, &frame_area); 
    else if (indicate == 1)
        render(hot, &frame_area);    
}

void renderNumbers(uint CountsPerMinute, uint c, uint d){      
            
    // this is just a weird hack, but it works sufficiently for the moment
    // exploits properties of the int datatype
    uint digit[3];
    digit[0] = CountsPerMinute/100;
    digit[1] = (CountsPerMinute - (100*digit[0]))/10;  
    uint interimValue = CountsPerMinute/10;
    digit[2] = CountsPerMinute - interimValue*10;
    
    for (int i=0; i < 3; i++)
    {          
      show_digits(digit[i], (48+(i*8)), (55+(i*8)), c, d);
    }
      
    uint8_t indicate;   
    // if there is a radiation source nearby, the display will throw an indication
    if (CountsPerMinute > 75)
       indicate = 1;
    else
       indicate = 0;
       
    show_highRadIndication(indicate, c, d);            
}

void show_BatteryIndication(uint16_t batteryRawValue)
{
    struct render_area frame_area = {120, 127, 3, 3};
    calc_render_area_buflen(&frame_area);
    
    const float conversion_factor = 3.3f / (1 << 12);            
    
    // note: ADC input resistance is not infinite! if voltage divider resistors are in the MOhm range (I've chosen 5 MOhm and 2.2 MOhm), a small voltage drop can be expected
    // values for the voltage stated below are empirical
    
    if ((batteryRawValue * conversion_factor) > 2.40) // can never exceed 3.3V with a 9V block and the chosen resistances
        render(full, &frame_area); 
    else if (((batteryRawValue * conversion_factor) <= 2.40) && ((batteryRawValue * conversion_factor) > 1.9))
        render(half_empty, &frame_area);
    else if (((batteryRawValue * conversion_factor) <= 1.9) && ((batteryRawValue * conversion_factor) > 1.5))
        render(empty, &frame_area);  
    else
        render(implausible, &frame_area);       // anything else is considered implausible        
}

void clear_startScreen()
{    
    // clearing data on the display after a few seconds in order to diminish power consumption (every pixel needs some current)
    
    int m = 8;
    
    for (int i=0; i < 5; i++)
    {
      struct render_area delete_area_1 = {88 + (m*i), 95 + (m*i), 3, 3};
      
      calc_render_area_buflen(&delete_area_1);    
      render(clr_digit, &delete_area_1);
      
      struct render_area delete_area_2 = {88 + (m*i), 95 + (m*i), 0, 0};
      calc_render_area_buflen(&delete_area_2);    
      render(clr_digit, &delete_area_2);
      
    }    
}

int main() {     
     
    /*
    ADC settings (for battery indication)
    */ 
    adc_init();
    adc_gpio_init(28);    
    adc_select_input(2);
    sleep_ms(100); // wait until voltage has settled
    uint16_t batteryRawValue = adc_read();  
    
    /*
    system clock settings
    */
    uint usedFreq = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC)); // "ROSC (...) is guaranteed to be in the range 1.8MHz to 12MHz" (see RP2040 data sheet, chapter 2.17.1)
          
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, // Main Clock source; here: "Auxiliary" for the System Clock clk_sys
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_ROSC_CLKSRC, // Auxiliary Clock source; here: "ROSC"
                    usedFreq*KHZ, // "Frequency of the input clock source"
                    usedFreq*KHZ); // "Requested frequency"
                    
    uint32_t *ROSC = (uint32_t *)0x40060000; // ROSC_BASE
    
    *(ROSC + 0x10 / 4) = 0x00000aa0; // divider value (see RP2040 data sheet, chapter 2.17.8)
    
    /*uint32_t *CLK = (uint32_t *)0x40008000; // CLOCKS_BASE
    
    *(CLK + 0x40 / 4) = 0x00000200;*/  // using this register (i.e. offset) has some drawbacks
    
    // saving a further few mA
    pll_deinit(pll_sys);           
                    
    uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))*1000;
    
    stdio_init_all();
    
    /*
    OLED settings
    */
    i2c_init(I2C_ID, 50 * 1000);
    gpio_set_function(DISPLAY_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(DISPLAY_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(DISPLAY_SDA_PIN);
    gpio_pull_up(DISPLAY_SCL_PIN);    
    
    gpio_init(OLED_CONTRASTCONTROL_GP);
    gpio_set_dir(OLED_CONTRASTCONTROL_GP, GPIO_IN);
    gpio_pull_up(OLED_CONTRASTCONTROL_GP);
    oled_init();
    
    // graphic scaffold, start screen
    struct render_area title_area = {0, 127, 0, 4};
    calc_render_area_buflen(&title_area);
    render(startScreen, &title_area);     
    
    // display battery status    
    show_BatteryIndication(batteryRawValue);  
    
    sleep_ms(50);      
    
    /*
    Geiger counter settings
    */
    gpio_set_function(PWM_GP, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_GP);
    uint chan = pwm_gpio_to_channel(PWM_GP);        
       
    gpio_init_mask(deviceGP);   
    gpio_set_dir_out_masked(deviceGP);
    
    gpio_init(X_GP);
    gpio_set_dir(X_GP, GPIO_IN);
    gpio_pull_up(X_GP);
    
    gpio_init(CALC_METHODCONTROL_GP);
    gpio_set_dir(CALC_METHODCONTROL_GP, GPIO_IN);
    gpio_pull_up(CALC_METHODCONTROL_GP);
    
    gpio_set_pulls(IRQ_GP, 1, 0);
    gpio_set_irq_enabled_with_callback(IRQ_GP, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &geiger_irq_handler);     
    
    uint n = 1;
    uint m = 1; 
    uint g = 0;
    uint k = 0;  
    float cpm = 0.0f; // why a float? to generate a sound extrapolated value
    uint CPM = 0;
    float timeFrame = 2.0f; // cpm calc method #1
    float pulsesFixed = 10.0f;  // cpm calc method #2; why a float? to generate a sound extrapolated value
    const float sixtySeconds = 60.0f;
    uint32_t countingEvent_minus1Minute = 0;  
    uint32_t absoluteTimeLastcpmCalculation = 0; 
    
    uint ringbuffer_elements=3;  
    float mean;  
    float ringbuffer[ringbuffer_elements]; 
    float sum;    
    
    struct render_area transition = {32, 47, 1, 1};        
    
    sleep_ms(50);       
               
    for (;;) {
        
        // HV control loop
        if (!gpio_get(X_GP)) {
           pwm_set_freq_duty(f_clk_sys, slice_num, chan, PWM_FQ, 0); 
           pwm_set_enabled(slice_num, true);}     
        else {
           pwm_set_freq_duty(f_clk_sys, slice_num, chan, PWM_FQ, 60);           
           pwm_set_enabled(slice_num, true);}           
          
       uint32_t absoluteTime = (to_ms_since_boot(get_absolute_time())/1000); 
       uint32_t absoluteTime_ms = to_ms_since_boot(get_absolute_time());
       
       if (absoluteTime_ms - setMask_TimeStamp_ms == 200){
            gpio_clr_mask(deviceGP); // adding some robustness
        }
       
       if (absoluteTime == 5)  {
             clear_startScreen(); 
        }            
       
       if (absoluteTime - g*60 == 62) {
          calc_render_area_buflen(&transition);    
          render(clr_digit, &transition); // clear the small transition arrow          
          g+=1; 
       }   
       
       // the calculation methods in the following are sort of preliminary (there exist different methods to deal with purely statistical data such as radioactivity)        
       
       if (gpio_get(CALC_METHODCONTROL_GP)){  // active by default       
       
         if ((absoluteTime/(timeFrame*n)) == 1) { 
             
           if (absoluteTime<=(ringbuffer_elements*timeFrame)){  // first few seconds when nothing is in the ringbuffer             
                   
             cpm = (pulsesTimeframe/timeFrame)*sixtySeconds;  
             pulsesTimeframe = 0;
             
             ringbuffer[n-1] = cpm;                            
         
             renderNumbers(cpm, 2, 2);         
             n+=1;    }
             
           else{            
             
             if (k == ringbuffer_elements)
               k=0;
               
             sum = 0;
           
             for (int i=0; i < ringbuffer_elements; i++)
               {          
                 sum += ringbuffer[i]; 
               }
               
             mean = sum/ringbuffer_elements;             
             renderNumbers(mean, 2, 2);
           
             cpm = (pulsesTimeframe/timeFrame)*sixtySeconds;
             pulsesTimeframe = 0;
             
             ringbuffer[k] = cpm;
             
             k+=1;
             n+=1;    }
          
         }           
       } 
       
       else if (!gpio_get(CALC_METHODCONTROL_GP)) { // not active by default; not really a sound algorithm so far anyway       
           
         if (pulsesTimeframe == pulsesFixed) {
             
           uint32_t DeltaTime =  absoluteTime - absoluteTimeLastcpmCalculation;
             
           cpm = (pulsesFixed/DeltaTime)*sixtySeconds;           
           pulsesTimeframe = 0;                            
         
           renderNumbers(cpm, 2, 2);            
               
           absoluteTimeLastcpmCalculation = absoluteTime;
           
         }
           
       }               
       
       if (absoluteTime/m == 60) {  
           
         CPM = countingEvent - countingEvent_minus1Minute; // real Counts Per Minute, updated every 60s        
         
         renderNumbers(CPM, 1, 1);  
         m+=1;        
         
         calc_render_area_buflen(&transition);  // indication for transition to the next minute
         render(arrow, &transition);
         
         countingEvent_minus1Minute = countingEvent; 
         
        } 
       
    }                 
    return 0;
}
