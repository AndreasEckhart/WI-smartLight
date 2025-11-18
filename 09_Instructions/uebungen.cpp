/***********************************************************************************
 * Teil 1: Grundlagen
 * WI-Schnuppertage 2025 / 2026
 * Einführung in die Programmierung mittels ESP32 und LED-Ring
 ************************************************************************************
 * HTL-Anichstrasse, Innsbruck
 * Wirtschaftsingenieure - Betriebsinformatik / (c)2025 Andreas Eckhart
 ************************************************************************************
 * Du startest immer mit dem Code in setup() und entkommentierst dann 
 * schrittweise die Übungen 2 bis 12 in der loop() Funktion.
 ***********************************************************************************/

#include "helper.h"   // Alle Hilfsfunktionen und Hardware-Definitionen


// ===============================================
// HAUPTPROGRAMM-STRUKTUR
// ===============================================

// Setup-Funktion - wird einmal beim Start ausgeführt
void setup() {
  // ÜBUNG 1: EINE LED LEUCHTET
  ring.begin();                     // NeoPixel starten
  ring.show();                      // Alle Pixel ausschalten
  ring.setBrightness(BRIGHTNESS);   // Helligkeit festlegen (BRIGHTNESS ist in helper.h)

  // ÜBUNG 8 & 9: VORBEREITUNG HARDWARE
  pinMode(STATUS_LED_PIN, OUTPUT);     // Status-LED als Ausgang definieren
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // Taster als Eingang (Ruhezustand HIGH) definieren

  // Test der Status-LED (optional, kann nach Test gelöscht werden)
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(200);
  digitalWrite(STATUS_LED_PIN, LOW);
}

// Loop-Funktion - wird ununterbrochen ausgeführt
void loop() {
  
  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 1: EINE LED LEUCHTET
  // //////////////////////////////////////////////////////////////////
  // // Farben definieren
  // // int farbe = ring.Color(Rot, Grün, Blau);
  // int rot = ring.Color(255, 0, 0);
  // int gruen = ring.Color(0, 255, 0);
  // int blau = ring.Color(0, 0, 255);
  // int gelb = ring.Color(255, 255, 0);
  // // füge hier beliebige weitere Farben hinzu falls benötigt
  // // int orange = ???

  // ring.setPixelColor(0, ring.Color(255, 0, 0)); // LED 0 auf Rot setzen
  // // oder mit oben definierten Farben:
  // ring.setPixelColor(0, rot); // LED 0 auf Rot setzen
  // ring.setPixelColor(1, gruen); // LED 1 auf Grün setzen
  // ring.setPixelColor(2, blau); // LED 2 auf Blau setzen
  // ring.setPixelColor(3, orange); // LED 3 auf Orange
  // ring.show(); 


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 3: ALLE LEDS IN EINER FARBE (LÖSCHE/KOMMENTIERE ALLE VORHERIGEN ÜBUNGEN AUS)
  // //////////////////////////////////////////////////////////////////
  // ring.fill(ring.Color(0, 255, 0)); // Alle LEDs auf Grün
  // ring.show();


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 4: EINFACHER FARBWECHSEL (BLOCKIEREND MIT delay())
  // //////////////////////////////////////////////////////////////////
  // ring.fill(ring.Color(255, 100, 0)); // Orange
  // ring.show();
  // delay(1000); // Hält das gesamte Programm an!

  // ring.fill(ring.Color(0, 0, 255)); // Blau
  // ring.show();
  // delay(1000); // Hält das gesamte Programm an!


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 5: FARBWECHSEL (NICHT-BLOCKIEREND mit updateErforderlich)
  // //////////////////////////////////////////////////////////////////
  // // WICHTIG: Ändere effectSpeed in helper.h auf z.B. 1000ms
  // if (updateErforderlich()) { 
  //   if (step % 2 == 0) {
  //     ring.fill(ring.Color(255, 100, 0)); // Orange
  //   } else {
  //     ring.clear(); // Aus
  //   }
  //   ring.show();
  // }


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 6: EINZELNES PIXEL WANDERT
  // //////////////////////////////////////////////////////////////////
  // // WICHTIG: effectSpeed in helper.h auf z.B. 50ms setzen
  // if (updateErforderlich()) {
  //   ring.clear(); // Alles ausschalten
  //   
  //   // Modulo sorgt für das Zurückspringen von 23 auf 0
  //   int pos = step % ring.numPixels(); 
  //   
  //   ring.setPixelColor(pos, ring.Color(0, 255, 255)); // Cyan
  //   ring.show();
  // }


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 7: REGENBOGEN-WELLE (HSV)
  // //////////////////////////////////////////////////////////////////
  // // WICHTIG: effectSpeed in helper.h auf z.B. 20ms setzen
  // if (updateErforderlich()) { 
  //   for(int i = 0; i < ring.numPixels(); i++) {
  //     // Berechnet den Farbton (Hue) basierend auf Position (i) und Zeit (step)
  //     int hue = (i * 3000 + step * 300) % 65536; 
  //     
  //     ring.setPixelColor(i, ring.ColorHSV(hue));
  //   }
  //   ring.show();
  // }


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 8: STATUS-LED BLINKT (Code nur in setup() und Übung 7 hier)
  // //////////////////////////////////////////////////////////////////
  // // Der Code hier ist derselbe wie Übung 7. Die Logik für die Status-LED
  // // ist in setup() (pinMode). Übung 7 zeigt, dass der Ring
  // // trotzdem läuft, auch wenn der Status-LED Test in setup() blockiert.
  // if (updateErforderlich()) { 
  //   for(int i = 0; i < ring.numPixels(); i++) {
  //     int hue = (i * 3000 + step * 300) % 65536; 
  //     ring.setPixelColor(i, ring.ColorHSV(hue));
  //   }
  //   ring.show();
  // }


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 9: TASTER-EINGABE (REAGIERT SOFORT)
  // //////////////////////////////////////////////////////////////////
  // // Achtung: Hier kein updateErforderlich(), damit die Reaktion sofort ist!
  // if (digitalRead(BUTTON_PIN) == LOW) {
  //   // Taster gedrückt (LOW)
  //   ring.fill(ring.Color(255, 0, 0)); // ROT
  // } else {
  //   // Taster nicht gedrückt (HIGH)
  //   ring.fill(ring.Color(0, 0, 255)); // BLAU
  // }
  // ring.show();


  // //////////////////////////////////////////////////////////////////
  // // ÜBUNG 10: STEUERUNG DER GESCHWINDIGKEIT
  // //////////////////////////////////////////////////////////////////
  // if (updateErforderlich()) {
  //   // effectSpeed ist global und kann jederzeit geändert werden
  //   if (step % 50 < 25) { // Die ersten 25 Schritte
  //     effectSpeed = 20; // Schnell
  //     ring.fill(ring.Color(255, 0, 0));
  //   } else { // Die nächsten 25 Schritte
  //     effectSpeed = 200; // Langsam
  //     ring.fill(ring.Color(0, 0, 255));
  //   }
  //   ring.show();
  // }


}