# Demonstration how to write several Pico GPIO ports at once by using the Single-cycle IO block (SIO);
# See figure 2 and chapter 2.3.1 in the RP2040 datasheet;
# "The SIO is connected to the single-cycle IOPORT bus of each processor, and provides GPIO access, two-way communications, and other core-local peripherals".

from machine import Pin, mem8 # mem8 = 8bit; mem16, mem32 also possible (RP2040 "fills" missing digits with zeros then) 
import utime

# Switching time for the displayed values
switch_time = 1

# Using a CD4511BE BCD-to-7-Segment Latch Decoder
# A1 = Pin(5, Pin.OUT) # A: Pin 7 
# B1 = Pin(2, Pin.OUT) # B: Pin 1    
# C1 = Pin(3, Pin.OUT) # C: Pin 2    
# D1 = Pin(4, Pin.OUT) # D: Pin 6

# -- 1267 --  Pins of CD4511BE
# -- BCDA --  BCD (Binary Coded Decimal) input for CD4511BE
# 01 2345 67  GPIO Number (0,1,6,7 n.c.)
# 00 0000 00  0x00 0
# 00 0001 00  0x20 1
# 00 1000 00  0x04 2
# 00 1001 00  0x24 3
# 00 0100 00  0x08 4
# 00 0101 00  0x28 5
# 00 1100 00  0x0C 6
# 00 1101 00  0x2C 7
# 00 0010 00  0x10 8
# 00 0011 00  0x30 9
#
# Important note: Reading from right (MSB) to left (LSB) to formulate the hex number from the binary code

def gpio_clr():
    utime.sleep(switch_time)
    mem8[0xd0000018] = 0xff # GPIO_OUT_CLEAR

while True:    
   # GPIO_OUT_SET, counting from 0 to 9 
   mem8[0xd0000014] = 0x00
   gpio_clr()
   mem8[0xd0000014] = 0x20
   gpio_clr()
   mem8[0xd0000014] = 0x04
   gpio_clr()
   mem8[0xd0000014] = 0x24
   gpio_clr()
   mem8[0xd0000014] = 0x08
   gpio_clr()
   mem8[0xd0000014] = 0x28
   gpio_clr()
   mem8[0xd0000014] = 0x0C
   gpio_clr()
   mem8[0xd0000014] = 0x2C
   gpio_clr()
   mem8[0xd0000014] = 0x10
   gpio_clr()
   mem8[0xd0000014] = 0x30
   gpio_clr()
