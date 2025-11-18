# WI-smartLight – IoT Benutzeranleitung

Diese Anleitung erklärt, wie du dein WI-smartLight mit dem WLAN verbindest, über das Webinterface steuerst und per MQTT-Gestenerkennung mit KI bedienst.

---

## Schnellstart

1. **Gerät einschalten** → Access Point `smartlight-Config-<DeinName>` erscheint
2. **Mit AP verbinden** (Passwort: `12345678`)
3. **Browser öffnen** → beliebige URL eingeben (z. B. `http://192.168.4.1`) → Webinterface erscheint automatisch (Captive Portal)
4. **WLAN konfigurieren** → Gerät startet neu und verbindet sich mit deinem Netzwerk
5. **Zugriff im lokalen Netz** über `http://smartlight-<DeinName>.local` oder die angezeigte IP-Adresse

---

## 1. Zugriff auf das WI-smartLight

### 1.1 Ersteinrichtung (Access Point Modus)

Wenn das Gerät noch nicht konfiguriert ist oder keine WLAN-Verbindung herstellen kann, startet es automatisch einen eigenen Access Point (AP):

- **SSID:** `smartlight-Config-<DeinName>` (z. B. `smartlight-Config-Max`)
- **Passwort:** `12345678`
- **IP-Adresse:** `192.168.4.1`

**So gehst du vor:**

1. Verbinde dein Smartphone/Laptop mit dem AP `smartlight-Config-<DeinName>`
2. Öffne einen Browser und gib eine beliebige URL ein (z. B. `http://example.com` oder `http://192.168.4.1`)
3. Das Captive Portal leitet dich automatisch zum Webinterface weiter
4. Falls keine automatische Weiterleitung erfolgt, navigiere manuell zu `http://192.168.4.1`

### 1.2 Zugriff im lokalen Netzwerk (Station Modus)

Sobald das Gerät mit deinem WLAN verbunden ist, kannst du es über zwei Wege erreichen:

**Option A: mDNS (empfohlen)**
```
http://smartlight-<DeinName>.local
```
Beispiel: `http://smartlight-max.local`

> **Hinweis:** mDNS funktioniert unter Windows, macOS, Linux und den meisten mobilen Geräten. Falls nicht, nutze Option B.

**Option B: IP-Adresse**

Die IP-Adresse wird im seriellen Monitor angezeigt oder kann über den Router ermittelt werden:
```
http://192.168.1.123
```
(Beispiel-IP, deine wird abweichen)

---

## 2. Webinterface Bedienung

Das Webinterface ist übersichtlich in Akkordeon-Sektionen unterteilt:

### 2.1 Status

Zeigt Systeminformationen an:
- **Chip ID:** Eindeutige Geräte-ID
- **Uptime:** Laufzeit seit letztem Neustart
- **WiFi Status:** Verbindung/Signalstärke
- **IP-Adresse:** Aktuelle Netzwerkadresse
- **MQTT Status:** Verbindung zum MQTT-Broker
- **Aktueller Effekt:** Welcher LED-Effekt läuft
- **Helligkeit:** Aktuelle Helligkeitsstufe
- **Wechselzeit:** Auto-Modus Intervall (nur sichtbar wenn Auto-Modus aktiv)

### 2.2 LED Effekte

Steuerung der Lichteffekte:

**Effekt auswählen:**
- **Auto-Modus (0):** Wechselt automatisch zwischen den Effekten 1–6 durch (konfigurierbare Wechselzeit)
- **Dein Effekt 1/2/3:** Deine selbst programmierten Effekte
- **Regenbogen (4):** Klassischer RGB-Regenbogen
- **Pixelrennen (5):** Theater Chase Effekt
- **Feuer (6):** Flammen-Simulation
- **Alle aus (7):** LEDs ausschalten

**Wechselzeit (nur bei Auto-Modus):**
- Slider: 5–600 Sekunden (5s-Schritte)
- Legt fest, wie lange jeder Effekt im Auto-Modus angezeigt wird

**Helligkeit:**
- Slider: 0–100 (entspricht 0–255 intern)
- Ändert die Helligkeit aller LEDs

**Status-LED:**
- Checkbox aktiviert/deaktiviert die blaue Status-LED auf dem Board
- Zeigt Aktivität und Betriebsmodus an

### 2.3 WLAN Konfiguration

**WLAN aktivieren:**
- Checkbox: Aktiviert Station-Modus (Verbindung zu deinem Router)

**SSID:**
- Name deines WLANs
- Button „Scannen": Zeigt verfügbare Netzwerke an (klickbar zum Auswählen)

**Passwort:**
- WLAN-Schlüssel (WPA/WPA2)
- Augen-Symbol: Passwort anzeigen/verbergen

**Speichern:**
- Nach dem Klicken startet das Gerät neu und verbindet sich mit dem konfigurierten WLAN
- Wichtig: Notiere dir die IP-Adresse oder verwende mDNS

---

## 3. MQTT Konfiguration

MQTT ermöglicht die Anbindung an Smart-Home-Systeme, Cloud-Plattformen und KI-Anwendungen.

### 3.1 Parameter

**MQTT aktivieren:**
- Checkbox: Schaltet MQTT-Client ein/aus

**Server:**
- Hostname oder IP-Adresse des MQTT-Brokers
- Beispiel: `broker.hivemq.com`, `mqtt.example.com`

**Port:**
- Standard: `1883` (unverschlüsselt)
- TLS: `8883` (verschlüsselt, wird vom Gerät unterstützt)

**Benutzername (optional):**
- Falls dein Broker Authentifizierung verlangt

**Passwort (optional):**
- MQTT-Passwort (Augen-Symbol zum Anzeigen/Verbergen)

**Topic:**
- Standard: `esp32/status`
- Das Gerät abonniert dieses Topic und empfängt Befehle
- Das Gerät publiziert Statusmeldungen auf diesem Topic

### 3.2 MQTT Nachrichtenformat

Das WI-smartLight erwartet JSON-Nachrichten im folgenden Format:

```json
{
  "dev_id": "a1b2c3d4",
  "finger_count": 3,
  "color": "#00ff00"
}
```

**Felder:**
- `dev_id` (String): Chip-ID des Zielgeräts (siehe Webinterface → Status → Chip ID)
- `finger_count` (Integer): Anzahl der erkannten Finger (0–5)
- `color` (String): Farbe im Hex-Format (z. B. `#ff0000` = Rot, `#00ff00` = Grün, `#0000ff` = Blau)

**Beispiel:**
```json
{
  "dev_id": "a1b2c3d4",
  "finger_count": 2,
  "color": "#ff00ff"
}
```
→ Zeigt 2 Finger in Magenta an (jeweils 2 LEDs pro Finger, also 4 LEDs insgesamt)

### 3.3 Testen mit MQTT-Client

**Mosquitto (Terminal):**
```bash
# Nachricht publishen
mosquitto_pub -h broker.hivemq.com -p 1883 -t "esp32/status" -m '{"dev_id":"a1b2c3d4","finger_count":3,"color":"#0000ff"}'
```

**MQTTX (GUI-Client):**
1. Broker-Verbindung einrichten (Host, Port, ggf. Credentials)
2. Topic `esp32/status` auswählen
3. JSON-Payload eintragen und senden

---

## 4. Gestenerkennung (Finger-Modus)

Die Gestenerkennung erfolgt extern über eine KI-Anwendung (z. B. Python-Skript mit OpenCV + MediaPipe), die erkannte Finger über MQTT an das Gerät sendet.

### 4.1 Funktionsweise

1. **Externe KI-Anwendung:**
   - Kamera erfasst Handgesten
   - KI erkennt Anzahl der gezeigten Finger (0–5)
   - Anwendung sendet MQTT-Nachricht mit `dev_id`, `finger_count` und `color`

2. **WI-smartLight empfängt MQTT-Nachricht:**
   - Prüft, ob `dev_id` mit eigener Chip-ID übereinstimmt
   - Aktiviert Finger-Modus für 5 Sekunden
   - Zeigt pro Finger 2 LEDs in der angegebenen Farbe an

3. **LED-Anordnung:**
   - Finger 1: LED 0–1
   - Finger 2: LED 5–6
   - Finger 3: LED 10–11
   - Finger 4: LED 15–16
   - Finger 5: LED 20–21

### 4.2 Beispiel: 3 Finger, blaue Farbe

MQTT-Payload:
```json
{
  "dev_id": "a1b2c3d4",
  "finger_count": 3,
  "color": "#0000ff"
}
```

**Ergebnis:**
- LEDs 0–1 leuchten blau (Finger 1)
- LEDs 5–6 leuchten blau (Finger 2)
- LEDs 10–11 leuchten blau (Finger 3)
- Nach 5 Sekunden kehrt das Gerät zum vorherigen Effekt zurück

### 4.3 Deaktivierung

Der Finger-Modus endet automatisch nach 5 Sekunden oder kann durch kurzes Drücken der Hardware-Taste am Gerät sofort beendet werden.

---

## 5. WLAN-Einbindung Schritt-für-Schritt

### 5.1 Erstverbindung

1. **Gerät einschalten** (USB-Kabel oder externes Netzteil)
2. **Access Point suchen:**
   - WLAN-Liste auf Smartphone/Laptop öffnen
   - Nach `smartlight-Config-<DeinName>` suchen
3. **Verbinden:**
   - Passwort: `12345678`
4. **Webinterface öffnen:**
   - Browser: beliebige URL eingeben → automatische Weiterleitung
   - Oder manuell: `http://192.168.4.1`
5. **WLAN konfigurieren:**
   - Akkordeon „WLAN Konfiguration" öffnen
   - „WLAN aktivieren" ankreuzen
   - SSID eingeben (oder „Scannen" nutzen)
   - Passwort eingeben
   - „WLAN konfigurieren" klicken
6. **Neustart abwarten:**
   - Gerät startet neu (ca. 10 Sekunden)
   - Status-LED zeigt Verbindungsversuch an
7. **Verbindung herstellen:**
   - Dein Smartphone/Laptop wieder mit deinem normalen WLAN verbinden
   - Zugriff über `http://smartlight-<DeinName>.local` oder IP-Adresse

### 5.2 WLAN-Problembehebung

**Gerät startet immer im AP-Modus:**
- WLAN-Passwort falsch → Konfiguration wiederholen
- WLAN-Signal zu schwach → Gerät näher am Router platzieren
- 5 GHz WLAN → ESP32 unterstützt nur 2.4 GHz, entsprechendes Netzwerk wählen

**mDNS funktioniert nicht:**
- Windows: Bonjour-Dienst installieren (kommt mit iTunes oder separat von Apple)
- Alternative: IP-Adresse über Router-Webinterface (DHCP-Liste) ermitteln

**Konfiguration löschen:**
- Webinterface → Akkordeon „System" → „Konfiguration löschen"
- Gerät startet neu im AP-Modus

---

## 6. Hardware-Taster

Das Gerät verfügt über einen physischen Taster (GPIO 9):

**Funktionen:**
- **Kurzer Druck (< 3 Sekunden):**
  - Nächster Effekt (1 → 2 → 3 → 4 → 5 → 6 → 7 → 1 → ...)
  - Deaktiviert Finger-Modus sofort
- **Langer Druck (> 3 Sekunden):**
  - Wechselt zwischen Auto-Modus (0) und aktuellem Effekt

---

## 7. Status-LED

Die blaue LED auf dem Board zeigt den Betriebszustand an:

- **Schnelles Blinken (0,1s):** Access Point Modus (Konfiguration erforderlich)
- **Langsames Blinken (1s):** WLAN verbunden, MQTT nicht verbunden
- **Sehr langsames Blinken (2s):** WLAN + MQTT verbunden
- **Permanent ein:** Boot-Sequenz oder Fehler

Die Status-LED kann im Webinterface deaktiviert werden (Akkordeon „LED Effekte").

---

## 8. MQTT-Anwendungsbeispiele

### 8.1 Home Assistant Integration

```yaml
# configuration.yaml
mqtt:
  sensor:
    - name: "WI-smartLight Status"
      state_topic: "esp32/status"
      value_template: "{{ value_json.effect }}"

  button:
    - name: "Finger 3 Blau"
      command_topic: "esp32/status"
      payload_press: '{"dev_id":"a1b2c3d4","finger_count":3,"color":"#0000ff"}'
```

### 8.2 Python-Skript (Gestenerkennung)

```python
import cv2
import mediapipe as mp
import paho.mqtt.client as mqtt
import json

# MQTT Setup
broker = "broker.hivemq.com"
topic = "esp32/status"
dev_id = "a1b2c3d4"  # Deine Chip-ID

client = mqtt.Client()
client.connect(broker, 1883)

# MediaPipe Hand Detection
mp_hands = mp.solutions.hands
hands = mp_hands.Hands()

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    results = hands.process(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            # Finger zählen (vereinfachte Logik)
            finger_count = count_fingers(hand_landmarks)  # Implementierung nicht gezeigt
            
            # MQTT-Nachricht senden
            payload = {
                "dev_id": dev_id,
                "finger_count": finger_count,
                "color": "#00ff00"
            }
            client.publish(topic, json.dumps(payload))
    
    cv2.imshow("Hand Tracking", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
```

### 8.3 Node-RED Flow

```json
[
  {
    "id": "mqtt_in",
    "type": "mqtt in",
    "topic": "esp32/status",
    "broker": "broker_config"
  },
  {
    "id": "function",
    "type": "function",
    "func": "msg.payload = {\"dev_id\":\"a1b2c3d4\",\"finger_count\":msg.payload,\"color\":\"#ff0000\"}; return msg;"
  },
  {
    "id": "mqtt_out",
    "type": "mqtt out",
    "topic": "esp32/status",
    "broker": "broker_config"
  }
]
```

---

## 9. Technische Details

### 9.1 Netzwerk

- **WiFi:** 2.4 GHz (802.11 b/g/n)
- **Protokolle:** HTTP, MQTT, mDNS, DNS (Captive Portal)
- **Authentifizierung:** Optional (standardmäßig deaktiviert)
- **TLS:** Unterstützt für MQTT (Port 8883)

### 9.2 Hardware

- **Mikrocontroller:** ESP32 (Dual-Core, 240 MHz)
- **LEDs:** 24x WS2812B (Adafruit NeoPixel)
- **Taster:** GPIO 9 (Pull-up, aktiv LOW)
- **Status-LED:** GPIO 2 (blau)
- **Neopixel-Pin:** GPIO 3

### 9.3 Software

- **Framework:** Arduino (PlatformIO)
- **Bibliotheken:**
  - WiFi, WebServer, ESPmDNS
  - PubSubClient (MQTT)
  - Adafruit_NeoPixel
  - ArduinoJson
  - LittleFS (Dateisystem für HTML/CSS/JS)
- **Speicher:**
  - Preferences (NVS) für Konfiguration
  - LittleFS für Webdateien

---

## 10. Troubleshooting

### Problem: Gerät verbindet sich nicht mit WLAN
**Lösung:**
- 5 GHz WLAN? → ESP32 unterstützt nur 2.4 GHz
- Passwort korrekt? → Groß-/Kleinschreibung beachten
- WLAN-Reichweite? → Gerät näher am Router platzieren
- Konfiguration löschen und neu einrichten

### Problem: mDNS funktioniert nicht
**Lösung:**
- Windows: Bonjour-Dienst installieren
- Alternative: IP-Adresse direkt verwenden
- Router-Firewall prüfen (mDNS nutzt Port 5353 UDP)

### Problem: MQTT-Verbindung schlägt fehl
**Lösung:**
- Broker erreichbar? → Mit MQTT-Client testen (z. B. MQTTX)
- Port korrekt? → 1883 (unverschlüsselt) oder 8883 (TLS)
- Credentials korrekt? → Benutzername/Passwort prüfen
- Firewall? → Port freigeben

### Problem: Gestenerkennung funktioniert nicht
**Lösung:**
- `dev_id` korrekt? → Chip-ID im Webinterface prüfen
- MQTT-Topic korrekt? → Standard: `esp32/status`
- JSON-Format korrekt? → Mit Online-Validator prüfen
- Finger-Modus läuft nur 5 Sekunden → Nachricht erneut senden

### Problem: Webinterface lädt nicht
**Lösung:**
- LittleFS nicht initialisiert → Dateien über PlatformIO hochladen:
  ```bash
  pio run -t uploadfs
  ```
- Browser-Cache leeren
- Andere URL versuchen (IP statt mDNS)

---

## 11. Weiterführende Ressourcen

- **Projekt-Repository:** [github.com/AndreasEckhart/WI-smartLight](https://github.com/AndreasEckhart/WI-smartLight)
- **PlatformIO:** [platformio.org](https://platformio.org)
- **MQTT:** [mqtt.org](https://mqtt.org)
- **MediaPipe (Gestenerkennung):** [google.github.io/mediapipe](https://google.github.io/mediapipe)
- **HTL Anichstraße:** [htlinn.ac.at](https://htlinn.ac.at)

---

## 12. Support

Bei Fragen oder Problemen:
- **Lehrer/Workshop-Leiter kontaktieren**
- **GitHub Issues:** [github.com/AndreasEckhart/WI-smartLight/issues](https://github.com/AndreasEckhart/WI-smartLight/issues)
- **E-Mail:** Siehe Projekt-Repository

---

**Viel Spaß mit deinem WI-smartLight! 🎨💡**
