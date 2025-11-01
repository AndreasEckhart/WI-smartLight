#pragma once

// Programmierkurs: Eigene LED-Effekte
// --------------------------------------------------
// In dieser Header-Datei stehen nur die Funktions-Signaturen.
// Implementiere deine Effekte in src/user_effects.cpp.
// Du brauchst in main.cpp nichts zu ändern.
//
// WICHTIG:
// - Verwende in deinen Funktionen den globalen NeoPixel-Streifen "strip".
// - Setze nur die Pixel-Farben (strip.setPixelColor(...)).
// - KEIN strip.show() hier aufrufen – das übernimmt die Hauptlogik.
// - Der Parameter "step" erhöht sich automatisch und eignet sich als Zeit-/Positionszähler.
//
// Beispiele findest du in src/user_effects.cpp.

void userEffect1(int step);
void userEffect2(int step);
void userEffect3(int step);
