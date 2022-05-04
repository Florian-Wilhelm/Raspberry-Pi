# Programm reagiert auf eine äußere Eingabe (entprellter Taster) und treibt einen DC-Motor an.
from machine import Pin, PWM
Taster = Pin(5, Pin.IN) # Pin 5 an Pull-Down Widerstand (10k oder mehr); Taster an 3.3V, Betätigung führt zu logisch High
pwm = PWM(Pin(1)) 
pwm.freq(1000) 
while True:
    if Taster.value()==1: 
      pwm.duty_u16(32768)      
    else:
      pwm.duty_u16(0)
