import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False) 
GPIO.setup(5, GPIO.OUT)
GPIO.output(5, GPIO.HIGH)
