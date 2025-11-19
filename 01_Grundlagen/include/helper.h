/***********************************************************************************
 * ⚠️ ACHTUNG: DIESE DATEI NICHT BEARBEITEN! ⚠️
 * 
 * Diese Datei enthält vordefinierte Hilfsfunktionen und Hardware-Konfiguration.
 * Alle deine Änderungen gehören in die main.cpp Datei!
 * 
 * HTL-Anichstrasse, Innsbruck / (c)2025 Andreas Eckhart
 ***********************************************************************************/

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Hardware Definitionen
#define NEOPIXEL_PIN 3      // Pin für den LED-Ring
#define NEOPIXEL_COUNT 24   // Anzahl der LEDs im Ring
#define BRIGHTNESS 20       // Standard LED Helligkeit (0-255)
#define STATUS_LED_PIN 2    // Pin für die Status-LED
#define BUTTON_PIN 9        // Pin für den Taster

// LED-Ring Initialisierung
Adafruit_NeoPixel ring(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Variablen für Timing (intern verwendet)
unsigned long _lastUpdate = 0;

// Geschwindigkeit der Animation (änderbar!)
// Kleinere Werte = schneller, größere Werte = langsamer (in Millisekunden)
int effectSpeed = 20;

// Schrittzähler für Animationen (änderbar!)
unsigned int step = 0;

// Prüft ob genug Zeit vergangen ist und gibt true zurück wenn ja
bool updateErforderlich() {
  if (millis() - _lastUpdate >= effectSpeed) {
    _lastUpdate = millis();
    step++;
    return true;
  }
  return false;
}

// Funktions-Signatur für deinen Effekt 1
void deinEffekt1(int step);