#ifndef __CONNECTIONS_H__
#define __CONNECTIONS_H__

// Flexible output GPIOs (only for prototype stage); GPs not soldered to PCB, therefore available: GP0-GP15; user LED Pin 25
#define DEVICEMASK (1<<8) | (1<<9) | (1<<25) 

// PWM frequency
#define PWM_FQ 1250

// PWM out GP
#define PWM_GP 16

// IRQ GP
#define IRQ_GP 17

// I2C Pins for the LCD
#define DISPLAY_SDA_PIN 4
#define DISPLAY_SCL_PIN 5
#define I2C_ID i2c0 // this is not an independent parameter, change it according to chosen SDA and SCL pins (see Pico datasheet)

#endif
