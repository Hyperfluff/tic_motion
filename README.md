# tic-motion README

In dieser Datei sind grundlegende Informationen zum Programm der Maschine vermerkt
das Projekt ist die Steuerung des Messestandes der EMAG Zerbst

- **Autor:** Johannes Röring
- **Programmiersprachen:** Arduino, HTML, Python
- **verwendete Hardware:** Raspberry Pi (3b+), Arduino Mega

## Abhängigkeiten

dieses Projekt basiert auf der Library ["Flexysteper"](https://github.com/Stan-Reifel/FlexyStepper) von ["Stan-Reifel"](https://github.com/Stan-Reifel) welche hier zu JRStep geändert wurde 

## Verwendung

1. Repository klonen `git clone https://github.com/Joro28062002/tic_motion`
2. `main_sketch.ino` aus Ordner `main_sketch` öffnen
3. Programm kompilieren und auf Arduino Hochladen
4. Seriellen Monitor des Arduinos öffnen und den Anweisungen folgen um Manuell zu steuern

## Funktion der Anlage

Die Anlage verfügt über einen Schrittmotor welcher eine Lineare Achse bewegt, sowie 4 Zylinder welche einzelne TICTAC Schachteln Auf den Schlitten schieben und einen Zylinder welcher am anderen Ende der Schiene diese Schachteln wieder abwirft.
diese Elemente werden durch einen Arduino Mega angesteuert welcher seine Befehle durch den Raspberry Pi oder Manuell über die Serielle Konsole erhält

## Bei Fragen und Problemen:

Bei Fragen und Problemen mit dem Projekt binn ich sowohl per E-Mail unter Projects@jroering.com sowie Telefonisch erreichbar
diese Kontaktdaten können auch auf meiner Homepage [jroering.com](https://www.jroering.com) unter [Contact](https://jroering.com/contact/) gefunden werden
