/***********************************************************************************
 * Teil 1: Grundlagen - Aufgabe 14
 * WI-Schnuppertage 2025 / 2026
 ***********************************************************************************
 * Effekt: "Knight Rider Scanner" – Ein heller Punkt fährt hin und her,
 * gefolgt von einem "Schweif" aus 2 nachleuchtenden schwächeren Punkten.
 * Dieser Effekt ist super beliebt bei Jugendlichen! 🚗💨
 *
 * Lernziele:
 *  - Richtungswechsel (hin und zurück) mit einer Variable
 *  - Helligkeitsabstufung für Schweif-Effekt
 *  - Nicht-blockierende Animation mit updateErforderlich()
 *  - Arbeiten mit Positionen und Grenzen
 *
 * Erweiterungsideen:
 *  - Längerer Schweif (3-4 LEDs statt 2)
 *  - Andere Farbe (z.B. Blau für "Blue Thunder")
 *  - Variable Geschwindigkeit (langsamer an den Enden)
 *  - Mehrere Scanner gleichzeitig
 ***********************************************************************************/

#include "helper.h"

void setup() {
  ring.begin();
  ring.show();
  ring.setBrightness(BRIGHTNESS);
}

void loop() {
  effectSpeed = 60;
  
  if (updateErforderlich()) {
    int n = ring.numPixels();
    
    // Position berechnen: hin (0..23) und zurück (23..0) = insgesamt 46 Schritte
    int cycle = step % (n * 2 - 2);  // 0..46 für 24 LEDs
    int pos = (cycle < n) ? cycle : (n * 2 - 2 - cycle);  // Dreieck-Welle
    
    ring.clear();
    
    // Schweif (2 LEDs schwächer) - Position ergibt Richtung
    int dir = (cycle < n) ? -1 : 1;
    ring.setPixelColor(pos + dir, ring.Color(100, 0, 0));
    ring.setPixelColor(pos + dir * 2, ring.Color(40, 0, 0));
    
    // Hauptpunkt (hell, rot)
    ring.setPixelColor(pos, ring.Color(255, 0, 0));
    ring.show();
  }
}

/*
 * Erweiterungen zum Ausprobieren:
 *
 * 1) Längerer Schweif (3 oder 4 LEDs):
 *    int trail3 = scannerPos - (scannerDirection * 3);
 *    if (trail3 >= 0 && trail3 < total) {
 *      ring.setPixelColor(trail3, ring.Color(255 * 0.08, 0, 0));
 *    }
 *
 * 2) Andere Farbe (z.B. Cyan für einen coolen Look):
 *    scannerColor = ring.Color(0, 255, 255);
 *    // Schweif anpassen: ring.Color(0, g_wert, g_wert)
 *
 * 3) Variable Geschwindigkeit (langsamer an den Enden):
 *    int distanceFromCenter = abs(scannerPos - total/2);
 *    effectSpeed = 40 + distanceFromCenter * 3;
 *
 * 4) Zwei Scanner gleichzeitig (einer von links, einer von rechts):
 *    Zweite Position-Variable anlegen und beide unabhängig bewegen.
 *
 * 5) Regenbogen-Scanner:
 *    uint16_t hue = (step * 500) % 65536;
 *    scannerColor = ring.ColorHSV(hue);
 */
