#ifndef __CONNECTIONS_H__
#define __CONNECTIONS_H__

// Flexible output GPIOs (can be changed)
#define DEVICEMASK (1<<8) | (1<<9) | (1<<25) 

// PWM frequency 
#define PWM_FQ 10000

// PWM out GP 
#define PWM_GP 3

// IRQ GP 
#define IRQ_GP 2

// X GP (control loop)
#define X_GP 22

// OLED contrast control GP
#define OLED_CONTRASTCONTROL_GP 18 // dip switch #1

// TBD (not used right now)
#define TBD 19 // dip switch #2

// OLED I2C Pins (can be changed)
#define DISPLAY_SDA_PIN 4
#define DISPLAY_SCL_PIN 5
#define I2C_ID i2c0 // this is not an independent parameter, change it according to chosen SDA and SCL pins (see Pico datasheet)

#endif
