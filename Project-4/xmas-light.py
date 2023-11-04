# Hell/Dunkel-Steuerung mehrerer (externer) LEDs via PWM (Pi Pico GPs)
# Vorwiderstände LEDs: 50-100 Ohm
# Spannungsteilerschaltung für SFH300 Phototransistor: 5k und 10k (Kollektor an +5V, Emitter an 5k), zwischen diesen beiden Widerständen ist der Abgriff für den ADC; der 10k hängt an Masse
from machine import Pin, PWM
import utime
ADC_A2 = machine.ADC(28)
pwm0 = PWM(Pin(16)) 
pwm0.freq(1000) 
pwm1 = PWM(Pin(18)) 
pwm1.freq(1000)
pwm2 = PWM(Pin(21)) 
pwm2.freq(1000)
def ramp(dutyCycleBin, puls):
   if dutyCycleBin == 256: # 256 ausserhalb des erlaubten Bereiches
      dutyCycleBin = 255
   #print("Duty Cycle bin: ", dutyCycleBin) 
   pwm0.duty_u16(dutyCycleBin*dutyCycleBin)
   pwm1.duty_u16(groesstwert-(dutyCycleBin*dutyCycleBin))
   pwm2.duty_u16(dutyCycleBin*dutyCycleBin)
   utime.sleep(puls)
groesstwert = pow(2, 16) # 2^16 = 65536
# Startwert (nicht ändern)
dutyCycleBin = 0
# Zeit (in Sekunden) die ein PWM-Wert erhalten bleibt
puls = 0.008
# Durchlauf
inkrement = 1 
while True:    
    photoTransistorValue = ADC_A2.read_u16()
    if photoTransistorValue < 14000: # Experimentierwert
      dutyCycleBin += inkrement 
      ramp(dutyCycleBin, puls)
      if dutyCycleBin == 256:
          while dutyCycleBin > 0:          
            dutyCycleBin -= inkrement
            ramp(dutyCycleBin, puls)
    if photoTransistorValue > 16000: # Experimentierwert, einfache Hysterese
      pwm0.duty_u16(0)
      pwm1.duty_u16(0)
      pwm2.duty_u16(0)
