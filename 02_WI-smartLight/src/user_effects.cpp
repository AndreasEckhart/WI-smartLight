/***********************************************************************************
 * ⚠️ ACHTUNG: DIESE DATEI ERST AB FUNKTION getDeinName() BEARBEITEN! ⚠️
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
  int magenta = ring.Color(255, 0, 255);
  ring.fill(magenta); // Alle Magenta/Pink (Rot + Blau)
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
