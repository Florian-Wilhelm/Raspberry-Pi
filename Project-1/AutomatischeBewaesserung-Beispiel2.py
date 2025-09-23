# "quick&dirty" Code für ein TEILautomatisches Bewässerungsprogramm ¯\_(ツ)_/¯
import umrechnung
import RPi.GPIO as GPIO
import time
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
GPIO.setmode(GPIO.BCM)
# Variables
s=0.0
moistureLevel1 = 0.0
moistureLevel2 = 0.0
SensorVoltage1 = 0.0
SensorVoltage2 = 0.0
opVoltage = 0.0
pumpMeasurementVoltage = 0.0
# Shell-Menu
print("*** Plant Watering via Raspberry Pi ***")
print("** Manual program **")
print("")
print("* Start *")
print("")
print("Please choose:")
print("(1) Soil moisture measurement")
print("(2) Plant watering (0-60s)")
print("(3) Abort")
select=int(input("Your choice: "))
print("")
if select == 1:    
    print("Please take care of proper positioning of the sensors!")
    print("0% = dry soil (0.0V)")
    print("100% = wet soil (1.7V)")
    print("")    
    # Sensor 1
    GPIO.setup(27, GPIO.OUT)
    GPIO.output(27, GPIO.HIGH)
    time.sleep(1)
    chan0 = AnalogIn(ads, ADS.P0)
    SensorVoltage1 = chan0.voltage
    print("Moisture Sensor 1 reads (raw value): ","{:>1.4f}".format(SensorVoltage1), "V")    
    moistureLevel1 = umrechnung.prozent(SensorVoltage1)    
    print("Moisture Level 1: ","{:>5.2f}".format(moistureLevel1), "%")   
    time.sleep(1)
    GPIO.output(27, GPIO.LOW)
    time.sleep(0.5)
    # Sensor 2
    GPIO.setup(12, GPIO.OUT)
    GPIO.output(12, GPIO.HIGH)
    time.sleep(1)
    chan1 = AnalogIn(ads, ADS.P1)
    SensorVoltage2 = chan1.voltage
    print("Moisture Sensor 2 reads (raw value): ","{:>1.4f}".format(SensorVoltage2), "V")
    moistureLevel2 = umrechnung.prozent(SensorVoltage2)    
    print("Moisture Level 2: ","{:>5.1f}".format(moistureLevel2), "%")
    time.sleep(1)
    GPIO.output(12, GPIO.LOW)
    time.sleep(0.5)   
    print("")
    print("* Finished *")
elif select == 2:
    GPIO.setup(5, GPIO.OUT) 
    GPIO.output(5, GPIO.HIGH)    
    zeit=int(input("Please choose time (0-60s): "))   
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
        pumpMeasurementVoltage = chan2.voltage # Pumpen-"Shunt" (normalerweise nur im Versuchsaufbau verwendet; wenn A2 nicht verbunden ist wird irgendein Wert ausgegeben)
        print(">Pump shunt reads: ","{:>1.3f}".format(pumpMeasurementVoltage), "V")
        try:
           while s<zeit:
               time.sleep(1)
               s=s+1.0
               print("Time elapsed:", s, "sec")               
        except KeyboardInterrupt:
            print("Interrupted!")
        GPIO.output(5, GPIO.HIGH)
        print("Driver reset (Pin 29 on 3.3V)")
        print("")
        print("* Finished *")
    elif zeit>=60:
        print("For safety reasons, only times <60s are allowed")
        print("Aborted")
    else:
        print("Input invalid. Aborted")
elif select == 3:
    print("Aborted")
else:
    print("Input invalid. Aborted")
