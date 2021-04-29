/**
  EMAG MESSESTAND, Arduino Programm
  Name: General_Functions
  Purpose: contains general functions that did not fit into the specific function tabs
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.1.7 29/04/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/
//lokal benötigte variablen
bool Merker_Blink_Meldeleuchte_Status = LOW;      //Merker für status der Meldeleuchte, wird für blinken benutzt
unsigned long previousMillis = 0;                 //speichert letzten zeitpunkt des LED Updates
int Automatikbetrieb_Schritt = 0;                 //schrittmerker für automatikzyklus
bool lastAutocycleState = true;                   //speichert ob zuletzt automatik aktiv war, um auf grundpos zu fahren
/**
  diese funktion prüft ob fehler vorhanden sind, und gibt bei fehler ein true zurück
  zur prüfung ob fehler vorhanden sind kann folgende funktion verwendet werden:
  if (checkFehler()){aktion}
  ist der letzte vorhandene fehler wieder behoben wird zusätzlich die quittierung des fehlercodes über Seriell ausgeführt
  @return ergebnis ob fehler vorhanden sind, true wenn fehler vorhanden, sonst false
*/
bool checkFehler() {
  //prüfe ob Steuerspannung (24V) aus ist oder notaus betätigt ist
  if (digitalRead(E_Anlage_EIN)) {
    resetReference();
    handleStatus(402);          //gebe fehlercode 402 aus, für Steuerspannung fehlt, notaus betätigt?
    blinkLed(C_ref_blink_Interval / 2);
    return (true);
  }
  //prüfe ob endschalter ausgelöst haben
  else if (digitalRead(E_Ini_Endschalter)) {
    resetReference();
    handleStatus(406);          //gebe fehlercode 406 aus, für Endschalter betätigt
    blinkLed(C_ref_blink_Interval / 2);
    return (true);
  }
  //prüfe ob der letzte fehler behoben ist, dann gebe statuscode 501 aus, für Anlage bereit, ref fahren
  else {
    //prüfe ob steuerspannung wieder an ist (inkl. entprellen), warte dann 5s auf motor start und gebe anlage frei
    if (lastStatus == 402 && !digitalRead(E_Anlage_EIN)) {
      delay(1);                                                                         //warte 1ms zum entprellen
      if (!digitalRead(E_Anlage_EIN)) {                                                 //prüfe ob spannung immernoch ein ist, zum entprellen
        Merker_Blink_Meldeleuchte_Status = LOW;                                         //speichere Meldeleuchte aus
        digitalWrite(A_Rly_Bed_Referenz, Merker_Blink_Meldeleuchte_Status);             //Schalte Meldeleuchte aus
        delay(3500);                                                                    //warte 3.5 sekunden um Motor starten zu lasseb
        handleStatus(501);                                                              //steuerspannung/notaus wieder ein
      }
    }
    else if (lastStatus == 406 && !digitalRead(E_Ini_Endschalter)) handleStatus(501);   //endschalter wieder frei
    else if (lastStatus == 410 && !checkCylinder(1)) handleStatus(501);                 //zylinder 1 wieder eingefahren
    else if (lastStatus == 411 && !checkCylinder(2)) handleStatus(501);                 //zylinder 2 wieder eingefahren
    else if (lastStatus == 412 && !checkCylinder(3)) handleStatus(501);                 //zylinder 3 wieder eingefahren
    else if (lastStatus == 413 && !checkCylinder(4)) handleStatus(501);                 //zylinder 4 wieder eingefahren
    else if (lastStatus == 414 && !checkCylinder(5)) handleStatus(501);                 //zylinder 5 wieder eingefahren
  }
  return (false);
}

/**
  diese funktion prüft ob referenzfahrt gefahren war und setzt diese zurück
  falls die referenz vorher gefahren war wird zusätzlich fehlercode 409 für referenz verloren ausgegeben
*/
void resetReference() {
  if (Merker_Referenzfahrt_Gefahren) handleStatus(409);   //gebe fehlercode 409 aus, für referenzpunkt verloren, referenzfahrt wird benötigt
  Merker_Referenzfahrt_Gefahren = false;                  //lösche merker für referenzfahrt gefahren
  //ausschalten der Meldeleuchte wird über Blinkfunktion erledigt
}

/**
  Diese funktion lässt die LED im bedienfeld blinken
  @param interval - Blinkintervall in millisekunden
*/
void blinkLed(unsigned long intervall) {
  unsigned long currentMillis = millis();               //speichere die aktuelle systemzeit
  //wenn systemzeit grösser als intervall im vergleich zur letzten zeit ist, schalte led um
  if (currentMillis - previousMillis >= intervall) {
    //speichere zeit als vergleichswert
    previousMillis = currentMillis;

    //wenn led aus ist, schalte sie ein und umgekehrt
    if (Merker_Blink_Meldeleuchte_Status == LOW)Merker_Blink_Meldeleuchte_Status = HIGH;
    else Merker_Blink_Meldeleuchte_Status = LOW;
    digitalWrite(A_Rly_Bed_Referenz, Merker_Blink_Meldeleuchte_Status);
  }
}

/**
  diese funktion verarbeitet die eingänge des bedienfeldes
*/
void bedienfeld() {
  //wenn taster auf bedienfeld gedrückt wurde, und nicht prellt (kurz warten) führe diesen block aus
  if (!digitalRead(E_Bed_Starte_Referenzfahrt)) {
    delay(5);                                                 //kurzes delay wegen prellen und induktionsproblemen
    if (!digitalRead(E_Bed_Starte_Referenzfahrt)) {
      if (!Merker_Referenzfahrt_Gefahren) referenzfahrt();    //wenn keine referenz gefahren, fahre Referenz
      else {
        Serial.println("*K$1");                               //ansonsten sende Taster 1 gedrückt an arduino
        //delay(1000);                                          //warte 1s, um zeit zum loslassen zu geben
        while (!digitalRead(E_Bed_Starte_Referenzfahrt))delay(1); //warte bis taster losgelassen wird
      }
    }
  }

  //wenn referenz gefahren ist
  if (Merker_Referenzfahrt_Gefahren) {
    //prüfe ob Schalter Automatikbetrieb betätigt ist (inkl. entprellen)
    if (!digitalRead(E_Bed_Automatik)) {
      delay(5);                                               //kurzes delay wegen prellen und induktionsproblemen
      if (!digitalRead(E_Bed_Automatik)) pendelbetrieb();     //fahre im Pendelbetrieb
    }
    //ansonsten wenn automatik zuletzt an war fahre auf grundposition
    else if (lastAutocycleState == true) {
      fahreAbsolut(1100);                                     //fahre auf Grundposition (1100)
      lastAutocycleState = false;                             //merke das Automatik zuletzt aus war
      handleStatus(504);
    }
  }
}

/**
  diese funktion prüft ob der Taster zum Herunterfahren gedrückt wurde
*/
void checkButton() {
  if (!digitalRead(E_Herunterfahren)) {
    delay(5);
    if (!digitalRead(E_Herunterfahren)) {
      Serial.println("*K$2");
      while (!digitalRead(E_Herunterfahren))delay(1);
    }
  }
}

/**
  diese funktion wird vom Pi ausgelöst und fährt den arduino in nullstellung
*/
void herunterfahren() {
  Motor.setSpeedInMillimetersPerSecond(100);                        //Lege die Maximalgeschwindigkeit des Motors in mm/s fest
  Motor.setAccelerationInMillimetersPerSecondPerSecond(100); //lege die Beschleunigung in mm/s² fest
  fahreAbsolut(0);
  delay(5000);
  //blinke Meldeleuchte im bedienpult aus für anlage aus
  digitalWrite(A_Rly_Bed_Referenz, !Rly_ON_Level);
  delay(1000);
  digitalWrite(A_Rly_Bed_Referenz, Rly_ON_Level);
  delay(250);
  digitalWrite(A_Rly_Bed_Referenz, !Rly_ON_Level);                     //Schalte Meldeleuchte im bedienpult aus für anlage aus
  printText("shutdown complete");
  while(true)delay(1000);                                              //friere die steuerung ein..
}
