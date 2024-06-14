import time
import utime
from L76 import l76x
import math
import hashlib
from L76.micropyGPS.micropyGPS import MicropyGPS
from machine import Pin

import machine
import uos
import sdcard # note: the "sdcard.py"-lib has to be uploaded into the Pico file system

# define the UART number and its baudrate , when UARTx is 1 please solder the UART1 0R resistor on Pico-GPS-L76B board
# UARTx = 1
UARTx = 0

# define the rp2040 uart baudrate , the default baudrate is 9600 of L76B 
BAUDRATE = 9600

# make an object of gnss device , the default uart is UART0 and its baudrate is 9600bps
gnss_l76b=l76x.L76X(uartx=UARTx,_baudrate = BAUDRATE)

# exit the backup mode when start
gnss_l76b.l76x_exit_backup_mode()

# enable/disable sync PPS when NMEA output
'''
optional:
SET_SYNC_PPS_NMEA_ON
SET_SYNC_PPS_NMEA_OFF
'''
gnss_l76b.l76x_send_command(gnss_l76b.SET_SYNC_PPS_NMEA_ON)


# make an object of NMEA0183 sentence parser
"""
Setup GPS Object Status Flags, Internal Data Registers, etc
local_offset (int): Timzone Difference to UTC
location_formatting (str): Style For Presenting Longitude/Latitude:
                           Decimal Degree Minute (ddm) - 40° 26.767′ N
                           Degrees Minutes Seconds (dms) - 40° 26′ 46″ N
                           Decimal Degrees (dd) - 40.446° N
"""
parser = MicropyGPS(location_formatting='dd')

sentence = ''

# SD-Card connected to SPI bus
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

button = Pin(10, Pin.IN) # push button and 10k pull-down resistor soldered to pin 10 (input GP)

n = 1
fileName = "GPS file"

while True:
  if button.value()==1: 
     if gnss_l76b.uart_any():
        sentence = parser.update(chr(gnss_l76b.uart_receive_byte()[0]))
        if sentence:
            
            print('WGS84 Coordinate:Latitude(%c),Longitude(%c) %.9f,%.9f'%(parser.latitude[1],parser.longitude[1],parser.latitude[0],parser.longitude[0]))
            buf2 = ("Measurement %d, WGS84 Coordinate:Latitude(%c),Longitude(%c) %.9f,%.9f"%(n,parser.latitude[1],parser.longitude[1],parser.latitude[0],parser.longitude[0]))
            print('copy WGS84 coordinates and paste it on Google map web https://www.google.com/maps')

            gnss_l76b.wgs84_to_bd09(parser.longitude[0],parser.latitude[0])
            print('Baidu Coordinate: longitude(%c),latitudes(%c) %.9f,%.9f'%(parser.longitude[1],parser.latitude[1],gnss_l76b.Lon_Baidu,gnss_l76b.Lat_Baidu))
            print('copy Baidu Coordinate and paste it on the baidu map web https://api.map.baidu.com/lbsapi/getpoint/index.html')
            
            print('UTC Timestamp:%d:%d:%d'%(parser.timestamp[0],parser.timestamp[1],parser.timestamp[2]))
            
#           print fix status
            '''
            1 : NO FIX
            2 : FIX 2D
            3 : FIX_3D
            '''
            print('Fix Status:', parser.fix_stat)
            
            print('Altitude:%d m'%(parser.altitude))
            print('Height Above Geoid:', parser.geoid_height)
            print('Horizontal Dilution of Precision:', parser.hdop)
            print('Satellites in Use by Receiver:', parser.satellites_in_use)
            print('')
            
            fileNumber = n
     
            buf1 = "/sd/%s%s.txt" % (fileName, fileNumber)                 
     
            with open(buf1, "w") as file:
             print("writing...")
             file.write(buf2)
       
            n = n+1
            
            utime.sleep(0.5) # very simple debouncing
