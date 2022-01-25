# Programm reagiert auf äußere Eingabe (Taster) und treibt einen DC-Motor an
from machine import Pin, PWM
import utime
Taster = Pin(5, Pin.IN) # Pin 5 an Pull-Down Widerstand, Tasterbetätigung führt zu logisch High
pwm = PWM(Pin(1)) 
pwm.freq(1000) # PWM-Frequenz
while True:
    if Taster.value()==1: 
      pwm.duty_u16(32768)      
    else:
      pwm.duty_u16(0)