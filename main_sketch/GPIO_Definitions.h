/**
  EMAG MESSESTAND, Arduino Programm
  Name: GPIO_Definitions
  Purpose: Storing the pin assignments for all GPIO Pins, and initializing them in the function initGPIO();
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.0 19/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/

//  Eingänge
//  --Bedienpult--
#define E_Anlage_EIN  2 //  Eingang vom Überwachungskontakt für K1, betätigt durch Notaus
#define E_Bed_Automatik 12  //  Eingang vom Schalter Automatikbetrieb
#define E_Bed_Starte_Referenzfahrt  13  //  Eingang vom Taster Referenzfahrt starten
//  --Sensoren--
#define E_Ini_Referenz  3 //  Eingang vom Referenztaster
#define E_Ini_Endschalter 4 //  Eingang von Überwachungsrelais K2 & K3, überwachen Endschalter
//  --Sensoren - Zylinder Endlagenüberwachung--
#define E_Ini_Zyl_1 5 //  Eingang Überwachung Zylinder 1 eingefahren
#define E_Ini_Zyl_2 6 //  Eingang Überwachung Zylinder 2 eingefahren
#define E_Ini_Zyl_3 7 //  Eingang Überwachung Zylinder 3 eingefahren
#define E_Ini_Zyl_4 8 //  Eingang Überwachung Zylinder 4 eingefahren
#define E_Ini_Zyl_5 9 //  Eingang Überwachung Zylinder 5 eingefahren


//  Ausgänge
//  --Motor--
#define A_Mot_Freigabe  A0  //  Ausgang Motorfreigabe, HIGH = Motor freigegeben
#define A_Mot_Richtung  A1  //  Ausgang Motor Richtung, HIGH = Linkslauf
#define A_Mot_Takt  A2  //  Ausgang Motor Takt
//  --Relais--
#define A_Rly_Power 41  //  Ausgang Relais -K9, Stromversorgung Relais -K10 bis -K16
#define A_Rly_Zyl_1 42  //  Ausgang Relais -K10, Zylinder 1 ausfahren
#define A_Rly_Zyl_2 43  //  Ausgang Relais -K11, Zylinder 2 ausfahren
#define A_Rly_Zyl_3 44  //  Ausgang Relais -K12, Zylinder 3 ausfahren
#define A_Rly_Zyl_4 45  //  Ausgang Relais -K13, Zylinder 4 ausfahren
#define A_Rly_Zyl_5 46  //  Ausgang Relais -K14, Zylinder 5 ausfahren
#define A_Rly_Bed_Referenz  47  //  Ausgang Relais -K15, Meldeleuchte Referenzfahrt gefahren
#define A_Rly_8 48  //  Ausgang Relais -K16, nicht verwendet

/**
  initialisiert alle GPIO Pins auf den korrekten pinMode (Input, Output or Input with Pullup)
*/
void initGPIO() {
  //inputs
  pinMode(E_Anlage_EIN, INPUT_PULLUP);
  pinMode(E_Bed_Automatik, INPUT_PULLUP);
  pinMode(E_Bed_Starte_Referenzfahrt, INPUT_PULLUP);
  pinMode(E_Ini_Referenz, INPUT_PULLUP);
  pinMode(E_Ini_Endschalter, INPUT_PULLUP);
  pinMode(E_Ini_Zyl_1, INPUT_PULLUP);
  pinMode(E_Ini_Zyl_2, INPUT_PULLUP);
  pinMode(E_Ini_Zyl_3, INPUT_PULLUP);
  pinMode(E_Ini_Zyl_4, INPUT_PULLUP);
  pinMode(E_Ini_Zyl_5, INPUT_PULLUP);
  
  //outputs
  //pinMode(A_Mot_Richtung,OUTPUT);       //wird durch Library erledigt
  //pinMode(A_Mot_Takt,OUTPUT);           //wird durch Library erledigt
  pinMode(A_Mot_Freigabe, OUTPUT);
  pinMode(A_Rly_Power, OUTPUT);
  pinMode(A_Rly_Zyl_1, OUTPUT);
  pinMode(A_Rly_Zyl_2, OUTPUT);
  pinMode(A_Rly_Zyl_3, OUTPUT);
  pinMode(A_Rly_Zyl_4, OUTPUT);
  pinMode(A_Rly_Zyl_5, OUTPUT);
  pinMode(A_Rly_Bed_Referenz, OUTPUT);
  pinMode(A_Rly_8, OUTPUT);

  //ausgänge auf definierten status setzen
  digitalWrite(A_Mot_Freigabe, HIGH);       //Gebe den Motor Frei
  digitalWrite(A_Rly_Power, LOW);          //relais -K9 einschalten um 24V für -K10 bis -K16 freizugeben
  digitalWrite(A_Rly_Zyl_1, LOW);
  digitalWrite(A_Rly_Zyl_2, LOW);
  digitalWrite(A_Rly_Zyl_3, LOW);
  digitalWrite(A_Rly_Zyl_4, LOW);
  digitalWrite(A_Rly_Zyl_5, LOW);
  digitalWrite(A_Rly_Bed_Referenz, LOW);
  digitalWrite(A_Rly_8, LOW);
}
