#include <Adafruit_NeoPixel.h>

// Hardware Definitionen
#define NEOPIXEL_PIN 3
#define NEOPIXEL_COUNT 24

// NeoPixel Initialisierung
Adafruit_NeoPixel ring(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.show(); // Alle Pixel ausschalten
  ring.setBrightness(20); // Helligkeit auf 20% setzen
}

void loop() {
  // Beispiel: Einfachen Farbwechsel-Effekt anzeigen
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(255, 0, 0)); // Rot
    ring.show();
    delay(100);
    ring.setPixelColor(i, ring.Color(0, 255, 0)); // Grün
    ring.show();
    delay(100);
    ring.setPixelColor(i, ring.Color(0, 0, 255)); // Blau
    ring.show();
    delay(100);
    ring.setPixelColor(i, ring.Color(0, 0, 0)); // Aus
    ring.show();
  }
}