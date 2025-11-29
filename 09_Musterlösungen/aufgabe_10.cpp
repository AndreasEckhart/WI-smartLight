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
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

// Loop-Funktion - wird ununterbrochen ausgeführt
void loop() {
    // hier kommt der Effekt-Code hin
    int rot = ring.Color(255, 0, 0);
    int gruen = ring.Color(0, 255, 0);
    int blau = ring.Color(0, 0, 255);
    int orange = ring.Color(255, 165, 0);
    int magenta = ring.Color(255, 0, 255);

    int buttonStatus = digitalRead(BUTTON_PIN);
    
    if (buttonStatus == LOW) {  // Taste gedrückt
        ring.fill(gruen); // alle LEDs grün
    } else {
        ring.clear();   // alle LEDs aus
    }

    ring.show();    // Änderungen anzeigen
  
}


/***********************************************************************************
 * hier kommt am Ende dein Effekt 1 hin - bis dahin einfach leer lassen
 ***********************************************************************************/
void deinEffekt1(int step) {

}