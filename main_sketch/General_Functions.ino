/**
  EMAG MESSESTAND, Arduino Programm
  Name: General_Functions
  Purpose: contains general functions that did not fit into the specific function tabs
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.0 19/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/


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
    return (true);
  }
  //prüfe ob endschalter ausgelöst haben
  else if (digitalRead(E_Ini_Endschalter)) {
    resetReference();
    handleStatus(406);          //gebe fehlercode 406 aus, für Endschalter betätigt
    return (true);
  }
  //prüfe ob der letzte fehler behoben ist, dann gebe statuscode 501 aus, für Anlage bereit, ref fahren
  else {
    if (lastStatus == 402 && !digitalRead(E_Anlage_EIN)) handleStatus(501);        //steuerspannung/notaus wieder ein
    else if (lastStatus == 406 && !digitalRead(E_Ini_Endschalter)) handleStatus(501);   //endschalter wieder frei
    else if (lastStatus == 410 && !checkCylinder()) handleStatus(501);                 //zylinder 1 wieder eingefahren
    else if (lastStatus == 411 && !checkCylinder()) handleStatus(501);                 //zylinder 2 wieder eingefahren
    else if (lastStatus == 412 && !checkCylinder()) handleStatus(501);                 //zylinder 3 wieder eingefahren
    else if (lastStatus == 413 && !checkCylinder()) handleStatus(501);                 //zylinder 4 wieder eingefahren
    else if (lastStatus == 414 && !checkCylinder()) handleStatus(501);                 //zylinder 5 wieder eingefahren
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
  digitalWrite(A_Rly_Bed_Referenz, HIGH);                 //Schalte Meldeleuchte im bedienpult aus für Referenzfahrt nicht gefahren
}
