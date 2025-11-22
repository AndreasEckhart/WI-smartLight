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
    // Geschwindigkeit festlegen (alle 50 ms ein Update)
    effectSpeed = 50;

    // Anzahl der LEDs ermitteln (24 bei unserem Ring)
    int n = ring.numPixels();
    
    // Aktuelle Position berechnen
    // step wird automatisch hochgezählt bei jedem updateErforderlich()
    // Modulo (%) sorgt für Endlosschleife: 0, 1, 2, ..., 23, 0, 1, ...
    int pos = step % n;
    
    // Alle LEDs ausschalten (wichtig für den Wander-Effekt)
    ring.clear();
    
    // Nur die LED an Position 'pos' auf Rot setzen
    ring.setPixelColor(pos, ring.Color(255, 0, 0)); // RGB: Rot=255, Grün=0, Blau=0
}