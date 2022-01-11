# Programm erhöht Schritt für Schritt die PWM für einen Motor
from machine import Pin, PWM
import utime
pwm = PWM(Pin(1)) # GPIO 1, an sich frei wählbar
pwm.freq(1000) # PWM-Frequenz
def ramp(dutyCycleBin, sleeping):
   pwm.duty_u16(dutyCycleBin)
   utime.sleep(sleeping)
# Größtwert PWM
groesstwert = pow(2, 16) # 2^16 = 65536
# Inkrement
teiler = pow(2, 3)
inkrement = int(groesstwert/teiler)
# Anlaufimpuls
anlaufdutyCycle = int(groesstwert/4)
anlaufImpulsZeit = 0.1
ramp(anlaufdutyCycle, anlaufImpulsZeit)
# Startwerte (bitte nicht ändern)
i = 1
dutyCycleBin = 0
# Laufzeit jedes PWM-Schrittes (kann geändert werden)
laufzeit = 10
# Durchlauf
durchlaufZahl = 4 # wie oft wird hochgezählt
while i<=durchlaufZahl:
    dutyCycleBin = dutyCycleBin + inkrement
    dutyCyclePercent = 100*(dutyCycleBin/groesstwert)
    if dutyCyclePercent >= 100: # 100% und größer ist verriegelt
        break
    print("Duty Cycle bin: ", dutyCycleBin)
    print("Duty Cycle percent: ", dutyCyclePercent, " %")
    ramp(dutyCycleBin, laufzeit)
    i += 1
# Abschaltung des Motors
ramp(0, 1)
print("Finished")