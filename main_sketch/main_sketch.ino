/**
  EMAG MESSESTAND, Arduino Programm
  Name: main_sketch
  Purpose: controls the hardware of the Machine
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.0 18/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/
//andere sketch tabs einbinden
#include "Settings.h"
#include "GPIO_Definitions.h"
#include "Serialtexts.h"
#include "Serial_Functions.h"
#include "Mechanic_Functions.h"

//Libraries laden
#include "libraries/JRStep/JRStep.cpp"      //modifizierte version von flexystepper, ist im sketch ordner abgelegt
JRStep Motor;                               //JRStep mit dem Namen stepper initialisieren

/*
   Variablen definieren
*/
bool Merker_Referenzfahrt_Gefahren = false; //Merker für Referenzfahrt gefahren

/**
  funktion läuft einmalig bei start und Reset
  hier werden initialisierungen und Definitionen vorgenommen
*/
void setup() {
  delay(100);                               //recovery delay, verhindert lästige bugs :D

  //IO initialisieren
  initGPIO();                               //zu finden in tab "GPIO_Definitions"

  //Serielle Schnittstelle initialisieren und starten
  initSerial();                             //zu finden in tab "Serial_Functions"

  //ausgabe arduino gestartet
  handleStatus(401);                        //ausgabe von information / statuscode 401 für anlage gestartet

  //motor initialisieren
  Motor.connectToPins(A_Mot_Takt, A_Mot_Richtung);                        //Initialisiere das Objekt stepper mit den Pins des Motors
  Motor.setStepsPerMillimeter(C_schritteProMM);
  Motor.setStepsPerRevolution(C_schritteProUmdrehung);
  Motor.setSpeedInMillimetersPerSecond(C_speed);                          //Lege die Maximalgeschwindigkeit des Motors in mm/s fest
  Motor.setAccelerationInMillimetersPerSecondPerSecond(C_beschleunigung); //lege die Beschleunigung in mm/s fest

  //Referenzmerker inizialisieren
  Merker_Referenzfahrt_Gefahren = false;
}

/**
  diese funktion läuft in endlosschleife, kann jedoch durch Serial events pausiert werden
  hier findet die Abfrage der Sicherheitseinrichtungen und bedienelemente statt
*/
void loop() {
  //prüfe ob steuerspannung vorhanden ist und notaus nicht betätigt ist (ANLAGE EIN)
  if (digitalRead(E_Anlage_EIN) == LOW) {
    //prüfe ob Endschalter betätigt wurden
    if (digitalRead(E_Ini_Endschalter) == HIGH) { //Wenn endschalter ausgelöst haben

    }
    //wenn keine endschalter betätigt sind, führe folgenden block aus
    else {
      //wenn keine referenzfahrt gefahren wurde und der taster Referenzfahrt fahren betätigt ist:
      if (!Merker_Referenzfahrt_Gefahren && (digitalRead(Referenzfahrt_Pin) == LOW)) {
        delay(100);                         //warte 100ms, um prellen des Tasters und Störungen durch induktion zu vermeiden
        //prüfe erneut ob der taster betätigt ist
        if (digitalRead(Referenzfahrt_Pin) == LOW){
          referenzfahrt();                  //fahre Referenz
        }
      }
    }
  }
}
