/***********************************************************************************
 *
 * Teil 1: Grundlagen
 * WI-Schnuppertage 2025 / 2026
 * 
 ***********************************************************************************
 * Einführung in die Programmierung mittels ESP32 und LED-Ring
 * HTL-Anichstrasse, Innsbruck
 * Wirtschaftsingenieure - Betriebsinformatik / (c)2025 Andreas Eckhart
 ***********************************************************************************/

#include "helper.h"   // Alle Hilfsfunktionen und Hardware-Definitionen

// Setup-Funktion - wird einmal beim Start ausgeführt
void setup() {
  ring.begin();                     // NeoPixel starten
  ring.show();                      // Alle Pixel ausschalten
  ring.setBrightness(BRIGHTNESS);   // Helligkeit der LEDs festlegen (0-255)
}

// Loop-Funktion - wird ununterbrochen ausgeführt
void loop() {
  // Prüfen ob genug Zeit vergangen ist für nächste Aktualisierung (ohne blockierendes delay!)
  if (updateErforderlich()) {
    
    /////////////////////////////////////////////
    // ab hier kannst du deinen Code schreiben //
    /////////////////////////////////////////////

    // Hier kommt dein Effekt-Code:
    // Farben definieren
    // int farbe = ring.Color(Rot, Grün, Blau);
    int rot = ring.Color(255, 0, 0);
    int gruen = ring.Color(0, 255, 0);
    int blau = ring.Color(0, 0, 255);
    int gelb = ring.Color(255, 255, 0);
    // füge hier beliebige weitere Farben hinzu falls benötigt
    // int lila = ???
    //

    // Beispiel: Lauflicht in verschiedenen Farben
    int pos = step % ring.numPixels();
    ring.clear();
    
    // Farbe wechselt alle 24 Schritte
    if ((step / ring.numPixels()) % 4 == 0) {
      effectSpeed = 100; // Mittel
      ring.setPixelColor(pos, rot); // Rot
    } else if ((step / ring.numPixels()) % 4 == 1) {
      effectSpeed = 20; // Schnell
      ring.setPixelColor(pos, gruen); // Grün
    } else if ((step / ring.numPixels()) % 4 == 2) {
      effectSpeed = 250; // Langsamer
      ring.setPixelColor(pos, blau); // Blau
    } else {
      effectSpeed = 50; // Mittel
      ring.setPixelColor(pos, gelb); // Gelb
    }
    

    //////////////////////////////////////////////
    // bis hier kannst du deinen Code schreiben //
    //////////////////////////////////////////////
    ring.show();  // Änderungen anzeigen

  }

}