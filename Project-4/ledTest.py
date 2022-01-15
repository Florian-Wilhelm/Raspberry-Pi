# Test-Script zur Ansteuerung zweier LEDs via PWM
from machine import Pin, PWM
import utime
pwm0 = PWM(Pin(0)) # GP0
pwm0.freq(1000) 
pwm1 = PWM(Pin(1)) # GP1
pwm1.freq(1000) 
# LED1 wird heller, LED2 wird dunkler
def ramp(dutyCycleBin, puls):
   pwm0.duty_u16(dutyCycleBin)
   pwm1.duty_u16(groesstwert-dutyCycleBin)
   utime.sleep(puls)
# Größtwert PWM (unveränderlich)
groesstwert = pow(2, 16) # 2^16 = 65536
# Startwert (bitte nicht ändern)
dutyCycleBin = 0
# Zeit die ein PWM-Wert erhalten bleibt
puls = 0.003
# Durchlauf
inkrement = pow(2, 8) # muss 2er Potenz sein
while True:
    dutyCycleBin += inkrement            
    # print("Duty Cycle bin: ", dutyCycleBin)    
    ramp(dutyCycleBin, puls)
    if dutyCycleBin == groesstwert:
        while dutyCycleBin > 0:          
          dutyCycleBin -= inkrement          
          # print("Duty Cycle bin: ", dutyCycleBin)    
          ramp(dutyCycleBin, puls)