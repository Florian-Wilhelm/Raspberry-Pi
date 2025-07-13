/**
 * Created January 2024 by Florian Wilhelm Dirnberger
 *
 * program will flash cyclically two LEDs in alternation; these are connected to GP2 and GP3
 * it isn't a code design worth copying, and solely intended for learning purposes and for getting to grips with the PIO
 * 
 */

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "doubleBlinky.pio.h" // located in the build directory after the compiling process

int main()
{
    int n = 0;
    
    uint offset = pio_add_program(pio0, &doubleBlinky_program);
    
    uint sm = pio_claim_unused_sm(pio0, true); 
    pio_sm_config c = doubleBlinky_program_get_default_config(offset);
    
    sm_config_set_set_pins(&c,2,2); // start on Pin 2 and use Pin 2 + 3 (2 Pins)
    sm_config_set_out_pins(&c,2,2);
    pio_gpio_init(pio0,2);
    pio_gpio_init(pio0,3);
    
    sm_config_set_clkdiv_int_frac(&c,255,0);
    pio_sm_init(pio0,sm,offset,&c);
    pio_sm_set_enabled(pio0,sm,true);  
    
    pio_sm_put_blocking(pio0, sm, 0x0002FFFF);  // for the delaying loop
    
    while(true) // one LED on, the other off
    {
        if (n == 0){
          pio_sm_put_blocking(pio0, sm, 0x2); 
          n+=1;
         } 
        
        else if (n == 1){
          pio_sm_put_blocking(pio0, sm, 0x1);
          n-=1;
        }
    }            
    
    return 0;
}
