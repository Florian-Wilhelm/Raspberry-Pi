from machine import Pin, PWM
#import utime
ADC_A0 = machine.ADC(26)
pwm3 = PWM(Pin(3)) #blau
pwm4 = PWM(Pin(4)) #rot
pwm5 = PWM(Pin(5)) #grün
pwm3.freq(500)
pwm4.freq(500)
pwm5.freq(500)
while True:        
    duty3=0
    duty4=(65535-(int((ADC_A0.read_u16()/1.7)*3.3)))
    if duty4<0:
      duty4 = 0
    duty5=(int((ADC_A0.read_u16()/1.7)*3.3))
    if duty5>65535:
      duty5 = 65535
    pwm3.duty_u16(duty3)
    pwm4.duty_u16(duty4)
    pwm5.duty_u16(duty5)    
    #print("ADC: ", ADC_A0.read_u16())
    #print("PWM3: ", pwm3.duty_u16())
    #print("PWM4: ", pwm4.duty_u16())
    #print("PWM5: ", pwm5.duty_u16())
    #utime.sleep(0.1)