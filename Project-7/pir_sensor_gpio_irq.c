/**
 * Created 06/04/2022 by Florian Wilhelm Dirnberger
 *
 * A PIR sensor is connected to GPIO1.
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
    
    gpio_disable_pulls(userGPIO); // disabling the pullup/pulldown resistor; depends on the used PIR sensor whether necessary or not
    gpio_set_irq_enabled_with_callback(userGPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &taster_irq_handler);    

    // Wait forever
    while (1);    
    
    return 0;
}
