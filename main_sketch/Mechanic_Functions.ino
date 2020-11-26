/**
  EMAG MESSESTAND, Arduino Programm
  Name: Mechanic_Functions
  Purpose: contains functions that control the motor and also pneumatics of the system
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.1.0 23/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/


/**
  initialisiert die Motorschnittstelle
*/
void initMotor() {
  //motor initialisieren
  Motor.connectToPins(A_Mot_Takt, A_Mot_Richtung, C_Mot_Richtung_Pegel);  //Initialisiere das Objekt motor mit den Pins des Motors und der Richtung des Koordinatensystems
  Motor.setStepsPerMillimeter(C_schritteProMM);                           //gebe die anzahl an schritten pro mm verfahrstrecke an
  Motor.setStepsPerRevolution(C_schritteProUmdrehung);                    //gebe das verhältnis von schritten zu umdrehungen der riemenachse an
  Motor.setSpeedInMillimetersPerSecond(C_speed);                          //Lege die Maximalgeschwindigkeit des Motors in mm/s fest
  Motor.setAccelerationInMillimetersPerSecondPerSecond(C_beschleunigung); //lege die Beschleunigung in mm/s² fest

  //Referenzmerker inizialisieren
  Merker_Referenzfahrt_Gefahren = false;
}

/**
  diese funktion prüft ob sicherheitseinrichtungen betätigt sind,
  dann ob die Zylinder alle in grundstellung sind (sofern überwachung eingeschaltet ist)
  und fährt dann den schlitten auf den Initiator - Referenz,
  sofern ein nullpunktversatz oder eine grundposition anders als 0 definiert wurde wird diese angefahren
*/
void referenzfahrt() {
  //gebe den Status des Referenzmerkers vor beginn der Referenzfahrt aus
  Serial.print("*H$");
  Serial.println(Merker_Referenzfahrt_Gefahren);

  //deaktiviere die meldeleuchte im bedienpult
  digitalWrite(A_Rly_Bed_Referenz, LOW);

  //lösche referenzfahrt
  resetReference();

  //gebe statuscode 503 aus, für referenzfahrt gestartet
  handleStatus(503);

  //prüfe ob Steuerspannung eingeschaltet und Notaus nicht betätigt ist, ansonsten führe diesen block aus
  if (digitalRead(E_Anlage_EIN) == HIGH) {
    handleStatus(405);                                      //gebe fehlercode 405 aus, für referenzfahrt fehlgeschlagen
    handleStatus(402);                                      //gebe fehlercode 402 aus, für Steuerspannung fehlt, notaus betätigt?
    return;                                                 //breche die referenzfahrt ab und beende die funktion
  }

  //Fahre alle Zylinder auf grundposition
  setCylinder(1, 0);                                        //zylinder 1 einfahren
  setCylinder(2, 0);                                        //zylinder 2 einfahren
  setCylinder(3, 0);                                        //zylinder 3 einfahren
  setCylinder(4, 0);                                        //zylinder 4 einfahren
  setCylinder(5, 0);                                        //zylinder 5 einfahren
  delay(C_zylEinfahrdauer * 2);                             //warte, um den zylindern zeit zu geben um einzufahren (*2 für eventuellen druckabfall)
  //prüfe ob alle zylinder auf grundposition stehen, wenn nicht führe diesen block aus
  if (checkCylinder())  {
    //fehlercode für Zylinder hat vorrang, darum wird 405 nicht ausgegeben
    //handleStatus(405);      //gebe fehlercode 405 aus, für referenzfahrt fehlgeschlagen
    Merker_Referenzfahrt_Gefahren = false;                            //speichere das der referenzpunkt nicht geholt wurde
    return;                 //breche die referenzfahrt ab und beende die funktion
  }
  //fahre den Initiator - referenz an, falls dies fehlschlägt führe folgenden block aus
  if (Motor.moveToHomeInMillimeters(C_ref_directionTowardsHome, C_ref_speed, C_ref_maximaleReferenzstreckeInMM, E_Ini_Referenz) == false) {
    handleStatus(405);      //gebe fehlercode 405 aus, für referenzfahrt fehlgeschlagen
    handleStatus(408);      //gebe fehlercode 408 aus, für ini nicht gefunden
    Merker_Referenzfahrt_Gefahren = false;                            //speichere das der referenzpunkt nicht geholt wurde
    return;                 //breche die referenzfahrt ab und beende die funktion
  }

  //Versetze den Nullpunkt falls nötig
  Motor.setCurrentPositionInMillimeters(V_ref_nullpunktversatzInMM);  //lege die entfernung zum Nullpunkt fest
  digitalWrite(A_Rly_Bed_Referenz, HIGH);                             //Schalte Meldeleuchte im bedienpult ein für Referenzfahrt gefahren
  Merker_Referenzfahrt_Gefahren = true;                               //speichere das der referenzpunkt geholt wurde
  fahreAbsolut(C_homePosInMM);                                        //fahre auf grundposition
  handleStatus(502);        //gebe statuscode 502 aus, für referenzfahrt beendet
  printPos();               //gebe die Position der Anlage über seriell aus
  return;                   //beende die Funktion
}

/**
  diese funktion verfährt den Motor auf die absolute position in mm die als parameter angegeben wird
  @param zielwert - zielposition in mm
*/
void fahreAbsolut(float zielwert) {
  if (checkFahrenOk(zielwert))Motor.moveToPositionInMillimeters(zielwert);         //fahre auf zielposition in mm, sofern grenzbereich frei
}

/**
  diese funktion verfährt den Motor relativ von der aktuellen position in mm, um den wert der als parameter angegeben wird
  @param zielwert - verfahrweg in mm
*/
void fahreRelativ(float verfahrwert) {
  float zielwert = verfahrwert + motorPosition();
  fahreAbsolut(zielwert);
}

/**
  lese die aktuelle motorposition aus und gebe diese zurück
  @return motorposition in mm
*/
float motorPosition() {
  return (Motor.getCurrentPositionInMillimeters());    //gebe position des Motors in mm zurück
}

/**
  diese funktion prüft ob der zielwert ausserhalb der Grenzbereiche des Motorsliegt
  wenn grenzbereiche eingehalten sind und zylinder eingefahren sind gebe true zurück sonst false
  @return freigabe ok oder nicht (true = fahren on, false = nicht fahren !!!)
*/
bool checkFahrenOk(float zielwert) {
  if (!Merker_Referenzfahrt_Gefahren) return (false); //wenn referenzfahrt fehlt, gebe false zurück
  if (checkCylinder()) return (false);                  //wenn zylinder nicht eingefahren sind, gebe false zurück
  //wenn zielwert zu klein ist
  else {
    if (zielwert < C_minPosInMM) {                      //wenn zielwert zu klein
      handleStatus(420);                                //gebe fehlercode 420 aus, für zielwert zu klein
      return (false);                                   //gebe false zurück
    }
    //wenn zielwert zu gross ist
    else if (zielwert > C_maxPosInMM) {                 //sonst wenn zielwert zu gross
      handleStatus(421);                                //gebe fehlercode 421 aus, für zielwert zu gross
      return (false);                                   //gebe false zurück
    }
    else return (true);
  }
}

/**
  diese funktion prüft ob alle Zylinder eingefahren sind,
  sie ist als bool ausgeführt da die funktion ein ergebnis zurück gibt
  zur prüfung ob die Zylinder eingefahren sind kann folgende funktion verwendet werden:
  if (checkCylinder()){aktion}
  ist einer der Zylinder nicht eingefahren wird true zurückgegeben
  und der entsprechende fehlercode automatisch gesendet
  zudem wird die Referenzfahrt zurückgesetzt, um fahren mit aktivem Zylinder zu verhindern
  @return ergebnis ob zylinder ausgefahren sind, true wenn einer NICHT Eingefahren ist, sonst false
*/
bool checkCylinder() {
  //lege temporäre variablen an, diese werden nach ablauf der funktion wieder gelöscht
  bool flag = true;         // lege variable flag an, sind Zylinder ausgefahren wird diese nicht auf false gesetzt

  //prüfe ob Pneumatik überwachung aktiv ist
  if (C_zylinderUeberwachen) {
    //wenn Endlagenüberwachung des Zylinders nicht eingefahren anzeigt, gib jeweiligen fehlercode aus
    if (digitalRead(E_Ini_Zyl_1)) handleStatus(410);
    else if (digitalRead(E_Ini_Zyl_2)) handleStatus(411);
    else if (digitalRead(E_Ini_Zyl_3)) handleStatus(412);
    else if (digitalRead(E_Ini_Zyl_4)) handleStatus(413);
    else if (digitalRead(E_Ini_Zyl_5)) handleStatus(414);
    //ansonsten setze die flag auf false, für "alle zylinder eingefahren"
    else flag = false;
  }
  //ansonsten prüfe nur das ausgangssignal der Ventile
  else {
    //wenn ausgangssignal für Ventile high ist (zylinder ausfahren), gib jeweiligen fehlercode aus
    if (digitalRead(A_Rly_Zyl_1)) handleStatus(410);
    else if (digitalRead(A_Rly_Zyl_2)) handleStatus(411);
    else if (digitalRead(A_Rly_Zyl_3)) handleStatus(412);
    else if (digitalRead(A_Rly_Zyl_4)) handleStatus(413);
    else if (digitalRead(A_Rly_Zyl_5)) handleStatus(414);
    //ansonsten setze die flag auf false, für "alle zylinder eingefahren"
    else flag = false;
  }

  //wenn flag immernoch true ist, also ein zylinder nicht eingefahren ist, lösche die referenzfahrt
  if (flag) Merker_Referenzfahrt_Gefahren = false;

  //gebe den Status zurück, false für alle zylinder eingefahren, true für mindestens einer ausgefahren
  return (flag);
}

/**
  diese funktion fährt einzelne zylinder aus oder ein
  @param nr - nummer des Zylinders
  @param state - richtung, 1 = ausfahren / 0 = einfahren
*/
void setCylinder(int nr, bool state) {
  if (!Merker_Referenzfahrt_Gefahren && (state == 1)) return;
  //frage variable nr ab, welcher zylinder angesteuert werden soll
  switch (nr) {
    case 1:
      digitalWrite(A_Rly_Zyl_1, state);   //steuere zylinder 1 an
      break;
    case 2:
      digitalWrite(A_Rly_Zyl_2, state);   //steuere zylinder 2 an
      break;
    case 3:
      digitalWrite(A_Rly_Zyl_3, state);   //steuere zylinder 3 an
      break;
    case 4:
      digitalWrite(A_Rly_Zyl_4, state);   //steuere zylinder 4 an
      break;
    case 5:
      digitalWrite(A_Rly_Zyl_5, state);   //steuere zylinder 5 an
      break;
  }
}
