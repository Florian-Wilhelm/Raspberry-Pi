import lcddriver
import Adafruit_BMP.BMP280 as BMP280
import time

lcd = lcddriver.lcd()
lcd.lcd_clear() # clears the LCD screen and positions the cursor in the upper-left corner

sensor = BMP280.BMP280()

# initial values
temperature = sensor.read_temperature()
pressure = sensor.read_pressure()
lcd.lcd_display_string("Temp = {0:0.2f} C".format(temperature),1)
lcd.lcd_display_string("Druck = {0:0.0f} Pa".format(pressure),2)

i = 0

# reading temperature and pressure in an infinite loop
while True:
  temperature = sensor.read_temperature()
  pressure = sensor.read_pressure()
  # temperature value is updated every second on the LCD
  lcd.lcd_display_string("Temp = {0:0.2f} C".format(temperature),1)
  # pressure value is updated once every minute on the LCD 
  if i == 60:
    lcd.lcd_display_string("Druck = {0:0.0f} Pa".format(pressure),2)
    i = 0
  time.sleep(1)
  i=i+1