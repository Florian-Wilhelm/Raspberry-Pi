#ifndef __CONNECTIONS_H__
#define __CONNECTIONS_H__

// Flexible output GPIOs (can be changed)
#define DEVICEMASK (1<<8) | (1<<9) | (1<<25) 

// PWM frequency (can be changed for testing purposes)
#define PWM_FQ 1000

// PWM out GP (do not change!)
#define PWM_GP 3

// IRQ GP (do not change!)
#define IRQ_GP 2

// X GP (do not change!)
#define X_GP 22

// I2C Pins for the LCD (can be changed)
#define DISPLAY_SDA_PIN 4
#define DISPLAY_SCL_PIN 5
#define I2C_ID i2c0 // this is not an independent parameter, change it according to chosen SDA and SCL pins (see Pico datasheet)

#endif
