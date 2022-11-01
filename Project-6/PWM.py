# "quick&dirty" PWM program for testing the boost converter
from machine import Pin, PWM
import utime
anschluss = 16
frequenz = 1250
LED = Pin(25, Pin.OUT)
pwm = PWM(Pin(anschluss))
pwm.freq(frequenz)
pwm.duty_u16(30000)
LED.on()
utime.sleep(5)
LED.off()
pwm.duty_u16(40000) 
