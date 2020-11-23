/**
  EMAG MESSESTAND, Arduino Programm
  Name: Serialtexts
  Purpose: Storing the Texts that will be send via serial on startup and on request
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.1.0 23/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/
#define Boot_Software_Info  "EMAG MESSESTAND, Arduino Mega, Softwarestand: V1.0.0"        //Starttext für die Serielle ausgabe im setup

/**
 Speichern der Textzeilen im Progmem, zur ausgabe der Char arrays wird folgende funktion genutzt, 
 es muss lediglich VARIABLENNAME ersetzt werden
 
 char myChar;                                              //lege leere Variable für einzelne textzeichen an
  for (unsigned long k = 0; k < strlen_P(VARIABLENNAME); k++) { //widerhole schleife für jedes zeichen im text
    myChar = pgm_read_byte_near(VARIABLENNAME + k);             //lege das textzeichen in die variable ab
    Serial.print(myChar);                                   //gebe das textzeichen aus
  }
 */

const char Boot_Text[] PROGMEM = R"(#
#******************************************************************
#*                                                                *
#*        Programm für den Die Steuerung des Messestandes         *
#*                                                                *
#*               J.Röring                18/11/2020               *
#*                   Ausgabe Hilfetext mit ?                      *
#*                                                                *
#******************************************************************
#Erklärung der Ausgaben:
# Zeilen die mit * beginnen sind als Maschinenlesbare daten formatiert,
#   und werden vom Pi verarbeitet, 
#   zur info welcher parameter was bedeutet, gebe "?2" ein
# Zeilen die mit # beginnen sind ausschließlich in der seriellen konsole lesbar
#   und werden vom PI ignoriert
#
# zeilen die mit § beginnen sind informationstexte die auch in der Bedienoberfläche erscheinen
#
)";

const char Help_Text_1[] PROGMEM = R"(#
#******************************************************************
#*                                                                *
#*                           Hilfetext                            *
#*                           Seite 1/3                            *
#*                            Befehle                             *
#*                                                                *
#******************************************************************
#Ausgabe nächster seite mit "?2"
#
#Steuerbefehle:
#Syntax   Beschreibung
#------   ------------
#?[seite] zeige Hilfemenü, mit jeweiliger Seitenangabe
#H        fahre referenz
#P        frage Position der Anlage ab
#A[x]     fahre absolut auf pos X in mm
#R[x]     fahre relativ um pos X in mm
#D[x]     warte Zeit X in ms
#E[x],[y] stoße Zylinder nr X für zeit Y aus
#Z[x],[y] steuere Zylinder X auf zustand Y an (y=1 -> ausfahren; y=0 -> einfahren)
#C        gebe code 504 zurück für Zyklusende (als feedback für RPI)
#******************************************************************
)";

const char Help_Text_2[] PROGMEM = R"(#
#******************************************************************
#*                                                                *
#*                           Hilfetext                            *
#*                           Seite 2/3                            *
#*                           Feedback                             *
#*                                                                *
#******************************************************************
#Ausgabe nächster seite mit "?3"
#
#Feedbackdaten (Arduino -> RPI):
#Aufbau: *[Parameter]$[wert1],[wert2],[...]
#
#parameter          beschreibung
#---------          ------------
#*F$[Statuscode]    gibt Status und fehlercodes aus, für eine Liste der codes siehe seite 3
#*K$[x]             taster nr x am arduino gedrückt (derzeit nur einer vorhanden)
#*P$[pos]           gibt Position des Schlittens in mm aus
#*Z$[x],[y]         gibt zustand y des zylinders x aus (y=1 -> ausgefahren; y=0 -> eingefahren)
#*H$[status]        gibt status des referenzmerkers aus (1 = referenz gefahren)
#*A$[x]             fahrbefehl fahre absolut um x mm empfangen
#*R$[x]             fahrbefehl fahre relativ um x mm empfangen
#*D$[x]             befehl warte x ms empfangen
#*E$[x],[y]         befehl fahre zylinder x für y ms aus empfangen
#******************************************************************
)";

const char Help_Text_3[] PROGMEM = R"(#
#******************************************************************
#*                                                                *
#*                           Hilfetext                            *
#*                           Seite 3/3                            *
#*                          Statuscodes                           *
#*                                                                *
#******************************************************************
#Statuscode     typ         Beschreibung
#----------     ---         ------------
#Gruppe 400:
#   401         INFO        Arduino neu gestartet, Referenz fahren!
#   402         Fehler      Steuerspannung fehlt, Notaus betätigt?
#   403                     Veraltet, seid V0.3.8 nicht belegt (ehem. motorfreigabe fehlt)
#   404         Fehler      Referenzfahrt fehlt
#   405         Fehler      Referenzfahrt fehlgeschlagen
#   406         Fehler      Endschalter betätigt
#   407                     Veraltet, nicht belegt
#   408         Fehler      Ini bei Referenzfahrt nicht gefunden
#   409         Fehler      Referenzpunkt verloren, Referenzfahrt wird benötigt
#
#   410         Fehler      Zylinder 1 Nicht eingefahren, Fahrbefehl nicht akzeptiert
#   411         Fehler      Zylinder 2 Nicht eingefahren, Fahrbefehl nicht akzeptiert
#   412         Fehler      Zylinder 3 Nicht eingefahren, Fahrbefehl nicht akzeptiert
#   413         Fehler      Zylinder 4 Nicht eingefahren, Fahrbefehl nicht akzeptiert
#   414         Fehler      Zylinder 5 Nicht eingefahren, Fahrbefehl nicht akzeptiert
#
#   420         Fehler      Zielposition zu klein, Fahrbefehl nicht akzeptiert
#   421         Fehler      Zielposition zu groß, Fahrbefehl nicht akzeptiert
#
#   430         Fehler      Befehl unbekannt
#
#Gruppe 500:
#   501         Informell   Anlage Bereit
#   502         Informell   Referenzfahrt beendet
#   503         Informell   Referenzfahrt gestartet
#   504         Informell   Zyklus beendet, folgt auf C
#
#******************************************************************
)";
