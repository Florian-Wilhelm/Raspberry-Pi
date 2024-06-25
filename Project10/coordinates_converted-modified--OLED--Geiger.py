import time
from L76 import l76x
import math
import hashlib
from L76.micropyGPS.micropyGPS import MicropyGPS
import machine

import utime
from machine import Pin
from machine import UART
from machine import I2C
from ssd1306 import SSD1306_I2C # note: SSD1306 packages have to be installed (via thonny: "Tools-->Manage packages..."); might not work with every MicroPython firmware

try:
   i2c = I2C(0,sda=Pin(4),scl=Pin(5),freq=40000)
   oled=SSD1306_I2C(128,64,i2c)
   oled.fill(0)
   oled.show()
   OLED_true = 1
except OSError:
   print("OLED not connected")
   OLED_true = 0

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
   
button = Pin(10, Pin.IN) # push button and 10k pull-down resistor soldered to pin 10 (input GP)

trigger = Pin(16, Pin.IN)
uart1 = UART(1, baudrate=115200, bits=8, parity = 0, stop=1, tx=Pin(8), rx=Pin(9))  # parity = 0 for EVEN parity

while True:
  if trigger.value() == 0:
    print('CPM:')  
    while uart1.any():  # counts down until there are no characters anymore    
      print(uart1.read(1))  # read one character
    
    print()
  elif button.value()==1:
     
     if gnss_l76b.uart_any():
        sentence = parser.update(chr(gnss_l76b.uart_receive_byte()[0]))
        if sentence:
            
            print('WGS84 Coordinate:Latitude(%c),Longitude(%c) %.9f,%.9f'%(parser.latitude[1],parser.longitude[1],parser.latitude[0],parser.longitude[0]))
            buf1 = ("(%c) %.9f"%(parser.latitude[1],parser.latitude[0]))
            buf2 = ("(%c) %.9f"%(parser.longitude[1],parser.longitude[0]))
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
            buf5 = 'Altitude:%d m'%(parser.altitude)
            print('Height Above Geoid:', parser.geoid_height)
            print('Horizontal Dilution of Precision:', parser.hdop)
            print('Satellites in Use by Receiver:', parser.satellites_in_use)
            print('')
            
            oled.fill(0)
            oled.show()            
                        
            if OLED_true == 1:
              oled.text(buf5,0,10)              
              oled.text("Latitude",0,20)
              oled.text(buf1,0,30)
              oled.text("Longitude",0,40)              
              oled.text(buf2,0,50)      
              oled.show()           
            utime.sleep(0.5) # very simple debouncing
