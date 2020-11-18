/**
  EMAG MESSESTAND, Arduino Programm
  Name: Serialtexts
  Purpose: Storing the Texts that will be send via serial on startup and on request
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.0 18/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/
#define Boot_Software_Info  "EMAG MESSESTAND, Arduino Mega, Softwarestand: V1.0.0"        //Starttext für die Serielle ausgabe im setup
const String Boot_Text = R"(#
#******************************************************************
#*                                                                *
#*        Programm für den Die Steuerung des Messestandes         *
#*                                                                *
#*               J.Röring                18/11/2020               *
#*                   Ausgabe Hilfetext mit ?                      *
#*                                                                *
#******************************************************************
)";

const String Help_Text_1 = R"(#
#******************************************************************
#*                                                                *
#*                           Hilfetext                            *
#*                           Seite 1/1                            *
#*                                                                *
#******************************************************************
#Steuerbefehle:
#Syntax   Beschreibung
#------   ------------
#?        zeige diese Liste
#H        fahre referenz
#A[x]     fahre absolut auf pos X in mm
#R[x]     fahre relativ um pos X in mm
#D[x]     warte Zeit X in ms
#E[x],[y] stoße Zylinder nr X für zeit Y aus
#P        frage Position der Anlage ab
#Z[x],[y] steuere Zylinder X auf zustand Y an (y=1 -> ausfahren; y=0 -> einfahren)
#******************************************************************
)";
