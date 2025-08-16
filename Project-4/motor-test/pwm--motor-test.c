/**
 * 
 * Testing a Brushed DC motor.
 * 
 * Some external electronic is necessary (transistor, diode, capacitors, resistors) in order to drive the motor with a 9V block battery.
 * See electronic literature for possible configurations.
 *  
 */

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
//#include "hardware/structs/clocks.h"

uint32_t pwm_set_freq_duty(uint32_t f_clk_sys, uint slice_num, uint chan, uint32_t f, int d)

{
    // reducing the PWM frequency enough for a motor to work with, involves some maths (see literature)
    uint32_t clock = f_clk_sys;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0); // the result in the brackets is either 1 or 0, will be added at the end
    if (divider16 / 16 == 0)
    divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);   
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
    
}

int main() {
    
    uint f_clk_sys = (frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS))*1000;
    
    int freq = 200;
    int duty = 0;    
    
    gpio_set_function(22, GPIO_FUNC_PWM); // DC-motor on A output of the 3rd slice
    gpio_set_function(25, GPIO_FUNC_PWM); // onBoard LED on B output of the 4th slice; additional visual control
        
    uint slice_num__motor = pwm_gpio_to_slice_num(22);
    uint chan__motor = pwm_gpio_to_channel(22);
    
    uint slice_num__led = pwm_gpio_to_slice_num(25);
    uint chan__led = pwm_gpio_to_channel(25);
    
    pwm_set_enabled(slice_num__motor, true);
    pwm_set_phase_correct (slice_num__motor, true);
    
    pwm_set_enabled(slice_num__led, true);
    pwm_set_phase_correct (slice_num__led, true);
    
    int i; 
    
    for (i = 0; i <= 60; i++)
     {
       pwm_set_freq_duty(f_clk_sys, slice_num__motor, chan__motor, freq, duty);
       pwm_set_freq_duty(f_clk_sys, slice_num__led, chan__led, freq, duty);
       
       // increasing the duty cycle every 0.5s by 1
       duty=duty+1;
       sleep_ms(500); 
     }    
       
    return 0;       
}
