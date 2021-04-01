from machine import Pin, PWM
#import utime
ADC_A0 = machine.ADC(26)
pwm3 = PWM(Pin(3)) #blau
pwm4 = PWM(Pin(4)) #rot
pwm5 = PWM(Pin(5)) #grün
pwm3.freq(500)
pwm4.freq(500)
pwm5.freq(500)
A = Pin(13, Pin.OUT) #A
B = Pin(10, Pin.OUT) #B    
C = Pin(11, Pin.OUT) #C    
D = Pin(12, Pin.OUT) #D
while True:
    # ADC
    neuerWert = int((ADC_A0.read_u16()/1.7)*3.3)
    if neuerWert < 0:
      neuerWert = 0
    if neuerWert > 65535:
      neuerWert = 65535
    # LED-Ansteuerung
    duty3=0
    duty4 = (65535-neuerWert)    
    duty5 = neuerWert    
    pwm3.duty_u16(duty3)
    pwm4.duty_u16(duty4)
    pwm5.duty_u16(duty5)
    # Siebensegmentansteuerung        
    if ((neuerWert>=0) and (neuerWert<=6553)):      
      A.low()      
      B.low()      
      C.low()      
      D.low()
    if ((neuerWert>=6554) and (neuerWert<=13106)):
      A.high()
      B.low()
      C.low()
      D.low()
    if ((neuerWert>=13107) and (neuerWert<=19659)):
      A.low()
      B.high()
      C.low()
      D.low()
    if ((neuerWert>=19660) and (neuerWert<=26212)):
      A.high()
      B.high()
      C.low()
      D.low()
    if ((neuerWert>=26213) and (neuerWert<=32765)):
      A.low()
      B.low()
      C.high()
      D.low()
    if ((neuerWert>=32766) and (neuerWert<=39318)):
      A.high()
      B.low()
      C.high()
      D.low()
    if ((neuerWert>=39319) and (neuerWert<=45871)):
      A.low()
      B.high()
      C.high()
      D.low()
    if ((neuerWert>=45872) and (neuerWert<=52424)):
      A.high()
      B.high()
      C.high()
      D.low()
    if ((neuerWert>=52425) and (neuerWert<=58977)):
      A.low()
      B.low()
      C.low()
      D.high()
    if ((neuerWert>=58978) and (neuerWert<=65535)):
      A.high()
      B.low()
      C.low()
      D.high()
    #print("ADC: ", neuerWert)
    #print("PWM3: ", pwm3.duty_u16())
    #print("PWM4: ", pwm4.duty_u16())
    #print("PWM5: ", pwm5.duty_u16())
    #utime.sleep(0.1)
