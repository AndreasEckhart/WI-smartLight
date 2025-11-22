/***********************************************************************************
 * Teil 1: Grundlagen
 * WI-Schnuppertage 2025 / 2026
 ***********************************************************************************
 * Einführung in die Programmierung mittels ESP32 und LED-Ring
 * HTL-Anichstrasse, Innsbruck
 * Wirtschaftsingenieure - Betriebsinformatik / (c)2025 Andreas Eckhart
 ***********************************************************************************/

#include "helper.h"   // Alle Hilfsfunktionen und Hardware-Definitionen

/***********************************************************************************
 * ✅ AB HIER KANNST DU BEARBEITEN! ✅
 ***********************************************************************************/

 // Setup-Funktion - wird einmal beim Start ausgeführt
void setup() {
    // hier kommt der Setup-Code hin
    ring.begin();
    ring.show();
    ring.setBrightness(BRIGHTNESS);
}

// Loop-Funktion - wird ununterbrochen ausgeführt
void loop() {
    // hier kommt der Effekt-Code hin

    // Prüft, ob genug Zeit vergangen ist (nicht-blockierend!)
    if (updateErforderlich()) {
        deinEffekt1(step); // deinen benutzerdefinierter Effekt aufrufen
        ring.show();  // Änderungen anzeigen
    }

}


/***********************************************************************************
 * hier kommt am Ende dein Effekt 1 hin - bis dahin einfach leer lassen
 ***********************************************************************************/
void deinEffekt1(int step) {

    // Farben definieren
    // int farbe = ring.Color(Rot, Grün, Blau);
    int rot = ring.Color(255, 0, 0);
    int gruen = ring.Color(0, 255, 0);
    int blau = ring.Color(0, 0, 255);
    int magenta = ring.Color(255, 0, 255);

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
        ring.setPixelColor(pos, magenta); // Magenta
    }
}