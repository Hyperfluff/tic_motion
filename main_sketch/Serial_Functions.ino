/**
  EMAG MESSESTAND, Arduino Programm
  Name: Serial_Functions
  Purpose: contains functions for handling and receiving and sending Serial content
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.0 19/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/


/**
  initialisiert das Serial interface
*/
void initSerial(){
  Serial.begin(SERIAL_BAUD);  //Initialisiere eine Serielle Konsole, BAUD definiert in settings.h
  Serial.println();           //Beginne eine Neue Zeile, verhindert nach neustart das die erste zeile an der vorherigen beginnt
  Serial.println();           //gebe eine leere zeile aus
  Serial.println();           //gebe eine leere zeile aus
  Serial.println();           //gebe eine leere zeile aus
  Serial.println();           //gebe eine leere zeile aus
  printText(Boot_Software_Info);       //Startnachricht ausgeben, enthält auch die Version der software
  Serial.println(F("#Dateiname " __FILE__));
  Serial.println(F("#Stand " __DATE__));

  //gebe starttext aus, diese funktion ist etwas komplexer da anstatt einem string der text im PROGMEM (nicht im RAM) abgelegt wird
  printCharArray(Boot_Text);  
  
  //Serial.println(Boot_Text);    //infotext ausgeben, wird in texts.h definiert
  if (!C_zylinderUeberwachen) printText("ACHTUNG PNEUMATIK WIRD NICHT ÜBERWACHT");
}

/**
  Verarbeitet Serielle befehle, sobald diese ankommen
  */
void serialEvent() {
  
}

/**
  sendet menschenlesbaren text an serielle schnittstelle mit § als kennzeichnung für infotext
  @param text - menschenlesbarer text.
  */
void printText(String text) {
  Serial.print("§");
  Serial.println(text);
}

/**
  gibt in PROGMEM gespeicherten text aus
  @param VARIABLENNAME enthält das relevante char array.
  */
void printCharArray(char* VARIABLENNAME){
  char myChar;                                              //lege leere Variable für einzelne textzeichen an
  for (unsigned long k = 0; k < strlen_P(VARIABLENNAME); k++) { //widerhole schleife für jedes zeichen im text
    myChar = pgm_read_byte_near(VARIABLENNAME + k);             //lege das textzeichen in die variable ab
    Serial.print(myChar);                                   //gebe das textzeichen aus
  }
}

/**
  gibt Position der Analge über Seriell aus
  Aufbau Datensatz:
  *P$[PositionInMM]
  *Z$1,[PositionZylinder1]
  *Z$2,[PositionZylinder2]
  *Z$3,[PositionZylinder3]
  *Z$4,[PositionZylinder4]
  *Z$5,[PositionZylinder5]
  *H$S[tatusMerkerReferenzfahrtGefahren]
 */
void printPos() {
  //gebe motor/schlittenposition aus
  Serial.print("*P$");
  Serial.println(-Motor.getCurrentPositionInMillimeters());

  //prüfe ob Pneumatik überwachung aktiv ist
  if (C_zylinderUeberwachen) {
    //gebe Position Zylinder 1 aus
    Serial.print("*Z$1,");
    Serial.println(digitalRead(E_Ini_Zyl_1));
    
    //gebe Position Zylinder 2 aus
    Serial.print("*Z$2,");
    Serial.println(digitalRead(E_Ini_Zyl_2));
    
    //gebe Position Zylinder 3 aus
    Serial.print("*Z$3,");
    Serial.println(digitalRead(E_Ini_Zyl_3));
    
    //gebe Position Zylinder 4 aus
    Serial.print("*Z$4,");
    Serial.println(digitalRead(E_Ini_Zyl_4));
    
    //gebe Position Zylinder 5 aus
    Serial.print("*Z$5,");
    Serial.println(digitalRead(E_Ini_Zyl_5));
  }
  //ansonsten gebe status der Ventilausgänge aus
  else {
    //gebe Status Ventil 1 aus
    Serial.print("*Z$1,");
    Serial.println(digitalRead(A_Rly_Zyl_1));
    
    //gebe Status Ventil 2 aus
    Serial.print("*Z$2,");
    Serial.println(digitalRead(A_Rly_Zyl_2));
    
    //gebe Status Ventil 3 aus
    Serial.print("*Z$3,");
    Serial.println(digitalRead(A_Rly_Zyl_3));
    
    //gebe Status Ventil 4 aus
    Serial.print("*Z$4,");
    Serial.println(digitalRead(A_Rly_Zyl_4));
    
    //gebe Status Ventil 5 aus
    Serial.print("*Z$5,");
    Serial.println(digitalRead(A_Rly_Zyl_5));
  }

  //gebe Status der Referenzfahrt aus (Merker für Referenzfahrt gefahren)
  Serial.print("*H$");
  Serial.println(Merker_Referenzfahrt_Gefahren);
}

/**
  Verarbeitet fehler und statuscodes, und gibt die dazugehörigen infos auf Seriell aus
  @param statuscode - fehler oder Statuscode.
  */
void handleStatus(int statuscode) {
  if (statuscode == lastStatus) return;           //wenn statuscode sich widerholt, ignoriere den code
  Serial.print("*F$");                            //gebe das Zeichen F für statuscode aus
  Serial.println(statuscode);                     //beende die zeile mit dem statuscode
  if (statuscode != 504)lastStatus = statuscode;  //wenn Zyklusende kommt, code nicht speichern, widerholung erlauben
  
  //Gebe zu dem Jeweiligen statuscode eine Menschenlesbare Beschreibung mit aus
  switch (statuscode) {
    case 401: printText("INFORMATION: Arduino Gestartet"); break;
    case 402: printText("FEHLER: Steuerspannung fehlt, Notaus betätigt?"); break;
    case 404: printText("FEHLER: Referenzfahrt fehlt"); break;
    case 405: printText("FEHLER: Referenzfahrt fehlgeschlagen"); break;
    case 406: printText("FEHLER: Endschalter betätigt"); break;
    case 408: printText("FEHLER: Initiator für Nullpunkt in Maximallänge des Verfahrweges nicht erreicht, Referenzfahrt abgebrochen"); break;
    case 409: printText("FEHLER: Referenzpunkt verloren, Referenzfahrt wird benötigt"); break;

    case 410: printText("FEHLER: Zylinder 1 Nicht eingefahren, Fahrbefehl nicht akzeptiert"); break;
    case 411: printText("FEHLER: Zylinder 2 Nicht eingefahren, Fahrbefehl nicht akzeptiert"); break;
    case 412: printText("FEHLER: Zylinder 3 Nicht eingefahren, Fahrbefehl nicht akzeptiert"); break;
    case 413: printText("FEHLER: Zylinder 4 Nicht eingefahren, Fahrbefehl nicht akzeptiert"); break;
    case 414: printText("FEHLER: Zylinder 5 Nicht eingefahren, Fahrbefehl nicht akzeptiert"); break;

    case 420: printText("FEHLER: Zielposition zu klein, Fahrbefehl nicht akzeptiert"); break;
    case 421: printText("FEHLER: Zielposition zu groß, Fahrbefehl nicht akzeptiert"); break;

    case 430: printText("FEHLER: Befehl unbekannt, gebe ? für eine Liste an befehlen ein"); break;

    case 501: printText("INFORMATION: Fehler Quittiert"); break;
    case 502: printText("INFORMATION: Referenzfahrt gefahren"); break;
    case 503: printText("INFORMATION: Referenzfahrt gestartet"); break;
    case 504: printText("INFORMATION: Zyklus beendet"); break;

    default: printText("FEHLER: Keine Beschreibung hinterlegt");
  }
}
