#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Zugriff auf den LED-Streifen aus main.cpp
extern Adafruit_NeoPixel ring;

// Zugriff auf die Effekt-Geschwindigkeit aus main.cpp
// Du kannst effectSpeed in deinen Funktionen ändern (z.B. effectSpeed = 100 für langsamer)
// Standardwert ist 50 (Millisekunden zwischen Updates)
extern int effectSpeed;

// DEIN NAME - ÄNDERE DIESEN WERT!
// Wird im Access Point Namen und Webinterface angezeigt
// Beispiel: return "MaxMustermann";
String getDeinName() {
  String deinName = "Andreas";  // ändere hier deinen Namen
  return deinName;
}

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
// Geschwindigkeit anpassen:
// - Du kannst die Variable "effectSpeed" ändern, um die Animationsgeschwindigkeit anzupassen.
// - Kleinere Werte = schneller, größere Werte = langsamer (Wert in Millisekunden)
// - Beispiel: effectSpeed = 100; (für langsamere Animation)
// - Beispiel: effectSpeed = 20; (für schnellere Animation)
//
// Tipp: Für Animationen kannst du z.B. mit (step % ring.numPixels()) arbeiten.

void deinEffekt1(int step) {
  // Beispiel 1: Lauflicht in Rot
  // Du kannst die Geschwindigkeit anpassen mit: effectSpeed = 100; (langsamer) oder effectSpeed = 20; (schneller)
  effectSpeed = 500;
  int n = ring.numPixels();
  int pos = step % n;
  ring.clear();
  ring.setPixelColor(pos, ring.Color(255, 0, 0));
}

void deinEffekt2(int step) {
  // Beispiel 2: Jede zweite LED soll in einer anderen Farbe leuchten.
  // Du kannst die Geschwindigkeit anpassen mit: effectSpeed = 100; (langsamer) oder effectSpeed = 20; (schneller)
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
  // Hier wird eine lokale Variable "speed" verwendet, aber du kannst auch directSpeed direkt ändern
  effectSpeed = 250; // Beispiel: Diesen Effekt langsamer machen
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
    effectSpeed = 50; // Effekt hier schneller machen
    int end = (n * 2 - 1) - pos;
    for (int i = 0; i <= end; i++) {
      ring.setPixelColor(i, ring.Color(0, 255, 60));
    }
  }
}
