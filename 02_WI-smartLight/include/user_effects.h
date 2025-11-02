#pragma once

// Programmierkurs: Eigene LED-Effekte
// --------------------------------------------------
// In dieser Header-Datei stehen nur die Funktions-Signaturen.
// Implementiere deine Effekte in src/user_effects.cpp.
// Du brauchst in main.cpp nichts zu ändern.
//
// WICHTIG:
// - Verwende in deinen Funktionen den globalen NeoPixel-Streifen "ring".
// - Setze nur die Pixel-Farben (ring.setPixelColor(...)).
// - KEIN ring.show() hier aufrufen – das übernimmt die Hauptlogik.
// - Der Parameter "step" erhöht sich automatisch und eignet sich als Zeit-/Positionszähler.
//
// Beispiele findest du in src/user_effects.cpp.

void deinEffekt1(int step);
void deinEffekt2(int step);
void deinEffekt3(int step);
