/***********************************************************************************
 * ⚠️ ACHTUNG: DIESE DATEI ERST AB FUNKTION getDeinName() BEARBEITEN! ⚠️
 * 
 * Diese Datei enthält nur die Funktions-Signaturen.
 * Implementiere deine Effekte ausschließlich in src/user_effects.cpp
 * 
 * HTL-Anichstrasse, Innsbruck / (c)2025 Andreas Eckhart
 ***********************************************************************************/
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Zugriff auf den LED-Streifen aus main.cpp
extern Adafruit_NeoPixel ring;

// Zugriff auf die Effekt-Geschwindigkeit aus main.cpp
// Du kannst effectSpeed in deinen Funktionen ändern (z.B. effectSpeed = 100 für langsamer)
// Standardwert ist 50 (Millisekunden zwischen Updates)
extern int effectSpeed;

// HINWEIS ZUM PROGRAMMIEREN
// --------------------------------------------------
// Hier schreibst du DEINEN Code für deine persönlichen drei Effekte.
// Die Funktionen werden von der Haupt-Logik in main.cpp regelmäßig aufgerufen.
// Du kannst mit dem Button durch die Effekte schalten, bis "Dein Effekt 1/2/3" erscheint.
//
// Regeln:
// - Keine blockierenden Delays verwenden (delay()), die Hauptschleife übernimmt das Timing.
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

/***********************************************************************************
 * ✅ AB HIER DARFST DU BEARBEITEN! ✅
 ***********************************************************************************/
// DEIN NAME - ÄNDERE DIESEN WERT!
// Wird im Access Point Namen und Webinterface angezeigt
String getDeinName() {
  String deinName = "Andreas";  // ändere hier deinen Namen
  return deinName;
}

void deinEffekt1(int step) {
  // Beispiel 1: Lauflicht in Rot
  // Du kannst die Geschwindigkeit anpassen mit: effectSpeed = 100; (langsamer) oder effectSpeed = 20; (schneller)
  effectSpeed = 50;
  int n = ring.numPixels();
  int pos = step % n;
  ring.clear();
  ring.setPixelColor(pos, ring.Color(255, 0, 0));
}

void deinEffekt2(int step) {
  // Beispiel 2: Pulsierende Helligkeit
  // Die Helligkeit aller LEDs pulsiert sanft zwischen dunkel und hell
  effectSpeed = 30; // Geschwindigkeit des Pulsierens
  
  int n = ring.numPixels();
  
  // Berechne Helligkeit basierend auf Sinusfunktion (0-255)
  // step wird durch 50 geteilt für langsamere Pulsierung
  float brightness = (sin(step / 50.0 * PI) + 1.0) / 2.0; // Wert zwischen 0 und 1
  int brightnessValue = (int)(brightness * 255); // Wert zwischen 0 und 255
  
  // Alle LEDs mit der berechneten Helligkeit in Blau setzen
  for (int i = 0; i < n; i++) {
    ring.setPixelColor(i, ring.Color(0, 0, brightnessValue));
  }
}

void deinEffekt3(int step) {
  // Beispiel 3: Color-Wipe ähnlich – eine grüne "Front" läuft voran, dahinter geht's aus
  // Hier wird eine lokale Variable "speed" verwendet, aber du kannst auch directSpeed direkt ändern
  effectSpeed = 75; // Beispiel: Diesen Effekt langsamer machen
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
