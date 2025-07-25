
# WI-smartLight - ESP32 LED Controller

![WI-smartLight](https://i.imgur.com/your-image.png) 

Dieses Projekt ist ein multifunktionaler LED-Controller, der auf einem ESP32 basiert. Er bietet ein Webinterface zur Konfiguration und Steuerung, MQTT-Integration und eine physische Tastensteuerung.

## Inhaltsverzeichnis

- [Funktionsumfang](#funktionsumfang)
- [Erste Schritte](#erste-schritte)
- [Webinterface](#webinterface)
- [Status-LED-Indikator](#status-led-indikator)
- [Tastensteuerung](#tastensteuerung)
- [MQTT-Steuerung](#mqtt-steuerung)
- [Zukünftige Funktionen](#zukünftige-funktionen)
- [Kompilieren aus dem Quellcode](#kompilieren-aus-dem-quellcode)

## Funktionsumfang

- **Passwortgeschütztes Webinterface:** Ein benutzerfreundliches Webinterface zur Konfiguration und Steuerung, erreichbar über die IP-Adresse des Geräts.
- **LED-Effekte:** Eine Vielzahl von vordefinierten LED-Effekten, darunter:
  - **Alle aus**
  - Rainbow
  - Color Wipe
  - Theater Chase
  - Breathing
  - Sparkle
  - Running Light
  - Fire
  - **Auto-Modus:** Wechselt automatisch in einem einstellbaren Intervall (in Sekunden) durch die Effekte 1-7.
- **MQTT-Integration:** Steuerung des Controllers über MQTT. Eine spezielle Funktion namens "Finger-Modus" ermöglicht die Anzeige einer Anzahl von "Fingern" (0-5) in einer benutzerdefinierten Farbe.
- **Tastensteuerung:** Eine physische Taste ermöglicht die Steuerung der Grundfunktionen (Effektwechsel, AP-Modus starten, Werksreset) ohne Netzwerkverbindung.
- **Konfigurierbare Status-LED:** Eine Status-LED zeigt den aktuellen Netzwerkstatus an und kann bei Bedarf über das Webinterface deaktiviert werden.
- **mDNS-Unterstützung:** Einfacher Zugriff auf das Webinterface über `http://smartlight.local`, ohne die IP-Adresse kennen zu müssen.
- **Persistente Konfiguration:** Alle Einstellungen werden im Flash-Speicher gesichert und bleiben auch nach einem Neustart erhalten.

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

Auf das integrierte Webinterface kann von jedem beliebigen Endgerät (z.B. Laptop, Smartphone) mit einem gewöhnlichen Webbrowser zugegriffen werden. Die Standard-Anmeldedaten lauten:
-   **Benutzername:** `admin`
-   **Passwort:** `admin`

Das Webinterface ist in mehrere Abschnitte unterteilt:

-   **Status:** Zeigt den aktuellen System- und Netzwerkstatus an, inklusive IP-Adresse, Uptime und Software-Version.
-   **LED Effekte:** Hier können Sie den aktuellen LED-Effekt auswählen und die Helligkeit anpassen.
-   **Effekt Konfiguration:** In diesem Bereich können Sie den Auto-Modus (automatischer Effektwechsel) aktivieren und das Intervall für den Wechsel in Sekunden festlegen.
-   **WLAN Konfiguration:** Zum Einrichten der Verbindung zu Ihrem lokalen WLAN.
-   **MQTT Konfiguration:** Zur Konfiguration der Verbindung mit einem MQTT-Broker.
-   **System:** Bietet die Möglichkeit, die Konfiguration auf die Werkseinstellungen zurückzusetzen, die Status-LED zu deaktivieren und sich abzumelden (Logout).

## Status-LED-Indikator

Die am `STATUS_LED_PIN` angeschlossene LED zeigt den aktuellen Betriebszustand des Controllers an. Dieses Verhalten kann im Webinterface unter "System" deaktiviert werden.

-   **Langsames Blinken (1x pro Sekunde):** Normalbetrieb, mit dem WLAN verbunden.
-   **Schnelles Blinken (mehrfach pro Sekunde):** Das Gerät befindet sich im Access Point (AP)-Modus und wartet auf die Konfiguration.

## Tastensteuerung

Die am `BUTTON_PIN` angeschlossene Taste hat mehrere Funktionen, die durch die Dauer des Tastendrucks bestimmt werden:

-   **Kurzer Tastendruck (< 1 Sekunde):**
    -   Deaktiviert den "Finger-Modus", falls dieser aktiv war.
    -   Schaltet zum nächsten LED-Effekt in der Liste (0-8).
    -   Wenn Effekt 8 (Auto-Modus) erreicht wird, wird der Auto-Modus aktiviert.
    -   Die neue Konfiguration wird im Flash-Speicher gesichert.
    -   Der Name des neuen Effekts wird auf der seriellen Konsole ausgegeben.

-   **Mittlerer Tastendruck (2s - 5s):**
    -   Aktiviert den Access Point (AP)-Modus, falls das Gerät nicht bereits in diesem Modus ist. Dies ist nützlich, um die Netzwerkkonfiguration zu ändern, ohne die gesamte Konfiguration zurücksetzen zu müssen.

-   **Langer Tastendruck (> 8 Sekunden):**
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

## Zukünftige Funktionen

-   Over-The-Air (OTA) Updates

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
