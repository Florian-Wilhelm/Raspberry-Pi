# ad-hoc test script a SD-Card connected to the SPI bus

import machine
import uos
import utime

import sdcard # note: the "sdcard.py" lib has to be uploaded into the Pico file system
from machine import Pin

cs = machine.Pin(17, machine.Pin.OUT)

spi = machine.SPI(0,
                  baudrate=1000000,
                  polarity=0,
                  phase=0, 
                  bits=8,
                  firstbit=machine.SPI.MSB,
                  sck=machine.Pin(18),
                  mosi=machine.Pin(19),
                  miso=machine.Pin(16))

sd = sdcard.SDCard(spi, cs)

vfs = uos.VfsFat(sd)
uos.mount(vfs, "/sd")

button = Pin(10, Pin.IN) # button and 10k pull-down resistor soldered to pin 10 (input GP)

n = 0
fileName = "file"

while True:
    # if button pushed, then pin 10 on 3.3V (i.e. high)
    # i.e. pushing the button will create a new file on the SD-Card
    if button.value()==1:         
     
     fileNumber = n
     
     buf1 = "/sd/%s%s.txt" % (fileName, fileNumber)
     
     buf2 = "Measurement %d\r\n" % (n)
     
     with open(buf1, "w") as file:
       print("writing...")
       file.write(buf2)
       
     n = n+1
     utime.sleep(0.5) # very simple debouncing
