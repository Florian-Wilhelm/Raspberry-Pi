# Ein Debug-Programm zu "IoT_Pico--HW390-GP26--103AT2-GP27.c"; extra Pico notwendig (Massen verbinden nicht vergessen);
# nur ein workaround, geht auch anders.
import os
import utime
import machine
from machine import Pin

#print sys info
#print(os.uname())

# bei jedem Sendebeginn auf UART TX wird das Signal auf 0 gezogen -> wird ausgenutzt zur Triggerung der Ausgabe (extra elektrische Verbindung notwendig, GP15 willkürlich)
trigger = Pin(15, Pin.IN)

# visuelle Indikation dass das Programm läuft 
led_onboard = machine.Pin(25, machine.Pin.OUT)
led_onboard.value(0)
utime.sleep(1)
led_onboard.value(1)

uart0 = machine.UART(0)  # UART0: GP0 und GP1 (hier nur RX, also GP1 notwendig)
print(uart0)

utime.sleep(0.1)

while True:
  if trigger.value() == 0:     
    while uart0.any():  # zählt herunter bis keine Zeichen (character) mehr zur Ausgabe vorhanden sind    
      print(uart0.read(1))  # lies ein Zeichen
    
    print()       
