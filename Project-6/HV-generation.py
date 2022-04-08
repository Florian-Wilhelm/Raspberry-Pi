from machine import Pin, PWM
import utime
pwm = PWM(Pin(13))  # GPIO13 bzw. GP13; an sich frei wählbar
pwm.freq(1250)  # PWM-Frequenz
pwm.duty_u16(51000)  # Tastgrad, 16bit Wert (0-65.535)