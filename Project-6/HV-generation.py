from machine import Pin, PWM
import utime
pwm = PWM(Pin(13))  # pick your GPIO
pwm.freq(1250)  # PWM-frequency (empirical)
pwm.duty_u16(55000)  # duty cycle (empirical), 16bit (0-65535)
