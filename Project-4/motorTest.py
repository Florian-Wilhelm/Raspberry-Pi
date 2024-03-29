# Sequenz erhöht schrittweise PWM-Werte (gedacht für Präsentationszwecke, Tests etc.; kein produktiver Code für einen Anwendungsfall).
from machine import Pin, PWM
import utime
pwm = PWM(Pin(1)) # GPIO 1, an sich frei wählbar
pwm.freq(1000) # PWM-Frequenz
def ramp(dutyCycleBin, sleeping):
   pwm.duty_u16(dutyCycleBin)
   utime.sleep(sleeping)
# Größtwert PWM + 1 (unveränderlich)
groesstwert = pow(2, 16) # 2^16 = 65536; Bezeichnung eigentlich irreführend da 65535 der größte erlaubte PWM-Wert ist
# Inkrement 
teiler = pow(2, 3) # Exponent kann geändert werden
inkrement = int(groesstwert/teiler) # Ergebnis immer ganzzahlig und Potenz von 2
# Anlaufimpuls (empirische Werte)
anlaufdutyCycle = int(groesstwert/4)
anlaufImpulsZeit = 0.1
ramp(anlaufdutyCycle, anlaufImpulsZeit)
# Startwerte (bitte nicht ändern)
i = 1
dutyCycleBin = 0
# Laufzeit jedes PWM-Schrittes in Sekunden (kann geändert werden)
laufzeit = 10
# Durchlauf
durchlaufZahl = 4 # so oft wird inkrementiert (es könnte auch direkt der "teiler" Wert verwendet werden)
while i<=durchlaufZahl:
    print("Sequenz", i, "von", durchlaufZahl)
    dutyCycleBin = dutyCycleBin + inkrement
    dutyCyclePercent = 100*(dutyCycleBin/(groesstwert-1))
    if dutyCyclePercent >= 100: # 100% und größer ist verriegelt
      print("Wert >=100%, abgebrochen")  
      break        
    print("Tastgrad Rohwert: ", dutyCycleBin)
    print("Tastgrad Prozent: ", dutyCyclePercent, " %")
    ramp(dutyCycleBin, laufzeit)
    i += 1
# Abschaltung des Motors
ramp(0, 1)
print("Ende")
