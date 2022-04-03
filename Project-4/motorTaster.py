# Programm reagiert auf eine äußere Eingabe (Taster) und treibt einen DC-Motor an;
# Algorithmus kann auch sehr simpel in C/C++ implementiert werden;
# Hinweis: produktiv einzusetzende Programme sollten IRQs nutzen.
from machine import Pin, PWM
Taster = Pin(5, Pin.IN) # Pin 5 an Pull-Down Widerstand (kann 10k sein), Tasterbetätigung führt zu logisch High (Taster hängt mit einem Kontakt an 3.3V, und mit dem anderem Kontakt an Pin 5)
pwm = PWM(Pin(1)) 
pwm.freq(1000) # PWM-Frequenz
while True:
    if Taster.value()==1: # in C: #define TASTER_GPIO 5 (...) TASTER_WERT=gpio_get(TASTER_GPIO) (...) if(TASTER_WERT==1) {(...)}
      pwm.duty_u16(32768)      
    else:
      pwm.duty_u16(0)
