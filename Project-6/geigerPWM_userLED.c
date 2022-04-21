/**
 * Created 21/04/2022 by Florian Wilhelm Dirnberger
 *
 * Output PWM signal on GP16, IRQ handling on GP01
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
// #include "hardware/adc.h"
#include "hardware/pwm.h"
// #include "hardware/irq.h"
#include "pico/time.h"


const uint userLED = 25; // the green user LED
const uint userGPIO = 1; // IRQ input

uint32_t pwm_set_freq_duty(uint slice_num, uint chan,uint32_t f, int d)
{
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

void geiger_irq_handler(uint gpio, uint32_t events) {
    if (events == GPIO_IRQ_EDGE_FALL) 
       gpio_put(userLED, 1);
    else if (events == GPIO_IRQ_EDGE_RISE) 
       gpio_put(userLED, 0);             
    else
       gpio_put(userLED, 0);
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
    
    stdio_init_all();
    gpio_init(userLED);
    gpio_set_dir(userLED, GPIO_OUT);
    
    // adc_gpio_init(26);
    // adc_select_input(0);
    
    //gpio_disable_pulls(userGPIO);
    gpio_set_irq_enabled_with_callback(userGPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &geiger_irq_handler);    

    // Wait forever
    while (1);    
    
    return 0;
}
