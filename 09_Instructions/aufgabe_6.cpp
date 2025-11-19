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
  for (int i = 0; i < 24; i++) {

    // 1. Pixel an Position 'i' auf die gewünschte Farbe setzen (z.B. rot)
    ring.setPixelColor(i, ring.Color(255, 0, 0));
    
    // 2. Die Änderungen anzeigen (LED leuchtet auf)
    ring.show();

    // 3. Kurz warten (250ms), damit das Auge das nacheinander Leuchten sieht
    delay(250);
  }

  // 4. Wenn alle LEDs an sind alle ausschalten
  ring.clear();
  ring.show();
  delay(1000);
  
}


// hier kommt am Ende dein Effekt 1 hin
// bis dahin einfach leer lassen
void deinEffekt1(int step) {

}