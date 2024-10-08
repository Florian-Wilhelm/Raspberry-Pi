# Raspberry Pi Pico project: https://ome-eng.net/raspi-pico-luftfeuchte
# Air moisture measurement and display on two "multiplexed" 7-Segment-Displays
# Additional feature: temperature compensation with temperature sensor 103AT2 (10k NTC resistor)
# ###
from machine import Pin
import utime
ADC_A0 = machine.ADC(26)
ADC_A2 = machine.ADC(28)
# Driving the 7-Segment-Displays, using only one CD4511BE IC BCD-to-7-Segment Latch Decoder
A0 = Pin(13, Pin.OUT) # A: Pin 7 
B0 = Pin(10, Pin.OUT) # B: Pin 1    
C0 = Pin(11, Pin.OUT) # C: Pin 2    
D0 = Pin(12, Pin.OUT) # D: Pin 6
userLED = Pin(25, Pin.OUT) # LED signals whether we are below temperature threshold 1, or above temperature threshold 2
tempCompPin = Pin(22, Pin.IN, Pin.PULL_DOWN) # HW switch temperature compensation in effect yes/no
# A2/B2 used for switching between the two 7-Segment-Displays (necessary additional electronic: 2x BC337 transistor (or similar), 2x 1k base electrode resistor)
A2 = Pin(5, Pin.OUT) 
B2 = Pin(2, Pin.OUT)
def switchA2():
   A2.high()
   B2.low()
def switchB2():
   A2.low()
   B2.high()
sample_hold = 0 # initial value 
mx_time = 0.004 # multiplexing time; must be low enough to deceive a human eye
ExpandedSensorValueS = ADC_A0.read_u16() # the variable name is just convenience her, there is nothing expanded of course
while True:
    # Temperature sensor 103AT2
    SensorValueT = ADC_A2.read_u16()
    zwErg = (3.3/65536)*SensorValueT
    readTempSensor = 0.7*pow(zwErg,4)-7.8*pow(zwErg,3)+32.5*pow(zwErg,2)-76.5*zwErg+70.6 # interpolation formula with datasheet values 
    # print("Temperature: ",  readTempSensor)
    # ###
    # Moisture sensor HIH 4020-001
    # ADC Reading plus "Sample&Hold"; that is supposed to suppress flickering and unsteady effects on the displays        
    if sample_hold>0.20: # new read
       # see datasheet sensor: Vout=(5V*(0.0062*RH(%) + 0.16); Vout_max = 3.9V (100% RH); Vout_min = 0.8V (0% RH)
       # voltage divider 20k, 100k in order to generate Vout_max = 3.26V and Vout_min = 0.66V
       # without temperature compensation
       if (tempCompPin.value() == 1): # so when you connect GP22 with 3.3V, you'll have no temperature compensation
          ExpandedSensorValueS = int(((ADC_A0.read_u16()-13000)/2.6)*3.3) # offset "-13000" (=0.66V) to get 0 for 0% RH; then expansion of the measurement range  
       # with temperature compensation
       if (tempCompPin.value() == 0): # default
          ExpandedSensorValueS = ((ADC_A0.read_u16()-13000)/2.6)*3.3 # calculation for 25°C
          ExpandedSensorValueS = int(ExpandedSensorValueS/(1.0546-0.00216*readTempSensor)) # compensate the whole construct (linear formula)
       # Suppressing values outside the allowed measurement range (voltage peaks etc.)
       if ExpandedSensorValueS < 0:
          ExpandedSensorValueS = 0
       if ExpandedSensorValueS > 65535:
          ExpandedSensorValueS = 65535 
       sample_hold = 0       
    else: # old value is maintained
       sample_hold = sample_hold+2*mx_time   
    # userLED as indicator for a temperature exceeding fixed thresholds
    if ((readTempSensor>30.0) or (readTempSensor<0.0)):
       userLED.on()
    else:
       userLED.off()
    # Driving 7-Segment-Displays, ranges equidistant;
    # there exist far more elegant algorithms to set the GPIOs high/low (using the Single-cycle IO block (SIO)), what you see below is quite lengthy (and not scalable to further digits anyway).
    # 0-9
    if ((ExpandedSensorValueS>=0) and (ExpandedSensorValueS<=655)):
      switchA2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=656) and (ExpandedSensorValueS<=1310)):      
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=1311) and (ExpandedSensorValueS<=1965)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=1966) and (ExpandedSensorValueS<=2620)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=2621) and (ExpandedSensorValueS<=3275)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=3276) and (ExpandedSensorValueS<=3930)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=3931) and (ExpandedSensorValueS<=4585)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=4586) and (ExpandedSensorValueS<=5240)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=5241) and (ExpandedSensorValueS<=5895)):
      switchA2()
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=5896) and (ExpandedSensorValueS<=6550)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
    # 10-19
    if ((ExpandedSensorValueS>=6551) and (ExpandedSensorValueS<=7205)): 
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=7206) and (ExpandedSensorValueS<=7860)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=7861) and (ExpandedSensorValueS<=8515)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=8516) and (ExpandedSensorValueS<=9170)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=9171) and (ExpandedSensorValueS<=9825)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=9826) and (ExpandedSensorValueS<=10480)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=10481) and (ExpandedSensorValueS<=11135)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=11136) and (ExpandedSensorValueS<=11790)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=11791) and (ExpandedSensorValueS<=12445)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=12446) and (ExpandedSensorValueS<=13100)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.low()
    # 20-29
    if ((ExpandedSensorValueS>=13101) and (ExpandedSensorValueS<=13755)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=13756) and (ExpandedSensorValueS<=14410)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=14411) and (ExpandedSensorValueS<=15065)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=15066) and (ExpandedSensorValueS<=15720)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=15721) and (ExpandedSensorValueS<=16375)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=16376) and (ExpandedSensorValueS<=17030)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=17031) and (ExpandedSensorValueS<=17685)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=17686) and (ExpandedSensorValueS<=18340)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=18341) and (ExpandedSensorValueS<=18995)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=18996) and (ExpandedSensorValueS<=19650)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.low()      
      D0.low()
    # 30-39
    if ((ExpandedSensorValueS>=19651) and (ExpandedSensorValueS<=20305)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=20306) and (ExpandedSensorValueS<=20960)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=20961) and (ExpandedSensorValueS<=21615)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=21616) and (ExpandedSensorValueS<=22270)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=22271) and (ExpandedSensorValueS<=22925)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=22926) and (ExpandedSensorValueS<=23580)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=23581) and (ExpandedSensorValueS<=24235)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=24236) and (ExpandedSensorValueS<=24890)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=24891) and (ExpandedSensorValueS<=25545)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    if ((ExpandedSensorValueS>=25546) and (ExpandedSensorValueS<=26200)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.low()      
      D0.low()
    # 40-49
    if ((ExpandedSensorValueS>=26201) and (ExpandedSensorValueS<=26855)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=26856) and (ExpandedSensorValueS<=27510)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=27511) and (ExpandedSensorValueS<=28165)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=28166) and (ExpandedSensorValueS<=28820)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=28821) and (ExpandedSensorValueS<=29475)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=29476) and (ExpandedSensorValueS<=30130)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=30131) and (ExpandedSensorValueS<=30785)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=30786) and (ExpandedSensorValueS<=31440)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=31441) and (ExpandedSensorValueS<=32095)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=32096) and (ExpandedSensorValueS<=32750)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.high()      
      D0.low()
    # 50-59
    if ((ExpandedSensorValueS>=32751) and (ExpandedSensorValueS<=33405)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=33406) and (ExpandedSensorValueS<=34060)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=34061) and (ExpandedSensorValueS<=34715)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=34716) and (ExpandedSensorValueS<=35370)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=35371) and (ExpandedSensorValueS<=36025)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=36026) and (ExpandedSensorValueS<=36680)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=36681) and (ExpandedSensorValueS<=37335)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=37336) and (ExpandedSensorValueS<=37990)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=37991) and (ExpandedSensorValueS<=38645)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=38646) and (ExpandedSensorValueS<=39300)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.high()      
      D0.low()
    # 60-69
    if ((ExpandedSensorValueS>=39301) and (ExpandedSensorValueS<=39955)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=39956) and (ExpandedSensorValueS<=40610)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=40611) and (ExpandedSensorValueS<=41265)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=41266) and (ExpandedSensorValueS<=41920)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=41921) and (ExpandedSensorValueS<=42575)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=42576) and (ExpandedSensorValueS<=43230)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=43231) and (ExpandedSensorValueS<=43885)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=43886) and (ExpandedSensorValueS<=44540)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=44541) and (ExpandedSensorValueS<=45195)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=45196) and (ExpandedSensorValueS<=45850)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.high()      
      C0.high()      
      D0.low()
    # 70-79
    if ((ExpandedSensorValueS>=45851) and (ExpandedSensorValueS<=46505)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=46506) and (ExpandedSensorValueS<=47160)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=47161) and (ExpandedSensorValueS<=47815)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=47816) and (ExpandedSensorValueS<=48470)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=48471) and (ExpandedSensorValueS<=49125)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=49126) and (ExpandedSensorValueS<=49780)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=49781) and (ExpandedSensorValueS<=50435)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=50436) and (ExpandedSensorValueS<=51090)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=51091) and (ExpandedSensorValueS<=51745)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    if ((ExpandedSensorValueS>=51746) and (ExpandedSensorValueS<=52400)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.high()      
      C0.high()      
      D0.low()
    # 80-89
    if ((ExpandedSensorValueS>=52401) and (ExpandedSensorValueS<=53055)):
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=53056) and (ExpandedSensorValueS<=53710)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=53711) and (ExpandedSensorValueS<=54365)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=54366) and (ExpandedSensorValueS<=55020)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=55021) and (ExpandedSensorValueS<=55675)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=55676) and (ExpandedSensorValueS<=56330)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=56331) and (ExpandedSensorValueS<=56985)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=56986) and (ExpandedSensorValueS<=57640)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=57641) and (ExpandedSensorValueS<=58295)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=58296) and (ExpandedSensorValueS<=58950)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.low()      
      B0.low()      
      C0.low()      
      D0.high()
    # 90-99
    if ((ExpandedSensorValueS>=58951) and (ExpandedSensorValueS<=59605)): 
      switchA2()    
      A0.low()      
      B0.low()      
      C0.low()      
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=59606) and (ExpandedSensorValueS<=60260)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=60261) and (ExpandedSensorValueS<=60915)):
      switchA2()
      A0.low()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=60916) and (ExpandedSensorValueS<=61570)):
      switchA2()
      A0.high()
      B0.high()
      C0.low()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=61571) and (ExpandedSensorValueS<=62225)):
      switchA2()
      A0.low()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=62226) and (ExpandedSensorValueS<=62880)):
      switchA2()
      A0.high()
      B0.low()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=62881) and (ExpandedSensorValueS<=63535)):
      switchA2()
      A0.low()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=63536) and (ExpandedSensorValueS<=64190)):
      switchA2()
      A0.high()
      B0.high()
      C0.high()
      D0.low()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=64191) and (ExpandedSensorValueS<=64845)):
      switchA2()
      A0.low()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()
    if ((ExpandedSensorValueS>=64846) and (ExpandedSensorValueS<=65535)):
      switchA2()
      A0.high()
      B0.low()
      C0.low()
      D0.high()
      utime.sleep(mx_time)
      # MX     
      switchB2()
      A0.high()      
      B0.low()      
      C0.low()      
      D0.high()         
    utime.sleep(mx_time)
