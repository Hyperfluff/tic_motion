/**
  EMAG MESSESTAND, Arduino Programm
  Name: Serial_Functions
  Purpose: contains functions for handling and receiving and sending Serial content
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.1.5 27/04/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/


/**
  initialisiert das Serial interface
*/
void initSerial() {
  Serial.begin(SERIAL_BAUD);  //Initialisiere eine Serielle Konsole, BAUD definiert in settings.h
  Serial.println();           //Beginne eine Neue Zeile, verhindert nach neustart das die erste zeile an der vorherigen beginnt
  Serial.println();           //gebe eine leere zeile aus
  Serial.println();           //gebe eine leere zeile aus
  Serial.println();           //gebe eine leere zeile aus
  Serial.println();           //gebe eine leere zeile aus
  printText(Boot_Software_Info);       //Startnachricht ausgeben, enthält auch die Version der software
  Serial.println(F("#Dateiname " __FILE__));
  Serial.println(F("#Stand " __DATE__));

  //warte bis Raspberry verbunden ist,
  Serial.println(F("#P Eingeben um Anlage zu Starten"));
  while (!Serial.available()) {
    delay(1);
  }
  while (Serial.read() != 'P'){
    Serial.println(F("# P EINGEBEN UM ZU STARTEN"));
  }
  //gebe starttext aus, diese funktion ist etwas komplexer da anstatt einem string der text im PROGMEM (nicht im RAM) abgelegt wird
  printCharArray(Boot_Text);

  //Serial.println(Boot_Text);    //infotext ausgeben, wird in texts.h definiert
  if (!C_zylinderUeberwachen) printText("ACHTUNG PNEUMATIK WIRD NICHT ÜBERWACHT");
}

/**
  Verarbeitet Serielle befehle, sobald diese ankommen
  für die verarbeitung wird die Loop funktion pausiert und dann die Daten erst vollständig ausgeführt
  erst wenn der Serielle puffer leer ist, wird die Funktion beendet
*/
void serialEvent() {
  //solange wie Daten verfügbar sind, führe die Funktion aus
  while (Serial.available()) {
    //initialisiere mehrere variablen in die die eingehenden werte gelegt werden können
    int int1, int2;                             //lege 2 integer variablen an, diese können 16 bit (-32768 bis 32767) als wert speichern
    bool bool1, bool2;                          //lege 2 boolean variablen an, diese können nur 1/true/HIGH oder 0/false/LOW als wert speichern
    float float1;                       //lege 1 float variable an, diese kann 32 bit werte speichern

    char parameter = Serial.read();             //lese das erste zeichen und speichere es ab
    //prüfe ob der input mit den möglichen parametern übereinstimmt
    switch (parameter) {
      //überspringe zuerst alle sonderzeichen die fehler verursachen können
      case '\n' : break;
      case '\r': break;
      case ' ' : break;
      case '0': break;
      case '1': break;
      case '2': break;
      case '3': break;
      case '4': break;
      case '5': break;
      case '6': break;
      case '7': break;
      case '8': break;
      case '9': break;
      case ',': break;
      case '.': break;
      //hilfemenü
      case '?':                                 //wenn hilfemenü gefordert wird
        int1 = Serial.parseInt();               //lese die folgende seitenzahl aus
        switch (int1) {                         //gebe die entsprechende seite aus
          case 1:
            printCharArray(Help_Text_1);
            break;
          case 2:
            printCharArray(Help_Text_2);
            break;
          case 3:
            printCharArray(Help_Text_3);
            break;
          default:
            printCharArray(Help_Text_1);
            break;
        }
        break;
      //referenzfahrt
      case 'H':
        referenzfahrt();
        break;
      //positionsabfrage
      case 'P':
        printPos();
        break;
      //fahre absolut
      case 'A':
        float1 = Serial.parseFloat();           //lese den folgenden zielwert aus
        Serial.print("*A$"); Serial.println(float1);  //feedback für befehl erhalten
        fahreAbsolut(float1);                   //fahre den zielwert an
        break;
      //fahre relativ
      case 'R':
        float1 = Serial.parseFloat();           //lese den folgenden zielwert aus
        Serial.print("*R$"); Serial.println(float1);  //feedback für befehl erhalten
        fahreRelativ(float1);                   //fahre den zielwert an
        break;
      //warte/pausiere
      case 'D':
        int1 = Serial.parseInt();               //lese die wartedauer aus
        Serial.print("*D$"); Serial.println(int1);  //feedback für befehl erhalten
        delay(int1);                            //pausiere das programm für die wartedauer
        break;
      //werfe zylinder für bestimte zeit aus
      case 'E':
        int1 = Serial.parseInt();               //lese den zielzylinder aus
        int2 = Serial.parseInt();               //lese die wartezeit aus
        setCylinder(int1, true);                //fahre zylinder aus
        delay(int2);                            //warte
        setCylinder(int1, false);               //fahre zylinder wieder ein
        break;
      //setze zylinder auf bestimmten status
      case 'Z':
        int1 = Serial.parseInt();               //lese den zielzylinder aus
        int2 = Serial.parseInt();               //lese den zielstatus aus
        Serial.print("*Z$"); Serial.print(int1); Serial.print(","); Serial.println(int2); //feedback für befehl erhalten
        //wandele int 2 in boolean um
        if (int2 == 1) bool1 = true;
        else bool1 = false;
        setCylinder(int1, bool1);               //fahre zylinder auf zielstatus
        break;
      //quittiere zyklusende oder gebe fehler zurück
      case 'C':
        //wenn referenzfahrt immernoch aktiv ist gebe status 504 aus, für zyklusende
        if (Merker_Referenzfahrt_Gefahren) handleStatus(504);
        //ansonsten setze den letzten status zurück um erneute fehlermeldung zu ermöglichen (bugfix)
        else lastStatus = 0;
        break;
      //unbekannter befehl
      default:
        handleStatus(430);                      //gebe fehlercode 430 aus, für befehl unbekannt
        break;
    }
    //gebe zum schluss die aktuelle position zurück
    printPos();
  }
  //ob der flush befehl überhaupt was macht ist mir bisher nicht bekannt, macht aber keine probleme :D
  Serial.flush(); //Serielle verbindung leerlaufen lassen (verhindert lästige bugs)
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
void printCharArray(char* VARIABLENNAME) {
  char myChar;                                                  //lege leere Variable für einzelne textzeichen an
  for (unsigned long k = 0; k < strlen_P(VARIABLENNAME); k++) { //widerhole schleife für jedes zeichen im text
    myChar = pgm_read_byte_near(VARIABLENNAME + k);             //lege das textzeichen in die variable ab
    Serial.print(myChar);                                       //gebe das textzeichen aus
  }
}

/**
  gibt Position der Analge über Seriell aus
  Aufbau Datensatz:
   P$[PositionInMM]
   Z$1,[PositionZylinder1]
   Z$2,[PositionZylinder2]
   Z$3,[PositionZylinder3]
   Z$4,[PositionZylinder4]
   Z$5,[PositionZylinder5]
   H$S[tatusMerkerReferenzfahrtGefahren]
*/
void printPos() {
  //gebe motor/schlittenposition aus
  Serial.print("*P$");
  Serial.println(motorPosition());

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
