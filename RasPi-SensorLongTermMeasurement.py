import xlsxwriter
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
moistureLevel1_percent = 0.0
moistureLevel2_percent = 0.0
sleepZeitSekunden = 3600 # 1h = 3600s
LaufzeitWert = 24 
gesamtZeitSekunden = LaufzeitWert*sleepZeitSekunden 
i=0
n=0
tempSensor = BMP280.BMP280()
workbook = xlsxwriter.Workbook("LTM.xlsx")
worksheet = workbook.add_worksheet()
while i<gesamtZeitSekunden:
       # Temperaturmessung
       temperature = tempSensor.read_temperature()   
       # Sensor 1
       GPIO.setup(27, GPIO.OUT)
       GPIO.output(27, GPIO.HIGH)
       time.sleep(0.5)
       chan0 = AnalogIn(ads, ADS.P0)
       moistureLevel1_percent = (chan0.voltage/1.7)*100
       time.sleep(0.1)
       GPIO.output(27, GPIO.LOW)
       time.sleep(0.1)
       # Sensor 2
       GPIO.setup(12, GPIO.OUT)
       GPIO.output(12, GPIO.HIGH)
       time.sleep(0.5)
       chan1 = AnalogIn(ads, ADS.P1)
       moistureLevel2_percent = (chan1.voltage/1.7)*100
       time.sleep(0.1)
       GPIO.output(12, GPIO.LOW)
       # Writing       
       worksheet.write(n,0, moistureLevel1_percent)
       worksheet.write(n,1, moistureLevel2_percent)
       worksheet.write(n,2, temperature)
       n = n+1
       i=i+sleepZeitSekunden
       time.sleep(sleepZeitSekunden)
workbook.close()