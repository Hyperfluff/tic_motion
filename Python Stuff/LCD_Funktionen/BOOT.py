from Adafruit_CharLCD import Adafruit_CharLCD
import Adafruit_GPIO.PCF8574 as PCF
import RPi.GPIO as GPIO
import time
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

lcd.message('EMAG MESSESTAND\nAnlage startet..')
time.sleep(1)
lcd.message('EMAG MESSESTAND\n_nlage startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n__lage startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n___age startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n____ge startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n_____e startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n______ startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n_______startet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n________tartet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n_________artet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n__________rtet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n___________tet..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n____________et..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n_____________t..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n______________..')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n_______________.')
time.sleep(0.1)
lcd.message('EMAG MESSESTAND\n________________')
time.sleep(0.1)

