# ad-hoc test script for the connected SD-Card; yet to be married to the actual measurement script

import machine
import sdcard
import uos

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

with open("/sd/test01.txt", "w") as file:
   file.write("RH test\r\n")
                  