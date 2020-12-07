from Adafruit_CharLCD import Adafruit_CharLCD
import Adafruit_GPIO.PCF8574 as PCF
import RPi.GPIO as GPIO
import sys
LCD = PCF.PCF8574(address=0x27)
LCD.setup(5,0)
LCD.output(5,0)
# Deklariert welche PCF-Pins mit welchem LCD-Pin verbunden sind.
lcd_rs = 4 # RegisterSelect Pin des LCDs ist an GPIO4
lcd_en = 7 # Enable Pin des LCDs ist an GPIO7
d4,d5,d6,d7 = 0,1,2,3 # Datenpins 4,5,6,7 des LCDs sind an GPIO 0,1,2,3
cols,lines = 16,2 # Anzahl der Spalten und Zeilen des Displays

GPIO.setmode(GPIO.BCM)
# Initialisiert den LCD
lcd = Adafruit_CharLCD(lcd_rs, lcd_en, d4, d5, d6, d7,cols, lines, gpio=LCD)
lcd.clear()

#lcd.message('EMAG MESSESTAND')
#lcd.set_cursor(0,1)
lcd.message(sys.argv[1])
