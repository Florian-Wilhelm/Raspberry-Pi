/**
 * Created 20/04/2022 by Florian Wilhelm Dirnberger
 *
 * Does not much except generating a PWM signal
 * 
 */

#include "pico/stdlib.h"
#include "hardware/pwm.h"

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d)
{
    uint32_t clock = 125000000; // default clock 125 MHz
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0); // summand "(clock % (f * 4096) != 0)" is either 1 (true) or 0 (false); standard way of rounding up positive values if there is a remainder
    if (divider16 / 16 == 0)
      divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    
    // pwm_set_phase_correct (slice_num, false);
    pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap); // setting the PWM frequency 
    pwm_set_chan_level(slice_num, chan, wrap * d / 100); // setting the duty cycle
    return wrap;
}

int main() {    
    uint usedPin = 16;
    uint usedFrequency = 1250;
    
    gpio_set_function(usedPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(usedPin);
    uint chan = pwm_gpio_to_channel(usedPin);
    
    pwm_set_freq_duty(slice_num, chan, usedFrequency, 45);
    pwm_set_enabled(slice_num, true);
    
    sleep_ms(5000);
    
    pwm_set_freq_duty(slice_num, chan, usedFrequency, 60);
    pwm_set_enabled(slice_num, true);
    
    // once set, it runs forever; no "while(1)" necessary in this program
    
    return 0;    
}