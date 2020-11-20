/**
  EMAG MESSESTAND, Arduino Programm
  Name: Settings
  Purpose: Storing all quick-access user configurable values
  Arduino IDE Version: 1.8.10

  @author Johannes Röring
  @version 1.0.1 20/11/20

  the following scripts will all be documented in german,
  for international use as well as translations and questions,
  feel free to contact me

  contact: mail@jroering.com
*/
//--Kommunikation--
#define SERIAL_BAUD 115200                          // Übertragungsgeschwindigkeit der Seriellen Schnittstelle in Bits / Sekunde

//--Sicherheitsmechanismen--
#define C_zylinderUeberwachen true                  // Schaltet Endlagenüberwachung der Zylinder ein

//--Motor--
const float C_homePosInMM = 1100;                     // Position die nach Referenzfahrt angefahren wird
const float C_minPosInMM = 0;                         // Unterer Grenzwert Motorposition in mm
const float C_maxPosInMM = 1200;                      // Oberer Grenzwert Motorposition in mm
const float C_schritteProMM = 16.39;                // Anzahl an Schritten des Motors pro mm verfahrstrecke
const float C_schritteProUmdrehung = 1024 * 1.25;   // Anzahl an Motorschritten um die welle des Riemens einmal zu drehen (Untersetzung 1,25:1)
#define C_ref_directionTowardsHome 1                // Drehrichtung des Motors um Ini anzufahren, 1 für Motor linkslauf, -1 für motor Rechtslauf
const int C_ref_maximaleReferenzstreckeInMM = 1250; // Maximale länge der Referenzfahrt in mm, darüber wird abgebrochen
float V_ref_nullpunktversatzInMM = 0;                 // Verschiebung des Nullpunktes vom Ini in mm
const int C_ref_speed = 250;                        // Maximalgeschwindigkeit in mm/s bei Referenzfahrt
const int C_speed = 250;                            // Maximalgeschwindigkeit in mm/s
const int C_beschleunigung = 1000;                  // Beschleunigung des Motors in mm/s²

//--Zylinder Überwachung--
const int C_zylEinfahrdauer = 500;                  // Zeit zum einfahren der Zylinder in ms

//--Optik--
const long C_ref_blink_Interval = 250;              //Blinkintervall der Meldeleuchte wenn keine Referenz gefahren ist

//Systemwerte, NICHT BEARBEITEN ohne konkreten grund!!!
#define SERIAL_RX_BUFFER_SIZE 256                   //Systemvariable für Seriellen eingangsbuffer, nicht ändern
