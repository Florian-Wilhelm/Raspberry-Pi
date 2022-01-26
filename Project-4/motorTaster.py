# Programm reagiert auf eine äußere Eingabe (Taster) und treibt einen DC-Motor an;
# Code könnte auch sehr simpel in C implementiert werden.
from machine import Pin, PWM
Taster = Pin(5, Pin.IN) # Pin 5 an Pull-Down Widerstand, Tasterbetätigung führt zu logisch High
pwm = PWM(Pin(1)) 
pwm.freq(1000) # PWM-Frequenz
while True:
    if Taster.value()==1: # in C: #define TASTER_GPIO 5 (...) TASTER_WERT=gpio_get(TASTER_GPIO) (...) if(TASTER_WERT==1) {(...)}
      pwm.duty_u16(32768)      
    else:
      pwm.duty_u16(0)
