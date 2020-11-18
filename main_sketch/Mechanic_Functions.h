/**
  EMAG MESSESTAND, Arduino Programm
  Name: Mechanic_Functions
  Purpose: contains functions that control the motor and also pneumatics of the system
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.0 18/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/

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

  //gebe statuscode 503 aus, für referenzfahrt gestartet
  handleStatus(503);

  //prüfe ob Steuerspannung eingeschaltet und Notaus nicht betätigt ist, ansonsten führe diesen block aus
  if (digitalRead(Anlage_EIN_Pin) == HIGH) {
    handleStatus(402);      //gebe fehlercode 402 aus, für Steuerspannung fehlt, notaus betätigt?
    handleStatus(405);      //gebe fehlercode 405 aus, für referenzfahrt fehlgeschlagen
    return;                 //breche die referenzfahrt ab und beende die funktion
  }

  //Fahre alle Zylinder auf grundposition
  setCylinder(1, 0);        //zylinder 1 einfahren
  setCylinder(2, 0);        //zylinder 2 einfahren
  setCylinder(3, 0);        //zylinder 3 einfahren
  setCylinder(4, 0);        //zylinder 4 einfahren
  setCylinder(5, 0);        //zylinder 5 einfahren
  delay(Einfahrdauer * 2);  //warte, um den zylindern zeit zu geben um einzufahren (*2 für eventuellen druckabfall)

  //prüfe ob alle zylinder auf grundposition stehen, wenn nicht führe diesen block aus
  if (checkCylinder())  {
    handleStatus(405);      //gebe fehlercode 405 aus, für referenzfahrt fehlgeschlagen
    Merker_Referenzfahrt_Gefahren = false;                          //speichere das der referenzpunkt nicht geholt wurde
    return;                 //breche die referenzfahrt ab und beende die funktion
  }
  //fahre den Initiator - referenz an, falls dies fehlschlägt führe folgenden block aus
  if (Motor.moveToHomeInMillimeters(Referenzfahrt_Richtung, Referenzfahrt_GeschwindigkeitInMMProSekunde, Referenzfahrt_MaximaleStreckeInMM, Ini_Pin) == false) {
    handleStatus(408);      //gebe fehlercode 408 aus, für ini nicht gefunden
    handleStatus(405);      //gebe fehlercode 405 aus, für referenzfahrt fehlgeschlagen
    Merker_Referenzfahrt_Gefahren = false;                          //speichere das der referenzpunkt nicht geholt wurde
    return;                 //breche die referenzfahrt ab und beende die funktion
  }
  //current progress spot
  
  Motor.setCurrentPositionInMillimeters(Referenzfahrt_NullpunktversatzInMM);  //lege die entfernung zum Nullpunkt fest
  
  Motor.moveToPositionInMillimeters(-Position_Home);
  Merker_Referenzfahrt_Gefahren = true;                          //speichere das der referenzpunkt geholt wurde
  handleStatus(502);
  printPos();
  return;
}
bool checkCylinder() {
  bool flag = true;
  if (override_Pneumatics_Check) {
    if (digitalRead(Zylinder_1_Pin)) handleStatus(410);
    else if (digitalRead(Zylinder_2_Pin)) handleStatus(411);
    else if (digitalRead(Zylinder_3_Pin)) handleStatus(412);
    else if (digitalRead(Zylinder_4_Pin)) handleStatus(413);
    else if (digitalRead(Zylinder_5_Pin)) handleStatus(414);
    else flag = false;
  }
  else {
    if (digitalRead(Zylinder_1_Ueberwachung_Pin)) handleStatus(410);
    else if (digitalRead(Zylinder_2_Ueberwachung_Pin)) handleStatus(411);
    else if (digitalRead(Zylinder_3_Ueberwachung_Pin)) handleStatus(412);
    else if (digitalRead(Zylinder_4_Ueberwachung_Pin)) handleStatus(413);
    else if (digitalRead(Zylinder_5_Ueberwachung_Pin)) handleStatus(414);
    else flag = false;
  }

  if (flag == true) {
    Merker_Referenzfahrt_Gefahren = false;
    return (true);
  }
  else return (false);
}
