# Raspberry Pi Pico project: https://hackaday.io/project/178522-soil-moisture-measurement-device
# Skript für allererste Bastelboards mit resistivem ME110-Sensor, RGB-LED (plus Vorwiderstände) und einer einzelnen 7-Segment-Anzeige
# ###
from machine import Pin, PWM
#import utime
ADC_A0 = machine.ADC(26) # ADC0 an Pin 26
pwm3 = PWM(Pin(3)) # gewählter Anschlusspin (GPIO) für blaue LED
pwm4 = PWM(Pin(4)) # s.o., rote LED
pwm5 = PWM(Pin(5)) # s.o., grüne LED
pwm3.freq(500) # PWM-Frequenz blaue LED, Wert nicht so wichtig (sollte nur nicht flackern)
pwm4.freq(500) # s.o., rote LED
pwm5.freq(500) # s.o., grüne LED
A = Pin(13, Pin.OUT) # A: Pin 7 CD4511BE IC BCD-to-7-Segment Latch Decoder
B = Pin(10, Pin.OUT) # B: Pin 1    
C = Pin(11, Pin.OUT) # C: Pin 2    
D = Pin(12, Pin.OUT) # D: Pin 6
while True:
    # ADC (Skalierung für ME110-Sensor, siehe Datenblatt)
    ExpandedSensorValueS = int((ADC_A0.read_u16()/1.7)*3.3)
    # Werte ausserhalb des erlaubten Wertebereiches (Spannungsspitzen, fehlerhafte ADC-Wandlung etc.) werden ausgeblendet
    if ExpandedSensorValueS < 0:
      ExpandedSensorValueS = 0
    if ExpandedSensorValueS > 65535:
      ExpandedSensorValueS = 65535
    # RGB-LED Ansteuerung
    duty3 = 0 # blau wird vorerst nicht verwendet
    duty4 = (65535-ExpandedSensorValueS)    
    duty5 = ExpandedSensorValueS    
    pwm3.duty_u16(duty3)
    pwm4.duty_u16(duty4)
    pwm5.duty_u16(duty5)
    # Ansteuerung der 7-Segmentanzeige; sperriger Code, nur für Präsentationszwecke        
    if ((ExpandedSensorValueS>=0) and (ExpandedSensorValueS<=6553)):      
      A.low()      
      B.low()      
      C.low()      
      D.low()
    if ((ExpandedSensorValueS>=6554) and (ExpandedSensorValueS<=13106)):
      A.high()
      B.low()
      C.low()
      D.low()
    if ((ExpandedSensorValueS>=13107) and (ExpandedSensorValueS<=19659)):
      A.low()
      B.high()
      C.low()
      D.low()
    if ((ExpandedSensorValueS>=19660) and (ExpandedSensorValueS<=26212)):
      A.high()
      B.high()
      C.low()
      D.low()
    if ((ExpandedSensorValueS>=26213) and (ExpandedSensorValueS<=32765)):
      A.low()
      B.low()
      C.high()
      D.low()
    if ((ExpandedSensorValueS>=32766) and (ExpandedSensorValueS<=39318)):
      A.high()
      B.low()
      C.high()
      D.low()
    if ((ExpandedSensorValueS>=39319) and (ExpandedSensorValueS<=45871)):
      A.low()
      B.high()
      C.high()
      D.low()
    if ((ExpandedSensorValueS>=45872) and (ExpandedSensorValueS<=52424)):
      A.high()
      B.high()
      C.high()
      D.low()
    if ((ExpandedSensorValueS>=52425) and (ExpandedSensorValueS<=58977)):
      A.low()
      B.low()
      C.low()
      D.high()
    if ((ExpandedSensorValueS>=58978) and (ExpandedSensorValueS<=65535)):
      A.high()
      B.low()
      C.low()
      D.high()
    #print("ADC: ", ExpandedSensorValueS)
    #print("PWM3: ", pwm3.duty_u16())
    #print("PWM4: ", pwm4.duty_u16())
    #print("PWM5: ", pwm5.duty_u16())
    #utime.sleep(0.1)
