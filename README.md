
# WI-smartLight - ESP32 LED Controller

![WI-smartLight](https://i.imgur.com/your-image.png) 

Dieses Projekt ist ein multifunktionaler LED-Controller, der auf einem ESP32 basiert. Er bietet ein Webinterface zur Konfiguration und Steuerung, MQTT-Integration und eine physische Tastensteuerung.

## Inhaltsverzeichnis

- [Funktionsumfang](#funktionsumfang)
- [Erste Schritte](#erste-schritte)
- [Webinterface](#webinterface)
- [Tastensteuerung](#tastensteuerung)
- [MQTT-Steuerung](#mqtt-steuerung)
- [Kompilieren aus dem Quellcode](#kompilieren-aus-dem-quellcode)

## Funktionsumfang

- **Webinterface:** Ein benutzerfreundliches Webinterface zur Konfiguration von WLAN, MQTT und zur Steuerung der LED-Effekte.
- **LED-Effekte:** Eine Vielzahl von vordefinierten LED-Effekten, darunter:
  - Rainbow
  - Color Wipe
  - Theater Chase
  - Breathing
  - Sparkle
  - Running Light
  - Fire
  - Auto-Modus (wechselt automatisch zwischen den Effekten)
- **MQTT-Integration:** Steuerung des Controllers über MQTT. Eine spezielle Funktion namens "Finger-Modus" ermöglicht die Anzeige einer Anzahl von "Fingern" in einer bestimmten Farbe.
- **Tastensteuerung:** Eine physische Taste ermöglicht die Steuerung der Grundfunktionen ohne Netzwerkverbindung.

## Erste Schritte

1.  **Stromversorgung:** Schließen Sie den ESP32 an eine Stromquelle an.
2.  **Access Point (AP)-Modus:** Beim ersten Start oder wenn keine WLAN-Verbindung konfiguriert ist, startet der ESP32 im AP-Modus.
    -   **SSID:** `smartlight-Config-<dein Name>`
    -   **Passwort:** `12345678`
3.  **Verbinden:** Verbinden Sie sich mit Ihrem Computer oder Smartphone mit diesem WLAN-Netzwerk.
4.  **Webinterface öffnen:** Öffnen Sie einen Webbrowser und navigieren Sie zu `http://192.168.4.1`.
5.  **WLAN konfigurieren:**
    -   Öffnen Sie den Abschnitt "WLAN Konfiguration".
    -   Aktivieren Sie WLAN, wählen Sie Ihr Heimnetzwerk (über den Scan-Button) und geben Sie Ihr WLAN-Passwort ein.
    -   Speichern Sie die Konfiguration. Der ESP32 wird neu gestartet und verbindet sich mit Ihrem WLAN.
6.  **IP-Adresse finden:** Die neue IP-Adresse des Controllers wird auf dem seriellen Monitor angezeigt oder kann über Ihren Router ermittelt werden. Alternativ können Sie versuchen, über `http://smartlight.local` auf das Gerät zuzugreifen.

## Webinterface

Auf das integrierte Webninterface kann von jedem beliebigen Engerät (z.B. Laptop, Smartphone) mit einem gewöhnlichen Webbrowser zugegriffen werden (admin / admin). 
Das Webinterface ist in mehrere Abschnitte unterteilt:

-   **Status:** Zeigt den aktuellen System- und Netzwerkstatus an.
-   **LED Effekte:** Hier können Sie den aktuellen LED-Effekt auswählen, die Helligkeit anpassen und den Auto-Modus konfigurieren.
-   **WLAN Konfiguration:** Zum Einrichten der Verbindung zu Ihrem lokalen WLAN.
-   **MQTT Konfiguration:** Zur Konfiguration der Verbindung mit einem MQTT-Broker.
-   **System:** Bietet die Möglichkeit, die Konfiguration auf die Werkseinstellungen zurückzusetzen.

## Tastensteuerung

Die am `BUTTON_PIN` angeschlossene Taste hat mehrere Funktionen, die durch die Dauer des Tastendrucks bestimmt werden:

### `handleButton()` Funktion

Die `handleButton()`-Funktion wird kontinuierlich in der Hauptschleife des Programms aufgerufen, um den Zustand der Taste zu überwachen. Sie implementiert eine Entprellung (Debouncing), um fehlerhafte Mehrfach-Auslösungen zu verhindern.

-   **Kurzer Tastendruck (100ms - 2000ms):**
    -   Deaktiviert den "Finger-Modus", falls dieser aktiv war.
    -   Schaltet zum nächsten LED-Effekt in der Liste (0-8).
    -   Wenn Effekt 8 (Auto-Modus) erreicht wird, wird der Auto-Modus aktiviert.
    -   Die neue Konfiguration wird im Flash-Speicher gesichert.
    -   Der Name des neuen Effekts wird auf der seriellen Konsole ausgegeben.

-   **Mittlerer Tastendruck (2000ms - 5000ms):**
    -   Aktiviert den Access Point (AP)-Modus, falls das Gerät nicht bereits in diesem Modus ist. Dies ist nützlich, um die Netzwerkkonfiguration zu ändern, ohne die gesamte Konfiguration zurücksetzen zu müssen.

-   **Langer Tastendruck (> 5 Sekunden):**
    -   Löst einen Werksreset aus.
    -   Alle gespeicherten Konfigurationen (WLAN, MQTT, etc.) werden gelöscht.
    -   Der ESP32 wird anschließend neu gestartet.

## MQTT-Steuerung

Der Controller kann über MQTT Nachrichten empfangen, um den "Finger-Modus" zu aktivieren.

-   **Topic:** Das konfigurierte MQTT-Topic (Standard: `esp32/status`).
-   **Payload-Struktur (JSON):**

    ```json
    {
      "dev_id": "IHRE_CHIP_ID",
      "finger_count": 3,
      "color": "blue"
    }
    ```

-   **Parameter:**
    -   `dev_id`: Die eindeutige Chip-ID Ihres ESP32. Die Nachricht wird ignoriert, wenn diese ID nicht übereinstimmt.
    -   `finger_count`: Eine Zahl von 0 bis 5, die anzeigt, wie viele "Finger" (LED-Gruppen) leuchten sollen.
    -   `color`: Die Farbe, in der die LEDs leuchten sollen. Unterstützte Farben sind: `red`, `green`, `blue`, `yellow`, `purple`, `orange`, `pink`, `cyan`, `white`.

Der Finger-Modus bleibt für 5 Sekunden aktiv und wird dann automatisch beendet.

## Kompilieren aus dem Quellcode

### Voraussetzungen

-   [Visual Studio Code](https://code.visualstudio.com/)
-   [PlatformIO IDE Extension](https://platformio.org/platformio-ide)

### Schritte

1.  Klonen Sie dieses Repository.
2.  Öffnen Sie den Projektordner in Visual Studio Code.
3.  PlatformIO installiert automatisch alle benötigten Bibliotheken.
4.  Laden Sie das Dateisystem-Image mit dem Befehl `Upload Filesystem Image` in PlatformIO hoch.
5.  Kompilieren und laden Sie den Code mit dem `Upload`-Button in PlatformIO auf Ihren ESP32 hoch.
