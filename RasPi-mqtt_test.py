# MQTT publishing data (test script)

import Adafruit_BMP.BMP280 as BMP280
import time
import board
import busio

i2c = busio.I2C(board.SCL, board.SDA)
sensor = BMP280.BMP280()

import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc): # callback function
    print("Connected with result code " + str(rc))

client=mqtt.Client()
client.on_connect = on_connect

username="florian"
password="12345"
client.username_pw_set(username,password)

client.connect("localhost", 1883, 60)

while True:   
   temperature = sensor.read_temperature()
   data1 = "{0:0.1f}".format(temperature)
   client.publish("BMP280/Temperature (°C)", data1)
   time.sleep(5)
   pressure = sensor.read_pressure()
   data2 = "{0:0.0f}".format(pressure)
   client.publish("BMP280/Pressure (Pa)", data2)
   time.sleep(5)
