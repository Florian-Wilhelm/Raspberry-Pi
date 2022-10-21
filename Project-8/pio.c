/**
 * Created 20/10/2022 by Florian Wilhelm Dirnberger
 *
 * Code is entirely copied from Harry Fairhead's book, page 241.
 * 
 */

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "sqwave.pio.h"

int main()
{
    uint offset = pio_add_program(pio0, &squarewave_program);
    
    uint sm = pio_claim_unused_sm(pio0, true); // the SDK chooses the state machine
    pio_sm_config c = squarewave_program_get_default_config(offset);
    
    sm_config_set_clkdiv_int_frac(&c,255,0);
    
    sm_config_set_set_pins(&c, 25, 1);
    pio_gpio_init(pio0, 25);
    
    pio_sm_init(pio0, sm, offset, &c);
    pio_sm_set_enabled(pio0, sm, true);
    return 0;
}
