/**
  EMAG MESSESTAND, Arduino Programm
  Name: main_sketch
  Purpose: controls the hardware of the Machine
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.1.7 29/04/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/
//andere sketch tabs einbinden
#include "Settings.h"
#include "Serialtexts.h"
#include "GPIO_Definitions.h"
//#include "Serial_Functions.h"             //wird als .ino file automatisch eingebunden, war vorher mal ein .h file
//#include "Mechanic_Functions.h"           //wird als .ino file automatisch eingebunden, war vorher mal ein .h file

//Libraries laden
#include "libraries/JRStep/JRStep.cpp"      //modifizierte version von flexystepper, ist im sketch ordner abgelegt
JRStep Motor;                               //JRStep mit dem Namen motor initialisieren

/*
   global verwendete Variablen definieren
*/
bool Merker_Referenzfahrt_Gefahren;         //Merker für Referenzfahrt gefahren
int lastStatus;                             //zwischenspeicher des letzten statuscodes von handleStatus();

/**
  funktion läuft einmalig bei start und Reset
  hier werden initialisierungen und Definitionen vorgenommen
*/
void setup() {
  delay(100);                               //100ms recovery delay, verhindert lästige bugs :D

  //IO initialisieren
  initGPIO();                               //zu finden in tab "GPIO_Definitions"

  //Serielle Schnittstelle initialisieren und starten
  initSerial();                             //zu finden in tab "Serial_Functions"

  //motor initialisieren
  initMotor();

  //ausgabe arduino gestartet
  handleStatus(401);                        //ausgabe von information / statuscode 401 für anlage gestartet
  
  delay(100);                               //100ms pause, aus prinzip :D
}

/**
  diese funktion läuft in endlosschleife, kann jedoch durch Serial events pausiert werden
  hier findet die Abfrage der Sicherheitseinrichtungen und bedienelemente statt
*/
void loop() {
  //wenn keine Referenzfahrt gefahren wurde blinke LED im bedienpult
  
  if (!Merker_Referenzfahrt_Gefahren) {
    blinkLed(C_ref_blink_Interval);
  }
  //prüfe ob fehler vorhanden sind, falls ja breche loop ab
  if (checkFehler()) return;
  bedienfeld();
  checkButton();
}

/**
  die verarbeitung von Seriellen events findet im tab Serial_Functions statt
  die funktion serialEvent() wird automatisch gestartet sobald seriell daten kommen und bedarf keiner manuellen abfrage
*/
