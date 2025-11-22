/***********************************************************************************
 * Teil 1: Grundlagen - Aufgabe 13
 * WI-Schnuppertage 2025 / 2026
 ***********************************************************************************
 * Effekt: "Sparkle" (Glitzern) – zufällig leuchtet jeweils genau eine LED kurz auf.
 * Kein delay(), vollständig nicht-blockierend über updateErforderlich().
 *
 * Lernziele:
 *  - Verwendung von random() für Zufall
 *  - Nicht-blockierende Animation (keine Verzögerung mit delay())
 *  - Einfacher Effekt mit sehr wenig Code
 *
 * Erweiterungsideen (nach Grundversion):
 *  - Mehrere gleichzeitige zufällige Pixel
 *  - Zufällige Farbe statt fester Farbe
 *  - Nachleuchten (Fade-Out) mit Helligkeitsreduktion
 ***********************************************************************************/

#include "helper.h"

// Feste Grundfarbe für das Funkeln (Pink/Violett)
uint32_t sparkleColor = ring.Color(255, 0, 255);

void setup() {
  ring.begin();
  ring.show();
  ring.setBrightness(BRIGHTNESS); // Grundhelligkeit aus helper.h
  randomSeed(analogRead(0));      // Zufallsstart (optional)
}

void loop() {
  // Geschwindigkeit der Funken (je kleiner, desto schneller)
  effectSpeed = 120; // alle ~120 ms neuer Funke

  if (updateErforderlich()) {
    ring.clear(); // Vorherige LED ausschalten

    // Zufällige Position wählen: Wertebereich 0..(Anzahl LEDs - 1)
    int pos = random(ring.numPixels());

    // LED an zufälliger Position einschalten
    ring.setPixelColor(pos, sparkleColor);

    // Anzeige aktualisieren
    ring.show();
  }
}

/* Erweiterungen:
 * 1) Zufällige Farbe:
 *    uint32_t c = ring.Color(random(256), random(256), random(256));
 *    ring.setPixelColor(pos, c);
 *
 * 2) Drei zufällige Pixel:
 *    for (int i = 0; i < 3; i++) {
 *      int p = random(ring.numPixels());
 *      ring.setPixelColor(p, sparkleColor);
 *    }
 *
 * 3) Fade-Out statt sofortigem Löschen:
 *    - Eigenes Array für Helligkeitswerte anlegen und pro Schritt reduzieren.
 *
 * 4) Geschwindigkeit dynamisch ändern:
 *    effectSpeed = 30 + (step % 5) * 40; // wechselt zwischen mehreren Stufen
 */
