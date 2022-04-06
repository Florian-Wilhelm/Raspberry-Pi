/**
 * Created 04/04/2022 by Florian Wilhelm Dirnberger
 *
 * What is an IRQ? This script will probably tell you.
 *
 * Use a (debounced) pushbutton on GPIO1 that connects the GPIO with 3.3V when pushed.
 * GPIO1 and GND are coupled via 10 kOhm resistor. Hence logical 0 on GPIO1 is the default state.
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
#include "pico/time.h"

const uint userLED = 25; // the green user LED
const uint userGPIO = 1;

void taster_irq_handler(uint gpio, uint32_t events) {
    if (events == GPIO_IRQ_EDGE_RISE)
       gpio_put(userLED, 1);
    else
       gpio_put(userLED, 0);
}

int main() {
    stdio_init_all();
    gpio_init(userLED);
    gpio_set_dir(userLED, GPIO_OUT);
    
    gpio_set_irq_enabled_with_callback(userGPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &taster_irq_handler);   

    // Wait forever
    while (1);    
    
    return 0;
}
