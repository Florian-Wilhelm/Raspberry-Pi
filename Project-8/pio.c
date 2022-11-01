/**
 * Created 31/10/2022 by Florian Wilhelm Dirnberger
 *
 * Code is copied from Harry Fairhead's book, page 244 (some modifications).
 * 
 */

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "sqwave.pio.h"

int main()
{
    uint offset = pio_add_program(pio1, &squarewave_program);
    
    uint sm = pio_claim_unused_sm(pio1, true); // the SDK chooses the s(tate) m(achine) (4 sm available)
    pio_sm_config c = squarewave_program_get_default_config(offset); // setting up the struct
    
    sm_config_set_set_pins(&c,25,1);
    pio_gpio_init(pio1, 25);
    
    sm_config_set_clkdiv_int_frac(&c,255,0); 
    pio_sm_init(pio1, sm, offset, &c);    
    pio_sm_set_enabled(pio1, sm, true);
        
    pio_sm_put_blocking(pio1, sm, 0x0001FFFF); // stores a 32-bit value in the TX FIFO   
    return 0;
}
