/***********************************************************************************
 * Teil 1: Grundlagen - Aufgabe 11
 * WI-Schnuppertage 2025 / 2026
 ***********************************************************************************
 * Ziel: Einen einfachen Effekt programmieren, der die erste Hälfte des Rings in
 * einer Farbe und die zweite Hälfte in einer anderen Farbe zeigt – und diese
 * Farben regelmäßig vertauscht (Blink-/Wechsel-Effekt ohne delay()).
 *
 * Lernziele:
 *  - Verwendung von updateErforderlich() für nicht-blockierende Animationen
 *  - Arbeiten mit ring.fill(farbe, startIndex, count)
 *  - Einfache Zustandslogik über step
 ***********************************************************************************
 * Anleitung:
 *  1. Schau dir effectSpeed an – je kleiner, desto schneller.
 *  2. Ändere die Farben (RGB: ring.Color(R,G,B)).
 *  3. Passe das Wechselintervall an (cyclesPerChange).
 *  4. Optional: Füge eine dritte Farbe hinzu (z. B. mittlerer Bereich).
 ***********************************************************************************/

#include "helper.h"

// Setup wird einmalig ausgeführt
void setup() {
  ring.begin();      // NeoPixel initialisieren
  ring.show();       // Alle ausschalten
  ring.setBrightness(BRIGHTNESS); // Grundhelligkeit setzen (in helper.h definiert)
}

// Loop wird ständig wiederholt
void loop() {
  // Geschwindigkeit der Aktualisierung (ms). Ändere den Wert für schneller/langsamer.
  effectSpeed = 150; // alle 150 ms nächste Prüfung

  // Nur wenn genug Zeit vergangen ist, Animation fortsetzen
  if (updateErforderlich()) {
    int total = ring.numPixels();     // Anzahl LEDs (24)
    int half  = total / 2;            // 12 pro Hälfte

    // Farben definieren (beliebig änderbar)
    uint32_t farbeLinks  = ring.Color(255, 0, 255);   // Pink/Violett
    uint32_t farbeRechts = ring.Color(0, 0, 255);     // Blau

    // Wie oft ein Zustand bestehen bleibt (Anzahl Schritte)
    int cyclesPerChange = 10; // nach 10 Schritten wechseln

    // Ermitteln ob wir im "normalen" oder "vertauchten" Zustand sind
    bool swapped = ((step / cyclesPerChange) % 2) == 1;

    // Ring leeren (nicht zwingend nötig, aber klarer)
    ring.clear();

    if (!swapped) {
      // Zustand A: Links Pink, Rechts Blau
      ring.fill(farbeLinks, 0, half);
      ring.fill(farbeRechts, half, half);
    } else {
      // Zustand B: Farben vertauscht
      ring.fill(farbeRechts, 0, half);
      ring.fill(farbeLinks, half, half);
    }

    // Änderungen anzeigen
    ring.show();
  }
}

/*
 * Ideen für Erweiterungen:
 *  - Andere Farben ausprobieren (z. B. ring.Color(255,165,0) für Orange)
 *  - Drei Bereiche: Teile total in Drittel und nutze drei Farben.
 *  - cyclesPerChange verkleinern oder vergrößern für schneller/langsamer.
 *  - brightness dynamisch ändern: ring.setBrightness((step % 50) + 10);
 */
