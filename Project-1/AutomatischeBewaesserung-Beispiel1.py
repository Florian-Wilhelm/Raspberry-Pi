import schattenspeicher
import Adafruit_BMP.BMP280 as BMP280
import RPi.GPIO as GPIO
import time
import datetime
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
GPIO.setmode(GPIO.BCM)
tempSensor = BMP280.BMP280()
# ### Initialisierungen
i=0
n=0
zyklusGesamt=0
zyklusWasser=0 # Teilmenge von zyklusGesamt, Zyklen in welchen bewässert wurde
LetzterWasserZyklus=0 # Hilfswert für PlausiCheck
plausiZaehler=0 # Hilfswert für PlausiCheck
PlausiCheck=0
s = 0.0
moistureLevel_1 = 0.0
moistureLevel_2 = 0.0 
opVoltage = 0.0
pumpenMessSpannung = 0.0
# 1s default für sleep-Werte
sleepSensor1 = 1 # Vor Analog-Spannung 1 einlesen
sleepSensor2 = 1 # Vor Sensor-Versorgung 1 auf 0.0V
sleepSensor3 = 1 # Vor Sensor-Versorgung 2 auf 3.3V
sleepSensor4 = 1 # Vor Analog-Spannung 2 einlesen
sleepSensor5 = 1 # Vor Sensor-Versorgung 2 auf 0.0V
sleepSensorGesamt = sleepSensor1+sleepSensor2+sleepSensor3+sleepSensor4+sleepSensor5 
# ### Ende
# ### Empirische Werte
idleZeit=295 # Zeit um den Zyklus aufzufüllen (Monitoring+Bewässern+IdleZeit); 5min = 300s
zyklusZeit=idleZeit+sleepSensorGesamt # "Zyklus Zeit" ist nicht ganz korrekt; ein Zyklus kann bei Bewässerung auch länger dauern, geht aber nicht in die Rechnung ein
moistureThreshold=40.0 # moisture level in % als Schwelle für die Bewässerung (Schätzwert, auch abhängig von Sensorposition und Pflanzenart)
plausiWert=3 # so oft darf unmittelbar hintereinander ein Bewässerungszyklus stattfinden, dannach Programmabbruch (da unplausibel)
bewZeit = 10 # Bewässerungszeit in Sekunden (wenn bewässert wird)
# ### Ende
# ### Allgemeine Programmparameter
zeitStunden = 168 # Gesamtlaufzeit
einstellmodus = 4 # PlausiCheck für die ersten n Zyklen deaktiviert
# ### Ende
zeitSekunden = zeitStunden*3600 # hier bitte nichts verändern!! Zeile dient nur der allgemeinen Lesbarkeit
while i<=zeitSekunden: # oder "while True:", dann endlos (nicht empfehlenswert)
       temperature = tempSensor.read_temperature()            
       # Sensor 1
       GPIO.setup(27, GPIO.OUT)
       GPIO.output(27, GPIO.HIGH)
       time.sleep(sleepSensor1)
       chan0 = AnalogIn(ads, ADS.P0)       
       moistureLevel_1 = (chan0.voltage/1.7)*100            
       time.sleep(sleepSensor2)
       GPIO.output(27, GPIO.LOW)
       time.sleep(sleepSensor3)
       # Sensor 2
       GPIO.setup(12, GPIO.OUT)
       GPIO.output(12, GPIO.HIGH)
       time.sleep(sleepSensor4)
       chan1 = AnalogIn(ads, ADS.P1)       
       moistureLevel_2 = (chan1.voltage/1.7)*100            
       time.sleep(sleepSensor5)
       GPIO.output(12, GPIO.LOW)       
       # Sequenz            
       if moistureLevel_1<moistureThreshold and moistureLevel_2<moistureThreshold:              
         PlausiCheck = zyklusGesamt - LetzterWasserZyklus
         if PlausiCheck == 1 and zyklusGesamt > einstellmodus: # hier wird getestet ob 2 aufeinanderfolgende Zyklen Bewässerung auslösen; in den ersten "einstellmodus" Zyklen inaktiv
            plausiZaehler = plausiZaehler+1
            if plausiZaehler == plausiWert: 
               file = open("logfiles/Plausicheck.txt", "w")
               file.write("!! Auslösung Plausi Check !!\n")
               file.write("{:%d.%m.%Y %H:%M:%S}".format(datetime.datetime.now()))
               file.write("\n") 
               file.close()                    
               break
         else:           
           plausiZaehler = 0 # Rücksetzen des Wertes
         GPIO.setup(5, GPIO.OUT)                            
         GPIO.output(5, GPIO.LOW)              
         time.sleep(0.5)
         chan3 = AnalogIn(ads, ADS.P3) # nichtinvertierender OP-Eingang, zeigt an ob Schwimmerschalter ausgelöst hat (dann auf 0V)
         opVoltage = chan3.voltage
         time.sleep(0.5)
         chan2 = AnalogIn(ads, ADS.P2) # Pumpen-"Shunt" (normalerweise nur im Versuchsaufbau verwendet; wenn A2 nicht verbunden ist wird irgendein Wert ausgegeben)
         pumpenMessSpannung = chan2.voltage 
         while n<bewZeit:
           time.sleep(1)
           s=s+1.0                
           n=n+1
         GPIO.output(5, GPIO.HIGH)
         n = 0
         s = 0.0              
         zyklusWasser=zyklusWasser+1
         LetzterWasserZyklus = zyklusGesamt         
       time.sleep(idleZeit)       
       i=i+zyklusZeit
       zyklusGesamt=zyklusGesamt+1
       # Logging erst nach komplettem Abschluss des Zyklus, Logfile wird jedesmal mit dem Neuesten überschrieben       
       schattenspeicher.erstellen(zeitStunden, zyklusGesamt, i, zyklusWasser, moistureLevel_1, moistureLevel_2, opVoltage, pumpenMessSpannung, sleepSensorGesamt, idleZeit, temperature)                        
