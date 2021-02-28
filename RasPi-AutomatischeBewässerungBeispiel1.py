import schattenspeicher
import RPi.GPIO as GPIO
import time
import datetime
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
# Create the I2C bus
i2c = busio.I2C(board.SCL, board.SDA)
# Create the ADC object using the I2C bus
ads = ADS.ADS1115(i2c)
GPIO.setmode(GPIO.BCM)
# ### Initialisierung
i=0
n=0
zyklusGesamt=0
zyklusWasser=0 # Zyklen wie oft schon bewässert wurde
check1=0
check2=0
plausi = 0
s = 0.0
moistureLevel_1 = 0.0
moistureLevel_2 = 0.0 
opVoltage = 0.0
pumpenMessSpannung = 0.0
# ### Ende
# ### Empirische Werte
idleZeit=295 # Zeit um den Zyklus aufzufüllen (Monitoring+Bewässern+IdleTime); 5min = 300s
zyklusZeit=idleZeit+5 # 5s ist die Monitoringzeit für die Sensoren (sollte auch nicht geändert werden, siehe code weiter unten); "Zyklus Zeit" ist nicht ganz korrekt; ein Zyklus kann bei Bewässerung auch länger dauern, geht aber nicht in die Rechnung ein
level=40.0 # moisture level in % als Schwelle für die Bewässerung (eher Schätzeisen, muss empirisch ermittelt werden)
plausiwert = 3 # damit muss noch experimentiert werden
# ### Ende
# ### Hier bitte je nach Bedarf verändern!
zeitStunden = 24 # Gesamtlaufzeit
einstellmodus = 4 # plausi check für die ersten n Zyklen deaktiviert
bewZeit=10 # Bewässerungszeit in Sekunden (wenn bewässert wird)
# ### Ende
zeitSekunden = zeitStunden*3600 # hier bitte nichts verändern!!
while i<=zeitSekunden:            
            # Sensor 1
            GPIO.setup(27, GPIO.OUT)
            GPIO.output(27, GPIO.HIGH)
            time.sleep(1)
            chan0 = AnalogIn(ads, ADS.P0)
            # print("Moisture Sensor 1 reads (raw value): ","{:>5.3f}".format(chan0.voltage), "V")
            moistureLevel_1 = (chan0.voltage/1.7)*100            
            time.sleep(1)
            GPIO.output(27, GPIO.LOW)
            time.sleep(1)
            # Sensor 2
            GPIO.setup(12, GPIO.OUT)
            GPIO.output(12, GPIO.HIGH)
            time.sleep(1)
            chan1 = AnalogIn(ads, ADS.P1)
            # print("Moisture Sensor 2 reads (raw value): ","{:>5.3f}".format(chan1.voltage), "V")
            moistureLevel_2 = (chan1.voltage/1.7)*100            
            time.sleep(1)
            GPIO.output(12, GPIO.LOW)
            # chan3 = AnalogIn(ads, ADS.P3) # OP-Eingang
            # Sequenz            
            if moistureLevel_1<level and moistureLevel_2<level:              
              check2 = zyklusGesamt - check1 # hier bestünde generell das "Problem" beim Spezialfall wenn im 1. Zyklus bewässert wird; dann reicht ein einziger Durchlauf für den Plausi Check
              if check2 == 1 and zyklusGesamt > einstellmodus: # hier wird getestet ob 2 aufeinanderfolgende Zyklen Bewässerung auslösen; in den ersten "einstellmodus" Zyklen deaktiviert
                 plausi = plausi+1
                 if plausi == plausiwert: 
                    file = open("logfiles/Plausicheck.txt", "w")
                    file.write("!! Auslösung Plausi Check !!\n")
                    file.write("{:%d.%m.%Y %H:%M:%S}".format(datetime.datetime.now()))
                    file.write("\n") 
                    file.close()                    
                    break
              else:
                check1 = 0 # glaub das brauch ich garnicht, wird ohnehin überschrieben
                plausi = 0 # Rücksetzen des Wertes (nur für Folge "Über Schwelle", "Über Schwelle", "Unter Schwelle" relevant; ansonsten sowieso auf 1)
              GPIO.setup(5, GPIO.OUT)                            
              GPIO.output(5, GPIO.LOW)              
              time.sleep(0.5)
              chan3 = AnalogIn(ads, ADS.P3) # nichtinvertierender OP-Eingang
              opVoltage = chan3.voltage
              time.sleep(0.5)
              chan2 = AnalogIn(ads, ADS.P2)
              pumpenMessSpannung = chan2.voltage # "shunt"
              while n<bewZeit:
                time.sleep(1)
                s=s+1.0                
                n=n+1
              GPIO.output(5, GPIO.HIGH)
              n = 0
              s = 0.0              
              zyklusWasser=zyklusWasser+1
              check1 = zyklusGesamt              
            # else:     
            time.sleep(idleZeit)             
            i=i+zyklusZeit
            zyklusGesamt=zyklusGesamt+1                       
            schattenspeicher.erstellen(zeitStunden, zyklusGesamt, i, zyklusWasser, moistureLevel_1, moistureLevel_2, opVoltage, pumpenMessSpannung)                        
# print("")
