#ifndef __CONNECTIONS_H__
#define __CONNECTIONS_H__

// Flexible output GPIOs (only for prototype stage); GPs not soldered to PCB, therefore available: GP0-GP15; user LED Pin 25
#define DEVICEMASK (1<<8) | (1<<9) | (1<<25) 

// PWM frequency
#define PWMFQ 1250

// PWM out GP
#define PWMGP 16

// IRQ GP
#define IRQGP 17

#endif
