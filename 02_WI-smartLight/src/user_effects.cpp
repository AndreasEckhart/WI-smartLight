#include <Adafruit_NeoPixel.h>

// Zugriff auf den LED-Streifen aus main.cpp
extern Adafruit_NeoPixel ring;

// HINWEIS ZUM PROGRAMMIERKURS
// --------------------------------------------------
// Hier schreibst du DEINEN Code für deine persönlichen drei Effekte.
// Die Funktionen werden von der Haupt-Logik in main.cpp regelmäßig aufgerufen.
// Du kannst mit dem Button durch die Effekte schalten, bis "Dein Effekt 1/2/3" erscheint.
//
// Regeln:
// - Keine blockierenden Delays (delay(...) vermeiden), die Hauptschleife übernimmt das Timing.
// - KEIN ring.show() hier aufrufen – das erledigt main.cpp nach deinem Funktionsaufruf.
// - Nutze "step" als fortlaufenden Zähler.
// - Verwende ring.numPixels(), ring.setPixelColor(i, r,g,b) oder ring.Color(r,g,b).
// - Farben sind im Format RGB (0-255).
//
// Tipp: Für Animationen kannst du z.B. mit (step % ring.numPixels()) arbeiten.

void deinEffekt1(int step) {
  // Beispiel 1: Lauflicht in Rot
  int n = ring.numPixels();
  int pos = step % n;
  ring.clear();
  ring.setPixelColor(pos, ring.Color(255, 0, 0));
}

void deinEffekt2(int step) {
  // Beispiel 2:Jede zweite LED soll in einer anderen Farbe leuchten.
  int n = ring.numPixels();
  ring.clear();
  for (int i = 0; i < n; i++) {
    if (i % 2 == 0) {
      ring.setPixelColor(i, ring.Color(0, 0, 255)); // Blau
    } else {
      ring.setPixelColor(i, ring.Color(255, 255, 0)); // Gelb
    }
  }
}

void deinEffekt3(int step) {
  // Beispiel 3: Color-Wipe ähnlich – eine grüne "Front" läuft voran, dahinter geht's aus
  int n = ring.numPixels();
  int speed = 2; // je größer, desto langsamer
  int pos = (step / speed) % (n * 2);
  ring.clear();
  if (pos < n) {
    // Aufbauphase: Pixel 0..pos grün füllen
    for (int i = 0; i <= pos; i++) {
      ring.setPixelColor(i, ring.Color(0, 255, 60));
    }
  } else {
    // Abbauphase: Pixel 0..(2n-pos) grün lassen, Rest aus
    int end = (n * 2 - 1) - pos;
    for (int i = 0; i <= end; i++) {
      ring.setPixelColor(i, ring.Color(0, 255, 60));
    }
  }
}
