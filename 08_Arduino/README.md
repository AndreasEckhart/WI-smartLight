# WI-smartLight für Arduino IDE 2.3.6

## Vollversion mit WiFi, WebServer und MQTT

Diese Version enthält **alle Funktionen** aus dem PlatformIO-Projekt:
- WiFi Access Point und Station Mode
- Web-Konfigurationsinterface (HTML/CSS/JavaScript)
- MQTT Client mit TLS-Verschlüsselung
- 8 LED-Effekte (3 eigene + 3 vordefinierte + Auto + Aus)
- Button-Steuerung und Status-LED

## Installation

### 1. Arduino IDE 2.3.6 installieren
- Download: https://www.arduino.cc/en/software
- Version 2.3.6 oder neuer verwenden

### 2. ESP32 Board Support installieren
1. Arduino IDE öffnen
2. **Datei** → **Einstellungen**
3. Bei "Zusätzliche Boardverwalter-URLs" einfügen:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
4. **OK** klicken
5. **Tools** → **Board** → **Boardverwalter...**
6. Nach "ESP32" suchen
7. "ESP32 by Espressif Systems" installieren (Version 2.0.x oder 3.x)

### 3. Benötigte Bibliotheken installieren

**Sketch** → **Bibliothek einbinden** → **Bibliotheken verwalten...**

Installiere folgende Bibliotheken:

1. **Adafruit NeoPixel** (für WS2812B LEDs)
2. **ArduinoJson** (Version 7.x für JSON-Parsing)
3. **PubSubClient** (für MQTT)

Die restlichen Bibliotheken (WiFi, WebServer, DNSServer, ESPmDNS, LittleFS, Preferences) sind bereits im ESP32 Board Support enthalten.

## Board-Einstellungen

**Tools** Menü konfigurieren:
- **Board:** ESP32 Dev Module
- **Upload Speed:** 115200
- **CPU Frequency:** 240MHz (WiFi/BT)
- **Flash Frequency:** 80MHz
- **Flash Mode:** QIO
- **Flash Size:** 4MB (32Mb)
- **Partition Scheme:** Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
- **Core Debug Level:** None
- **Port:** COMx (den richtigen USB-Port auswählen)

## LittleFS Dateisystem hochladen

Die Webinterface-Dateien (HTML, CSS, JavaScript) müssen separat hochgeladen werden:

### Option 1: Arduino ESP32 LittleFS Plugin (empfohlen)

1. **Plugin installieren:**
   - Download: https://github.com/lorol/arduino-esp32littlefs-plugin/releases
   - Entpacke die ZIP-Datei
   - Verschiebe den Ordner `ESP32LittleFS` nach:
     - Windows: `C:\Users\<Username>\Documents\Arduino\tools\`
     - Mac: `~/Documents/Arduino/tools/`
     - Linux: `~/Arduino/tools/`
   - Erstelle den `tools` Ordner, falls er nicht existiert
   - Arduino IDE neu starten

2. **Dateien hochladen:**
   - Sketch öffnen: `08_Arduino/WI-smartLight/WI-smartLight.ino`
   - **Tools** → **ESP32 Sketch Data Upload**
   - Warten bis "LittleFS Image Uploaded" erscheint

### Option 2: esptool.py (manuell)

Falls das Plugin nicht funktioniert:

```powershell
# LittleFS Image erstellen und hochladen
pip install esptool
mklittlefs -c data -s 0x170000 littlefs.bin
esptool.py --chip esp32 --port COM3 write_flash 0x290000 littlefs.bin
```

⚠️ **Wichtig:** Der LittleFS Upload muss nur **einmal** durchgeführt werden, außer die Webinterface-Dateien ändern sich.

## Projekt öffnen

1. Arduino IDE öffnen
2. **Datei** → **Öffnen**
3. Navigiere zu: `08_Arduino/WI-smartLight/WI-smartLight.ino`
4. Datei öffnen

## Programmieren

### Deinen Namen eintragen
Ändere in Zeile 156:
```cpp
String getDeinName() {
  String deinName = "Andreas";  // ändere hier deinen Namen
  return deinName;
}
```

Dieser Name wird verwendet für:
- Hostname: `smartlight-<DeinName>` (für mDNS)
- Access Point SSID: `smartLight-Config-<DeinName>`
- Anzeige im Webinterface

### Deinen LED-Effekt programmieren
Ändere die Funktionen ab Zeile 161:

**Effekt 1:**
```cpp
void deinEffekt1(int step) {
  int magenta = ring.Color(255, 0, 255);
  ring.fill(magenta); // Alle Magenta/Pink (Rot + Blau)
}
```

**Effekt 2:**
```cpp
void deinEffekt2(int step) {
  // Pulsierende Helligkeit
  effectSpeed = 30; // Geschwindigkeit anpassen
  
  float brightness = (sin(step / 50.0 * PI) + 1.0) / 2.0;
  int brightnessValue = (int)(brightness * 255);
  
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(0, 0, brightnessValue));
  }
}
```

**Effekt 3:**
```cpp
void deinEffekt3(int step) {
  // Eigene Kreativität - siehe Beispielcode im Sketch
}
```

#### Verfügbare Variablen und Funktionen:

**NeoPixel-Objekt `ring`:**
```cpp
ring.numPixels()                    // Anzahl LEDs (24)
ring.Color(rot, grün, blau)         // Farbe erstellen (0-255)
ring.setPixelColor(pos, farbe)      // LED setzen (pos: 0-23)
ring.fill(farbe)                    // Alle LEDs gleiche Farbe
ring.clear()                        // Alle LEDs ausschalten
ring.getPixelColor(pos)             // LED-Farbe auslesen
ring.show()                         // LEDs aktualisieren (wird automatisch aufgerufen)
```

**Effekt-Geschwindigkeit:**
```cpp
effectSpeed = 50;  // Millisekunden zwischen Updates (Standard: 50ms)
```

**Animation-Counter:**
```cpp
step  // Erhöht sich automatisch, nutze für Animationen
```

## Upload

### 1. Code hochladen
1. ESP32 per USB verbinden
2. Richtigen **Port** unter **Tools** auswählen
3. **Sketch** → **Hochladen** (oder Strg+U)
4. Warten bis "Hard Resetting via RTS pin..." erscheint

### 2. LittleFS Dateien hochladen (nur einmal nötig)
1. **Tools** → **ESP32 Sketch Data Upload**
2. Warten bis "LittleFS Image Uploaded" erscheint

## Erster Start

### Access Point Modus
Beim ersten Start (oder wenn keine WiFi-Konfiguration vorhanden):
1. ESP32 startet als Access Point: `smartLight-Config-<DeinName>`
2. Passwort: `12345678`
3. Mit dem Smartphone/Laptop verbinden
4. Browser öffnet automatisch Konfigurationsseite (Captive Portal)
5. Falls nicht: Navigiere zu `http://192.168.4.1`

### Webinterface Konfiguration

**WiFi konfigurieren:**
1. Öffne Accordion "WLAN Konfiguration"
2. Setze Haken bei "WLAN aktivieren"
3. Klicke "Scannen" für verfügbare Netzwerke
4. Wähle dein WLAN und gib Passwort ein
5. Klicke "WLAN konfigurieren"
6. ESP32 startet neu und verbindet sich

**MQTT konfigurieren (optional):**
1. Öffne Accordion "MQTT Konfiguration"
2. Setze Haken bei "MQTT aktivieren"
3. Server: `ae6eb0d482a24683b08611859374ec88.s1.eu.hivemq.cloud`
4. Port: `8883` (TLS-verschlüsselt)
5. Benutzername: `testHTL`
6. Passwort: `iOT12XX23by`
7. Topic: `schnuppern`
8. Klicke "MQTT konfigurieren"

**LED Effekte steuern:**
1. Öffne Accordion "LED Effekte"
2. Wähle Effekt aus Dropdown:
   - **Auto-Modus**: Wechselt automatisch zwischen Effekten 1-6
   - **Dein Effekt 1-3**: Deine programmierten Effekte
   - **Regenbogen**: Vordefinierter Rainbow-Effekt
   - **Pixelrennen**: Theater Chase Effekt
   - **Feuer**: Feuer-Simulation
   - **Alle aus**: LEDs ausschalten
3. Helligkeit: 0-100 (Slider)
4. Wechselzeit (nur Auto-Modus): 5-600 Sekunden
5. Status-LED: Ein/Aus (blinkt je nach Modus)

### Nach WiFi-Verbindung

**mDNS Zugriff:**
- URL: `http://smartlight-<DeinName>.local`
- Funktioniert auf Windows, Mac, Linux (mit Avahi/Bonjour)

**IP-Adresse:**
- Steht im Serial Monitor oder im Status-Bereich des Webinterfaces

## Button-Funktionen

- **Kurz drücken (<2s):** Effekt wechseln (0→1→2→...→7→0)
- **Mittel drücken (>2s):** Access Point Modus starten
- **Lang drücken (>8s):** Konfiguration löschen und neu starten

## Status-LED

Die eingebaute LED (GPIO 2) zeigt den Betriebsstatus:
- **Schnelles Blinken (100ms):** WiFi nicht verbunden
- **Langsames Blinken (2000ms):** WiFi verbunden
- **Mittleres Blinken (250ms):** Access Point Modus

## Serial Monitor

1. **Tools** → **Serieller Monitor** (oder Strg+Shift+M)
2. Baudrate auf **115200** einstellen
3. Ausgaben beobachten:
   - Chip ID und Benutzername
   - WiFi Verbindungsstatus
   - MQTT Verbindungsstatus
   - IP-Adresse
   - Button-Events
   - Effekt-Wechsel

**Beispiel-Ausgabe:**
```
Starting smartLight Controller...
=== ESP32 Status ===
Chip ID: 08ECEB7446A8
User: Andreas
Version: 1.0.0
Mode: Station
WiFi: Enabled
IP: 192.168.1.42
MQTT: Enabled
Effect: 0
Status-LED: Enabled
==================
```

## Hardware

- **24x WS2812B NeoPixel** am Pin 3
- **Button** am Pin 9 
  - Kurz: Effekt wechseln
  - Mittel (>2s): AP-Modus
  - Lang (>8s): Reset
- **Status LED** am Pin 2 (blinkt je nach Modus)

## MQTT Integration

### Finger-Count Steuerung

Der Controller abonniert das MQTT Topic `schnuppern` und reagiert auf JSON-Messages:

```json
{
  "dev_id": "08ECEB7446A8",
  "finger_count": 3,
  "color": "rgb(255, 0, 0)"
}
```

**Verhalten:**
- `dev_id` muss mit Chip-ID übereinstimmen
- `finger_count`: 0-5 Finger werden als 2 LEDs pro Finger dargestellt
- `color`: RGB-Format `rgb(r, g, b)` oder Hex-Format `#RRGGBB`
- Finger-Modus läuft 5 Sekunden und kehrt dann zum vorherigen Effekt zurück
- Button-Druck deaktiviert Finger-Modus sofort

**Test über os-beyond.at:**
- URL: `https://os-beyond.at/schnuppern/index.html?client_id=<CHIP_ID>`
- Link wird im Webinterface beim MQTT-Status angezeigt (wenn verbunden)

## Unterschied zur PlatformIO-Version

Diese Arduino IDE Version ist **funktional identisch** zur PlatformIO-Version in `02_WI-smartLight`:

**Gleiche Features:**
- ✅ Alle WiFi-Funktionen (AP + Station Mode)
- ✅ Vollständiges Webinterface mit JSON-API
- ✅ MQTT Client mit TLS-Verschlüsselung
- ✅ 8 LED-Effekte + Auto-Modus
- ✅ Button-Steuerung und Status-LED
- ✅ Captive Portal für einfache Konfiguration
- ✅ mDNS für lokalen Zugriff
- ✅ Preferences für persistente Speicherung

**Unterschiede:**
- 📁 **Dateistruktur:** `.ino` statt `.cpp` + separate Dateien
- 📦 **Bibliotheken:** Manuelle Installation statt `platformio.ini`
- 🗂️ **Dateisystem:** LittleFS Upload über Plugin statt automatisch
- 🔧 **Build-System:** Arduino IDE Compiler statt PlatformIO

**Wann welche Version nutzen?**
- **Arduino IDE:** Für Schüler, einfacher Einstieg, GUI-basiert
- **PlatformIO:** Für Entwickler, professionelle Features, besseres Dependency-Management

## Problembehebung

**"Port nicht gefunden":**
- USB-Kabel überprüfen (muss Daten unterstützen)
- ESP32 Treiber installieren:
  - CP210x: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
  - CH340: http://www.wch.cn/downloads/CH341SER_ZIP.html
- Anderen USB-Port probieren
- ESP32 neu einstecken

**"Upload fehlgeschlagen":**
- BOOT-Button am ESP32 beim Upload gedrückt halten
- Board-Einstellungen überprüfen (siehe oben)
- Serial Monitor schließen vor Upload
- Baudrate auf 115200 setzen (nicht höher)
- USB-Hub vermeiden, direkt an PC anschließen

**"LittleFS Mount Failed":**
- LittleFS Plugin korrekt installiert?
- **Tools** → **ESP32 Sketch Data Upload** ausgeführt?
- Partition Scheme: "Default 4MB with spiffs" gewählt?
- Erst Code hochladen, dann LittleFS Data Upload

**"LEDs leuchten nicht":**
- Stromversorgung überprüfen (USB reicht für Tests)
- NeoPixel-Datenleitung an GPIO 3 angeschlossen?
- Erste LED: Data-IN Pin (nicht Data-OUT)
- `ring.show()` wird automatisch aufgerufen

**"WiFi verbindet nicht":**
- SSID und Passwort korrekt?
- 2.4 GHz WLAN (ESP32 unterstützt kein 5 GHz)
- WiFi-Router in Reichweite?
- Serial Monitor prüfen für Fehlermeldungen
- WiFi-Konfiguration über AP-Modus neu eingeben

**"MQTT verbindet nicht":**
- WiFi muss zuerst verbunden sein
- Server-Adresse korrekt?
- Port 8883 (TLS) oder 1883 (unverschlüsselt)
- Benutzername/Passwort korrekt?
- Firewall-Regeln prüfen
- Serial Monitor zeigt `rc=-2` = Timeout, `rc=-4` = Connection refused

**"Webinterface lädt nicht":**
- LittleFS Dateien hochgeladen?
- mDNS funktioniert nicht → IP-Adresse nutzen (siehe Serial Monitor)
- Browser-Cache leeren (Strg+F5)
- Captive Portal: Im AP-Modus automatisch

**"Compilation error":**
- Alle Bibliotheken installiert? (NeoPixel, ArduinoJson, PubSubClient)
- ESP32 Board Support installiert?
- ArduinoJson Version 7.x (nicht 6.x)
- Syntaxfehler im eigenen Code (Zeile wird angezeigt)

**"Brownout detector was triggered":**
- Stromversorgung zu schwach
- USB-Port wechseln oder externes Netzteil verwenden
- LEDs auf niedrigere Helligkeit setzen

**"Guru Meditation Error":**
- Watchdog-Reset durch blockierenden Code
- Serial Monitor für Stack-Trace prüfen
- Zu viel Code in `loop()` oder LED-Effekt
- WiFi/MQTT Timeouts zu lang

## Weiterführende Links

- Adafruit NeoPixel Guide: https://learn.adafruit.com/adafruit-neopixel-uberguide
- Arduino Referenz: https://www.arduino.cc/reference/en/
- ESP32 Dokumentation: https://docs.espressif.com/projects/arduino-esp32/

## Support

Bei Fragen wende dich an: Andreas Eckhart (HTL Anichstrasse)
