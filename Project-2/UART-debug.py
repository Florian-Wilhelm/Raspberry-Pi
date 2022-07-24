# das Debug-Programm zu "IoT_Pico-ME110.c"; extra Pico notwendig (Massen verbinden nicht vergessen)
import os
import utime
import machine
from machine import Pin

#print sys info
#print(os.uname())

# bei jedem Sendebeginn auf UART TX wird das Signal auf 0 gezogen -> wird ausgenutzt zur Triggerung der Ausgabe (elektrische Verbindung notwendig, GP15 willkürlich)
trigger = Pin(15, Pin.IN)

# visuelle Indikation dass das Programm läuft 
led_onboard = machine.Pin(25, machine.Pin.OUT)
led_onboard.value(0)
utime.sleep(0.5)
led_onboard.value(1)

uart0 = machine.UART(0)  # UART0: GP0 und GP1 (hier nur RX, also GP1 notwendig)
print(uart0)

utime.sleep(0.1)

while True:
  if trigger.value() == 0:     
    while uart0.any():  # zählt herunter bis keine Zeichen mehr zur Ausgabe vorhanden sind    
      print(uart0.read(1))  # # lies 1 character
    
    print()    
    
