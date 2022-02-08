# Dieses Skript dient dazu, GPIO5 sicher auf High zu setzen nachdem das erste Python-Skript ausgeführt wurde (werden sequentiell über ein Shell-Skript aufgerufen);
# bei fehlerhafter Ausführung oder Abbruch des ersten Skripts könnte andernfalls die Pumpe dauerhaft laufen.
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False) 
GPIO.setup(5, GPIO.OUT)
GPIO.output(5, GPIO.HIGH)
