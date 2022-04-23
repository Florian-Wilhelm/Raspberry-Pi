/**
 * Created 23/04/2022 by Florian Wilhelm Dirnberger
 *
 * Output PWM signal on GP16, IRQ handling on GP17
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
//#include "hardware/irq.h"
#include "pico/time.h"


const uint userLED = 25; // the green user LED
const uint userGPred = 9;
const uint userGPyellow = 8;
const uint userIRQgpio = 17; // IRQ input

const uint usedPWMgpio = 16;
const uint usedPWMfreq = 1250;

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
    if (events == GPIO_IRQ_EDGE_FALL) {
       gpio_put(userLED, 1);
       gpio_put(userGPred, 1);
       gpio_put(userGPyellow, 1); }
    else if (events == GPIO_IRQ_EDGE_RISE) {
       gpio_put(userLED, 0);
       gpio_put(userGPred, 0);
       gpio_put(userGPyellow, 0); }            
    /*else {
       gpio_put(userGPred, 0);
       gpio_put(userLED, 0);
       gpio_put(userGPyellow, 0); }*/
}

int main() {    
    
    gpio_set_function(usedPWMgpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(usedPWMgpio);
    uint chan = pwm_gpio_to_channel(usedPWMgpio);
    pwm_set_freq_duty(slice_num, chan, usedPWMfreq, 45);
    pwm_set_enabled(slice_num, true);
    sleep_ms(5000);
    pwm_set_freq_duty(slice_num, chan, usedPWMfreq, 60);
    pwm_set_enabled(slice_num, true);
    
    stdio_init_all();
    gpio_init(userGPred);
    gpio_set_dir(userGPred, GPIO_OUT);
    gpio_init(userGPyellow);
    gpio_set_dir(userGPyellow, GPIO_OUT);
    gpio_init(userLED);
    gpio_set_dir(userLED, GPIO_OUT);    
    
    //gpio_disable_pulls(userIRQgpio);
    gpio_set_irq_enabled_with_callback(userIRQgpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &geiger_irq_handler);    

    // Wait forever
    while (1);    
    
    return 0;
}
