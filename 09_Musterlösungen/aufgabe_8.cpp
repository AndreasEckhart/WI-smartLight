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
    int magenta = ring.Color(255, 0, 255);
    int aus = ring.Color(0, 0, 0);

    // Nacheinander alle 24 LEDs einschalten 0 bis 23
    for (int led = 0; led < 24; led++) {

        // 1. Pixel an Position 'i' auf die gewünschte Farbe setzen (z.B. rot)
        ring.setPixelColor(led, rot);

        // 2. Die Änderungen anzeigen (LED leuchtet auf)
        ring.show();

        // 3. Kurz warten (250ms), damit das Auge das nacheinander Leuchten sieht
        delay(100);
    }

    delay(500);
    
    // Nacheinander alle 24 LEDs einschalten 0 bis 23
    for (int led = 23; led >= 0; led--) {

        // 1. Pixel an Position 'i' auf die gewünschte Farbe setzen (z.B. rot)
        ring.setPixelColor(led, aus);

        // 2. Die Änderungen anzeigen (LED leuchtet auf)
        ring.show();

        // 3. Kurz warten (250ms), damit das Auge das nacheinander Leuchten sieht
        delay(100);
    }
    delay(500);

    // 4. Wenn alle LEDs an sind alle ausschalten
    ring.clear();
    ring.show();
    delay(500);

}


/***********************************************************************************
 * hier kommt am Ende dein Effekt 1 hin - bis dahin einfach leer lassen
 ***********************************************************************************/
void deinEffekt1(int step) {

}
