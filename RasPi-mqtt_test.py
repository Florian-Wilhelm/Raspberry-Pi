import Adafruit_BMP.BMP280 as BMP280
import time
import board
import busio

i2c = busio.I2C(board.SCL, board.SDA)
sensor = BMP280.BMP280()

import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

client=mqtt.Client()
client.on_connect = on_connect

username="florian"
password="12345"
client.username_pw_set(username,password)

client.connect("localhost", 1883, 60)

while True:   
   temperature = sensor.read_temperature()
   data1 = "{0:0.2f}".format(temperature)
   client.publish("BMP280/temperature", data1)
   time.sleep(5)
   pressure = sensor.read_pressure()
   data2 = "{0:0.2f}".format(pressure)
   client.publish("BMP280/pressure", data2)
   time.sleep(5)