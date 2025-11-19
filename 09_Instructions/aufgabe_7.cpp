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
    int rot = ring.Color(255, 0, 0);
    int gruen = ring.Color(0, 255, 0);
    int blau = ring.Color(0, 0, 255);
    int orange = ring.Color(255, 165, 0);

    // BLOCKIERENDE Variante: Helligkeit von 0 bis 100 erhöhen
    // Diese Variante nutzt eine for-Schleife und delay(), dadurch reagiert

    ring.fill(rot, 0, 8); // Alle LEDs auf Rot vorbereiten
    ring.fill(gruen, 8, 8); // Alle LEDs auf Grün vorbereiten
    ring.fill(blau, 16, 8); // Alle LEDs auf Blau vorbereiten

    // Helligkeit schrittweise erhöhen
    for (int hell = 1; hell <= 100; hell++) {
        ring.setBrightness(hell); // Globale Helligkeit setzen (0-255 möglich, hier 0-100)
        ring.show();              // Änderungen anwenden
        delay(30);                // BLOCKIEREND: 30 ms Pause pro Schritt (100 * 30ms ≈ 3s)
    }

    // Kurze Pause am Ende bevor Helligkeit wieder verringert wird
    delay(500);

    // Helligkeit schrittweise verringern
    for (int hell = 100; hell >= 1; hell--) {
        ring.setBrightness(hell); // Globale Helligkeit setzen (0-255 möglich, hier 0-100)
        ring.show();              // Änderungen anwenden
        delay(30);                // BLOCKIEREND: 30 ms Pause pro Schritt (100 * 30ms ≈ 3s)
    }

    // Kurze Pause am Ende bevor Zyklus neu startet
    delay(500);
}


// hier kommt am Ende dein Effekt 1 hin
// bis dahin einfach leer lassen
void deinEffekt1(int step) {

}