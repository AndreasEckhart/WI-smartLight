/***********************************************************************************
 * ⚠️ ACHTUNG: DIESE DATEI NICHT BEARBEITEN! ⚠️
 * 
 * Diese Datei enthält nur die Funktions-Signaturen.
 * Implementiere deine Effekte ausschließlich in src/user_effects.cpp
 * 
 * HTL-Anichstrasse, Innsbruck / (c)2025 Andreas Eckhart
 ***********************************************************************************/
#pragma once

#include <Arduino.h>

//
// WICHTIG:
// - Verwende in deinen Funktionen den globalen NeoPixel-Streifen "ring".
// - Setze nur die Pixel-Farben (ring.setPixelColor(...)).
// - KEIN ring.show() hier aufrufen – das übernimmt die Hauptlogik.
// - Der Parameter "step" erhöht sich automatisch und eignet sich als Zeit-/Positionszähler.
//
// GESCHWINDIGKEIT ANPASSEN:
// - Du kannst die Variable "effectSpeed" in deinen Funktionen ändern
// - Kleinere Werte = schneller, größere Werte = langsamer (in Millisekunden)
// - Beispiel: effectSpeed = 100; macht die Animation langsamer
// - Beispiel: effectSpeed = 20; macht die Animation schneller
//
// Beispiele findest du in src/user_effects.cpp.

// Funktion die deinen Namen zurückgibt
// ÄNDERE den Rückgabewert in src/user_effects.cpp!
String getDeinName();

void deinEffekt1(int step);
void deinEffekt2(int step);
void deinEffekt3(int step);
