##########################################
### Manuelle Bewässerung mit dem RasPi ###
##########################################
#
# Ersteller: Florian Wilhelm Dirnberger
#
import umrechnung
import RPi.GPIO as GPIO
import time
import board
import busio
import adafruit_ads1x15.ads1115 as ADS 
from adafruit_ads1x15.analog_in import AnalogIn
# Create the I2C bus
i2c = busio.I2C(board.SCL, board.SDA)
# Create the ADC object using the I2C bus
ads = ADS.ADS1115(i2c)
# GPIO setmode
GPIO.setmode(GPIO.BCM)
# Variables
s=0.0
i=1
auswahl=0
moistureLevel_1 = 0.0
moistureLevel_2 = 0.0
voltage1 = 0.0
voltage2 = 0.0
pumpenMessSpannung = 0.0
# Startmenü
print("*** Plant Watering via Raspberry Pi ***")
print("** Manual program **")
print("")
print("* Start *")
print("")
print("Please choose:")
print("(1) Soil moisture measurement")
print("(2) Plant watering (0-60s)")
print("(3) Abort")
auswahl=int(input("Your choice: "))
print("")
auswahl, type(auswahl)
# Programm
if auswahl == 1:    
    print("Please take care of proper positioning of the sensors!")
    print("0% = dry soil (0.0V)")
    print("100% = wet soil (1.7V)")
    print("")
    # Zeitlicher Versatz der Sensorabfrage um Ausgänge weniger zu belasten
    # Sensor 1
    GPIO.setup(27, GPIO.OUT)
    GPIO.output(27, GPIO.HIGH)
    time.sleep(1)
    chan0 = AnalogIn(ads, ADS.P0)
    voltage1 = chan0.voltage
    print("Moisture Sensor 1 reads (raw value): ","{:>1.4f}".format(voltage1), "V")    
    moistureLevel_1 = umrechnung.prozent(voltage1)
    # moistureLevel_1 = ((voltage1/1.7)*100)
    print("Moisture Level 1: ","{:>5.2f}".format(moistureLevel_1), "%") # :> align right    
    time.sleep(1)
    GPIO.output(27, GPIO.LOW)
    time.sleep(1)
    # Sensor 2
    GPIO.setup(12, GPIO.OUT)
    GPIO.output(12, GPIO.HIGH)
    time.sleep(1)
    chan1 = AnalogIn(ads, ADS.P1)
    voltage2 = chan1.voltage
    print("Moisture Sensor 2 reads (raw value): ","{:>1.4f}".format(voltage2), "V")
    # moistureLevel_2 = (chan1.voltage/1.7)*100
    moistureLevel_2 = umrechnung.prozent(voltage2)
    print("Moisture Level 2: ","{:>5.1f}".format(moistureLevel_2), "%")
    time.sleep(1)
    GPIO.output(12, GPIO.LOW)
    time.sleep(1)   
    print("")
    print("* Finished *")
elif auswahl == 2:
    GPIO.setup(5, GPIO.OUT) 
    GPIO.output(5, GPIO.HIGH)    
    zeit=int(input("Please choose time (0-60s): "))
    zeit, type(zeit)
    if zeit<60:
        print("Program will activate driver for", zeit, "seconds")
        print("")
        GPIO.output(5, GPIO.LOW)
        print("Driver set (Pin 29 on 0.0V)")
        time.sleep(0.5)
        chan3 = AnalogIn(ads, ADS.P3)
        opVoltage = chan3.voltage
        print(">Non-inverting OP input reads: ","{:>1.3f}".format(opVoltage), "V")        
        time.sleep(0.5)
        chan2 = AnalogIn(ads, ADS.P2)
        pumpenMessSpannung = chan2.voltage # "shunt"
        print(">Pump shunt reads: ","{:>1.3f}".format(pumpenMessSpannung), "V")
        try:
           while i<=zeit:
               time.sleep(1)
               s=s+1.0
               print("Time elapsed:", s, "sec")
               i=i+1
        except KeyboardInterrupt:
            print("Interrupted!")
        GPIO.output(5, GPIO.HIGH)
        print("Driver reset (Pin 29 on 3.3V)")
        print("")
        print("* Finished *")
    elif zeit>=60:
        print("For safety purposes, only times<60s are allowed")
        print("Aborted")
    else:
        print("Input invalid. Aborted")
elif auswahl == 3:
    print("Aborted")
else:
    print("Input invalid. Aborted")
