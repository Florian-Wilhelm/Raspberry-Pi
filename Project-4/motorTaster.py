# Programm reagiert auf eine äußere Eingabe (Taster) und treibt einen DC-Motor an.
from machine import Pin, PWM
Taster = Pin(5, Pin.IN) # Pin 5 an Pull-Down Widerstand (kann 10k sein), Tasterbetätigung führt zu logisch High
pwm = PWM(Pin(1)) 
pwm.freq(1000) 
while True:
    if Taster.value()==1: 
      pwm.duty_u16(32768)      
    else:
      pwm.duty_u16(0)