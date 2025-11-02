/***********************************************************************************
 * WI-Schnuppertage 2025 / 2026
/***********************************************************************************
 * Einführung in die Programmierung mittels ESP32 und LED-Ring
 * HTL-Anichstrasse, Innsbruck
 * Wirtschaftsingenieure - Betriebsinformatik / (c)2025 Andreas Eckhart
 * ***********************************************************************************/

 // erforderliche Bibliotheken einbinden
#include <Adafruit_NeoPixel.h>

// Hardware Definitionen
#define NEOPIXEL_PIN 3      // Pin für den LED-Ring
#define NEOPIXEL_COUNT 24   // Anzahl der LEDs im Ring
#define BRIGHTNESS 20       // Standard LED Helligkeit (0-255)
#define STATUS_LED_PIN 2    // Pin für die Status-LED
#define BUTTON_PIN 9        // Pin für den Taster

// LED-Ring Initialisierung
Adafruit_NeoPixel ring(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


// Setup-Funktion - wird einmal beim Start ausgeführt
void setup() {
  ring.begin(); // NeoPixel starten
  ring.show();  // Alle Pixel ausschalten
  ring.setBrightness(BRIGHTNESS);   // Helligkeit der LEDs festlegen (0-255)
}

// Loop-Funktion - wird ununterbrochen ausgeführt
void loop() {
  ring.fill(ring.Color(255, 0, 0)); // Alle LEDs rot einschalten
  ring.show();                      // Änderungen anzeigen
  delay(1000);                      // 1 Sekunde warten
  ring.fill(ring.Color(0, 0, 0));   // Alle LEDs ausschalten
  ring.show();                      // Änderungen anzeigen
  delay(1000);                      // 1 Sekunde warten
  ring.fill(ring.Color(0, 255, 0)); // Alle LEDs grün einschalten
  ring.show();                      // Änderungen anzeigen
  delay(1000);                      // 1 Sekunde warten
  ring.fill(ring.Color(0, 0, 255)); // Alle LEDs blau einschalten
  ring.show();                      // Änderungen anzeigen
  delay(1000);                      // 1 Sekunde warten
  ring.fill(ring.Color(0, 0, 0));   // Alle LEDs ausschalten
}