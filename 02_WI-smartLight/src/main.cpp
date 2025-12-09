/***********************************************************************************
* ⚠️ ACHTUNG: DIESE DATEI NICHT BEARBEITEN! ⚠️
************************************************************************************
*                           _   _     _       _     _   
*  ___ _ __ ___   __ _ _ __| |_| |   (_) __ _| |__ | |_ 
* / __| '_ ` _ \ / _` | '__| __| |   | |/ _` | '_ \| __|
* \__ \ | | | | | (_| | |  | |_| |___| | (_| | | | | |_ 
* |___/_| |_| |_|\__,_|_|   \__|_____|_|\__, |_| |_|\__|
*                                       |___/           
*
*  ESP32 LED Controller mit Webinterface und MQTT
*  HTL-Anichstrasse, Innsbruck
*  Wirtschaftsingenieure - Betriebsinformatik / (c)2025 Andreas Eckhart
***********************************************************************************/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

// Hinweis für den Programmierkurs:
// Bitte bearbeite deine eigenen LED-Effekte ausschließlich in:
//   - include/user_effects.h (Signaturen)
//   - src/user_effects.cpp   (Implementierung)
// Der gesamte Core (WLAN, Webserver, MQTT, Captive Portal etc.) ist hier fertig vorbereitet.
// In diesem File musst du nichts ändern, außer du willst die Tasten-Logik oder Namen ergänzen.

#include "user_effects.h"

#define PRODUCT_VERSION "1.0.0"

// Hardware Definitionen
#define NEOPIXEL_PIN 3
#define NEOPIXEL_COUNT 24
#define STATUS_LED_PIN 2
#define BUTTON_PIN 9

// Authentifizierungsdaten (hardcoded)
bool useAuthentication = false;   // HTTP-Authentifizierung deaktiviert
const char* http_username = "admin";
const char* http_password = "admin";

// Captive Portal DNS
const byte DNS_PORT = 53;
DNSServer dnsServer;

// WiFi und Server
WebServer server(80);
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);
Preferences preferences;

// NeoPixel
Adafruit_NeoPixel ring(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Konfigurationsvariablen
bool wifi_enabled = false;
String wifi_ssid = "";
String wifi_password = "";
String hostname = "smartlight-" + getDeinName();
bool ap_mode = false;
String ap_ssid = "smartLight-Config-" + getDeinName();
String ap_password = "12345678";
String currentVersion = PRODUCT_VERSION;

// MQTT Server von GreiA
String mqtt_server = "ae6eb0d482a24683b08611859374ec88.s1.eu.hivemq.cloud";
int mqtt_port = 8883;
String mqtt_user = "testHTL";
String mqtt_password = "iOT12XX23by";
String mqtt_topic = "schnuppern";
bool mqtt_enabled = false;

// LED Effekte
const int BRIGHTNESS = 20; // Standard Neopixel Helligkeit (0-255)
int currentBrightness = BRIGHTNESS;

int currentEffect = 0; // 0 = Auto-Modus, 1..3 = Benutzer, 4..6 vordefiniert, 7 = Alle aus
unsigned long lastEffectUpdate = 0; // Zeitstempel der letzten Effektaktualisierung
const int DEFAULT_EFFECT_SPEED = 50; // Standard-Geschwindigkeit für Effekte in ms
int effectSpeed = DEFAULT_EFFECT_SPEED;

// Auto-Modus
bool autoMode = false;
unsigned long autoModeTimer = 0;
int autoModeIndex = 0;
int currentAutoTimer = 30;
unsigned long fingerModeTimer = 0;

// Button
bool buttonPressed = false;
unsigned long buttonPressStart = 0;
unsigned long lastButtonChange = 0;
bool lastButtonState = true;

// Status LED
bool statusLedState = false;
unsigned long statusLedTimer = 0;
bool statusLedEnabled = true;

// MQTT finger count application
int fingerCount = 0;
String fingerColor = "blue";
bool fingerMode = false;
unsigned long lastWiFiCheck = 0;
bool onlineStatus = false;

// Funktionen Prototypen
void loadConfig();
void saveConfig();
void clearConfig();
void startWiFi();
void checkWiFi();
void startAPMode();
void setupWebServer();
void setupMQTT();
void reconnectMQTT();
void publishStatus();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void handleButton();
void updateStatusLED();
void updateNeoPixels();
void runEffect(int effect);
void rainbowEffect(int step);
void colorWipeEffect(int step, uint32_t color);
void theaterChaseEffect(int step, uint32_t color);
void breathingEffect(int step, uint32_t color);
void sparkleEffect(uint32_t color);
void runningLightEffect(int step, uint32_t color);
void fireEffect(int step);
void printStatus();
void handleRoot();
void handleCSS();
void handleJS();
void handleStatus();
void handleWiFiScan();
void handleWiFiConfig();
void handleMQTTConfig();
void handleEffectConfig();
void handleBrightness();
void handleAutoTimer();
void handleReset();
void handleLogout();
void handleNotFound();
void handleStatusLed();
void handleFavicon();
void handleCaptiveRedirect();
String getEffectName(int effect);
String getChipId();
bool webAuthenticate();
void fingerLedEffect();
uint32_t hexToNeoPixelColor(const char* hexColor);

void setup() {
  Serial.begin(115200);
  delay(1000); // Kurze Pause für die serielle Verbindung

  // Hardware initialisieren
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  for (int i = 0; i <= 6; i++) {
    digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
    delay(200);
  }
  digitalWrite(STATUS_LED_PIN, LOW);
  
  Serial.println("Starting smartLight Controller..."); 

  // NeoPixel initialisieren
  ring.begin();
  ring.show();
  ring.setBrightness(10);
  
  // LittleFS initialisieren
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  
  // Konfiguration laden
  loadConfig();
  
  // WiFi starten
  startWiFi();
  
  // mDNS starten
  if (MDNS.begin(hostname)) {
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
  }
  
  // Webserver starten
  setupWebServer();
  server.begin();
  
  // MQTT initialisieren
  if (mqtt_enabled && !ap_mode) {
    setupMQTT();
  }
  
  Serial.println("Setup completed");
  printStatus();
}

void loop() {
  server.handleClient();
  // Captive Portal DNS bearbeiten, wenn AP-Modus aktiv ist
  if (ap_mode) {
    dnsServer.processNextRequest();
  }
  
  handleButton();
  updateStatusLED();
  updateNeoPixels();
  checkWiFi();
  
  delay(1); // Kurze Pause, um die CPU nicht zu überlasten
}

void loadConfig() {
  preferences.begin("config", false);
  
  wifi_enabled = preferences.getBool("wifi_enabled", false);
  wifi_ssid = preferences.getString("wifi_ssid", "");
  wifi_password = preferences.getString("wifi_pass", "");
  
  mqtt_server = preferences.getString("mqtt_server", "");
  mqtt_port = preferences.getInt("mqtt_port", 8883);
  mqtt_user = preferences.getString("mqtt_user", "");
  mqtt_password = preferences.getString("mqtt_pass", "");
  mqtt_topic = preferences.getString("mqtt_topic", "esp32/status");
  mqtt_enabled = preferences.getBool("mqtt_enabled", false);
  
  int savedEffect = preferences.getInt("effect", 0);
  currentBrightness = preferences.getInt("brightness", BRIGHTNESS);
  currentAutoTimer = preferences.getInt("auto_timer", 30);
  statusLedEnabled = preferences.getBool("led_enabled", true);
  
  // Migration und Begrenzung auf neue Effektbelegung (0..7)
  if (savedEffect == 8) {
    currentEffect = 0; // Auto-Modus
  } else if (savedEffect == 9) {
    currentEffect = 1; // User Effekt 1
  } else if (savedEffect == 10) {
    currentEffect = 2; // User Effekt 2
  } else if (savedEffect < 0 || savedEffect > 7) {
    currentEffect = 0; // Fallback: Auto-Modus
  } else {
    currentEffect = savedEffect;
  }

  ring.setBrightness(currentBrightness);

  // Auto-Modus aktivieren, wenn Effekt 0
  autoMode = (currentEffect == 0);
  
  preferences.end();
}

void saveConfig() {
  preferences.begin("config", false);
  
  preferences.putBool("wifi_enabled", wifi_enabled);
  preferences.putString("wifi_ssid", wifi_ssid);
  if (wifi_password.length() > 0) {
    preferences.putString("wifi_pass", wifi_password);
  }
  
  preferences.putString("mqtt_server", mqtt_server);
  preferences.putInt("mqtt_port", mqtt_port);
  preferences.putString("mqtt_user", mqtt_user);
  if (mqtt_password.length() > 0) {
    preferences.putString("mqtt_pass", mqtt_password);
  }

  preferences.putString("mqtt_topic", mqtt_topic);
  preferences.putBool("mqtt_enabled", mqtt_enabled);
  
  preferences.putInt("effect", currentEffect);
  preferences.putInt("brightness", currentBrightness);
  preferences.putInt("auto_timer", currentAutoTimer);
  preferences.putBool("led_enabled", statusLedEnabled);
  
  preferences.end();
}

void clearConfig() {
  preferences.begin("config", false);
  preferences.clear();
  preferences.end();
  
  // Reset zu Standardwerten
  wifi_enabled = false;
  wifi_ssid = "";
  wifi_password = "";
  mqtt_server = "";
  mqtt_port = 8883;
  mqtt_user = "";
  mqtt_password = "";
  mqtt_topic = "esp32/status";
  mqtt_enabled = false;

  currentEffect = 0;
  currentBrightness = BRIGHTNESS;
  currentAutoTimer = 30;
  statusLedEnabled = true;
  
  Serial.println("Configuration cleared");
}

void startWiFi() {
  if(!wifi_enabled) {
    Serial.println("WiFi is disabled. Starting AP mode");
    startAPMode();
    return;
  } else {
    Serial.println("WiFi is enabled, attempting to connect");
    if (wifi_ssid.length() > 0) {
      WiFi.mode(WIFI_STA);
      WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
      
      Serial.print("Connecting to WiFi");
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts <= 5) {
        delay(1000);
        Serial.print(".");
        attempts++;
      }
      
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.print("Connected! IP: ");
        Serial.println(WiFi.localIP());
        ap_mode = false;
        onlineStatus = true; // Setze Online-Status auf true, wenn verbunden
        // Sicherstellen, dass Captive DNS gestoppt ist und AP aus
        dnsServer.stop();
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
      } else {
        onlineStatus = false; // Setze Online-Status auf false, wenn nicht verbunden
        Serial.println();
        Serial.println("WiFi connection failed, starting AP mode");
        startAPMode();
      }
    } else {
      Serial.println("No WiFi credentials, starting AP mode");
      startAPMode();
    }
  }
}

void checkWiFi() {
  // MQTT Loop (nicht blockierend)
  if (mqtt_enabled && !ap_mode && onlineStatus) {
    mqttClient.loop(); // Zuerst loop aufrufen
    if (!mqttClient.connected()) {
      reconnectMQTT(); // Dann reconnect (mit eigenem Timeout)
    }
  }
  
  if(wifi_enabled) {
    if (millis() - lastWiFiCheck >= 10000) { // Alle 10 Sekunden prüfen (schneller)
      lastWiFiCheck = millis();

      // Prüfen ob AP-Modus aktiv ist
      if (ap_mode) {
        // Im AP-Modus KEINE parallele STA-Verbindung versuchen, um AP stabil zu halten
      } else {
        // Kein AP: normale Reconnect-Logik
        if (WiFi.status() != WL_CONNECTED) {
          onlineStatus = false;
          Serial.print("WiFi disconnected (status=");
          Serial.print(WiFi.status());
          Serial.println("), attempting to reconnect...");
          
          // WiFi.begin ist non-blocking auf ESP32, Verbindung läuft im Hintergrund
          WiFi.reconnect();
          
          // Warte bis zu 5 Sekunden auf Verbindung
          unsigned long reconnectStart = millis();
          while (WiFi.status() != WL_CONNECTED && (millis() - reconnectStart < 5000)) {
            delay(100);
            yield(); // Gib anderen Tasks Zeit
          }
          
          if (WiFi.status() == WL_CONNECTED) {
            Serial.println("WiFi reconnected successfully");
            onlineStatus = true;
            // Vorsichtshalber DNS stoppen, falls noch aktiv
            dnsServer.stop();
          } else {
            Serial.print("WiFi reconnect failed (status=");
            Serial.print(WiFi.status());
            Serial.println(")");\
          }
        } else {
          onlineStatus = true; // Setze Online-Status auf true, wenn verbunden
        }
      }
    }
  }

}

void startAPMode() {
  ap_mode = true;
  if(wifi_enabled) statusLedEnabled = true; // Status LED immer aktivieren, wenn WiFi aktiviert ist

  // Stabiler reiner AP-Modus
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid.c_str(), ap_password.c_str());
  
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP Mode started. IP: ");
  Serial.println(apIP);

  // DNS fängt alle Domains ab -> Captive Portal
  dnsServer.start(DNS_PORT, "*", apIP);

}

void setupWebServer() {
  // Statische Dateien
  server.on("/", HTTP_GET, handleRoot);
  server.on("/index.html", HTTP_GET, handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleJS);
  server.on("/favicon.ico", handleFavicon);
  
  // Captive Portal / OS Connectivity Checks auf mDNS umleiten
  server.on("/generate_204", HTTP_ANY, handleCaptiveRedirect);       // Android
  server.on("/hotspot-detect.html", HTTP_ANY, handleCaptiveRedirect); // iOS/macOS
  server.on("/fwlink", HTTP_ANY, handleCaptiveRedirect);              // Windows alt
  server.on("/ncsi.txt", HTTP_ANY, [](){                              // Windows NCSI (bewusst 200 OK)
    server.send(200, "text/plain", "Microsoft NCSI");
  });
  
  // API Endpoints
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/scan", HTTP_GET, handleWiFiScan);
  server.on("/api/wifi", HTTP_POST, handleWiFiConfig);
  server.on("/api/mqtt", HTTP_POST, handleMQTTConfig);
  server.on("/api/effect", HTTP_POST, handleEffectConfig);
  server.on("/api/brightness", HTTP_POST, handleBrightness);
  server.on("/api/auto-timer", HTTP_POST, handleAutoTimer);
  server.on("/api/status-led", HTTP_POST, handleStatusLed);
  server.on("/api/reset", HTTP_POST, handleReset);
  server.on("/api/logout", HTTP_POST, handleLogout);
  
  server.onNotFound(handleNotFound);
}

void setupMQTT() {
  if (mqtt_server.length() > 0) {
    wifiClient.setInsecure(); // Use this only for testing, it allows connecting without a root certificate
    wifiClient.setTimeout(5); // 5 Sekunden Timeout für TLS
    mqttClient.setServer(mqtt_server.c_str(), mqtt_port);
    mqttClient.setCallback(mqttCallback); // Callback setzen
    mqttClient.setSocketTimeout(5); // 5 Sekunden Socket-Timeout
    // KEIN reconnectMQTT() hier! Wird im loop() aufgerufen
  }
}

void reconnectMQTT() {
  if (mqtt_server.length() == 0) return;
  if (!wifi_enabled || ap_mode) return; // Nur verbinden wenn WiFi aktiv und nicht im AP-Modus
  
  static unsigned long lastAttempt = 0;
  if (millis() - lastAttempt < 15000) return; // 15 Sekunden zwischen Versuchen
  lastAttempt = millis();
  
  // Prüfen ob WiFi verbunden ist
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("MQTT: Warte auf WiFi-Verbindung");
    return;
  }
  
  Serial.println("MQTT: Verbindungsversuch...");
  String clientId = "WI-" + getChipId();
  
  bool connected = false;
  if (mqtt_user.length() > 0) {
    connected = mqttClient.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_password.c_str());
  } else {
    connected = mqttClient.connect(clientId.c_str());
  }
  
  if (connected) {
    Serial.println("MQTT verbunden");
    mqttClient.subscribe((mqtt_topic).c_str());
  } else {
    Serial.print("MQTT Verbindung fehlgeschlagen, rc=");
    Serial.println(mqttClient.state());
  }
}

void publishStatus() {
  if (mqttClient.connected()) {
    JsonDocument doc;
    doc["chip_id"] = getChipId();
    doc["version"] = currentVersion;
    doc["ip"] = ap_mode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
    doc["effect"] = currentEffect;
    doc["uptime"] = millis() / 1000; // Uptime in Sekunden
    
    String payload;
    serializeJson(doc, payload);
    
    mqttClient.publish(mqtt_topic.c_str(), payload.c_str());
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Payload zu String konvertieren
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("MQTT Message received: ");
  Serial.println(message);
  
  // JSON parsen
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, message);
  
  if (error) {
    Serial.print("JSON Parse error: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Prüfen ob dev_id mit unserer Chip-ID übereinstimmt
  String dev_id = doc["dev_id"].as<String>();
  if (dev_id != getChipId()) {
    Serial.println("Message not for this device");
    return;
  }
  
  // finger_count und color extrahieren
  if (doc["finger_count"].is<int>()) {
    fingerCount = doc["finger_count"];
    if (doc["color"].is<String>()) {
      fingerColor = doc["color"].as<String>();
    }
    
    // Validierung
    if (fingerCount >= 0 && fingerCount <= 5) {
      fingerMode = true;
      Serial.print("Finger mode activated: ");
      Serial.print(fingerCount);
      Serial.print(" LEDs, Color: ");
      Serial.println(fingerColor);
    } else {
      Serial.println("Invalid finger_count (must be 0-5)");
    }
  }
}

void handleButton() {
  bool currentButtonState = digitalRead(BUTTON_PIN) == LOW;
  unsigned long currentTime = millis();
  
  // Entprellen (Debounce)
  if (currentButtonState != lastButtonState) {
    lastButtonChange = currentTime;
  }
  
  if (currentTime - lastButtonChange > 50) { // 50ms Entprellzeit
    if (currentButtonState && !buttonPressed) {
      // Button wurde gedrückt
      buttonPressed = true;
      buttonPressStart = currentTime;
    } else if (!currentButtonState && buttonPressed) {
      // Button wurde losgelassen
      buttonPressed = false;
      unsigned long pressDuration = currentTime - buttonPressStart;
      
      if (pressDuration > 8000) {
        // Lange Taste gedrückt (>5 Sekunden) = Reset
        Serial.println("Reset durch Button ausgelöst");
        clearConfig();
        ESP.restart();
      } else if (pressDuration > 2000) {
        // Mittellange Taste gedrückt (>2 Sekunden) = AP-Modus starten
        Serial.println("AP-Mode durch Button ausgelöst");
        if (!ap_mode) {
          startAPMode();
        }
      } else if (pressDuration > 100) {
        fingerMode = false; // Kurze Taste gedrückt = Finger-Modus deaktivieren
        // Kurze Taste gedrückt = LED Effekt wechseln
        // Neue Belegung: 0 = Auto, 1..6 = Effekte, 7 = Alle aus
        currentEffect = (currentEffect + 1) % 8; // 0-7 Effekte
        autoMode = (currentEffect == 0);
        saveConfig();
        runEffect(currentEffect);
        Serial.print("LED Effekt gewechselt: ");
        Serial.println(getEffectName(currentEffect));
      }
    }
  }
  
  lastButtonState = currentButtonState;
}

void updateStatusLED() {
  if (!statusLedEnabled) {
    digitalWrite(STATUS_LED_PIN, LOW); // LED aus, wenn deaktiviert
    return;
  }
  unsigned long interval = ap_mode ? 250 : (WiFi.status() == WL_CONNECTED ? 2000 : 100);
  
  if (millis() - statusLedTimer >= interval) {
    statusLedState = !statusLedState;
    digitalWrite(STATUS_LED_PIN, statusLedState);
    statusLedTimer = millis();
  }
}

void updateNeoPixels() {
  if (millis() - lastEffectUpdate < effectSpeed) return;
  lastEffectUpdate = millis();
  
  // Finger-Modus hat Priorität
  if (fingerMode) {
    fingerLedEffect();
    if (fingerModeTimer == 0) fingerModeTimer = millis();
    if (millis() - fingerModeTimer > 5000) { // fingerMode nach 5s beenden
      fingerMode = false;
      fingerModeTimer = 0;
    }
  } else if (autoMode) {
    // Auto-Modus: zyklisch durch 1..6 (User1, User2, User3, Rainbow, Theater, Fire)
    if (millis() - autoModeTimer > currentAutoTimer * 1000) { // konfigurierte Wechelzeit pro Effekt
      autoModeIndex = (autoModeIndex % 6) + 1; // 1..6
      autoModeTimer = millis();
    }
    if (autoModeIndex < 1 || autoModeIndex > 6) autoModeIndex = 1;
    runEffect(autoModeIndex);
  } else {
    runEffect(currentEffect);
  }
  
  ring.show();
}

void runEffect(int effect) {
  static unsigned int step = 0;
  step++;
  
  switch (effect) {
    case 0: // Auto-Modus (Logik in updateNeoPixels), hier nichts tun
      break;

    case 1: // User Effekt 1
      deinEffekt1(step);
      break;

    case 2: // User Effekt 2
      deinEffekt2(step);
      break;

    case 3: // User Effekt 3
      deinEffekt3(step);
      break;

    case 4: // Rainbow
      effectSpeed = DEFAULT_EFFECT_SPEED; // Standardwert für vordefinierte Effekte
      rainbowEffect(step);
      break;

    case 5: // Theater Chase
      effectSpeed = DEFAULT_EFFECT_SPEED; // Standardwert für vordefinierte Effekte
      theaterChaseEffect(step, ring.Color(127, 127, 127));
      break;

    case 6: // Fire
      effectSpeed = DEFAULT_EFFECT_SPEED; // Standardwert für vordefinierte Effekte
      fireEffect(step);
      break;

    case 7: // Alle aus
      effectSpeed = DEFAULT_EFFECT_SPEED; // Standardwert für vordefinierte Effekte
      ring.clear();
      break;
  }
}

void rainbowEffect(int step) {
  for (int i = 0; i < ring.numPixels(); i++) {
    int pixelHue = (i * 65536L / ring.numPixels()) + (step * 256);
    ring.setPixelColor(i, ring.gamma32(ring.ColorHSV(pixelHue)));
  }
}

void colorWipeEffect(int step, uint32_t color) {
  int pos = (step / 3) % (ring.numPixels() * 2);
  if (pos < ring.numPixels()) {
    ring.setPixelColor(pos, color);
  } else {
    ring.setPixelColor(pos - ring.numPixels(), 0);
  }
}

void theaterChaseEffect(int step, uint32_t color) {
  ring.clear();
  for (int i = 0; i < ring.numPixels(); i += 3) {
    ring.setPixelColor(i + ((step / 3) % 3), color);
  }
}

void breathingEffect(int step, uint32_t color) {
  float breath = (sin(step * 0.1) + 1.0) / 2.0;
  uint8_t brightness = (uint8_t)(breath * 255);
  
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(brightness, brightness/2, brightness/4));
  }
}

void sparkleEffect(uint32_t color) {
  ring.clear();
  int pos = random(ring.numPixels());
  ring.setPixelColor(pos, color);
}

void runningLightEffect(int step, uint32_t color) {
  ring.clear();
  int pos = step % ring.numPixels();
  ring.setPixelColor(pos, color);
}

void fireEffect(int step) {
  for (int i = 0; i < ring.numPixels(); i++) {
    int heat = random(0, 255);
    heat = heat - random(0, 50);
    if (heat < 0) heat = 0;
    
    uint8_t r = heat;
    uint8_t g = heat / 3;
    uint8_t b = 0;
    
    ring.setPixelColor(i, ring.Color(r, g, b));
  }
}

void printStatus() {
  Serial.println("=== ESP32 Status ===");
  Serial.print("Chip ID: ");
  Serial.println(String(ESP.getEfuseMac(), HEX));
  Serial.print("Version: ");
  Serial.println(currentVersion);
  Serial.print("Mode: ");
  Serial.println(ap_mode ? "Access Point" : "Station");
  Serial.print("WiFi: ");
  Serial.println(wifi_enabled ? "Enabled" : "Disabled");
  Serial.print("IP: ");
  Serial.println(ap_mode ? WiFi.softAPIP().toString() : WiFi.localIP().toString());
  Serial.print("MQTT: ");
  Serial.println(mqtt_enabled ? "Enabled" : "Disabled");
  Serial.print("Effect: ");
  Serial.println(currentEffect);
  Serial.print("Status-LED: ");
  Serial.println(statusLedEnabled ? "Enabled" : "Disabled");
  Serial.println("==================");
}

void handleRoot() {
  if (!webAuthenticate()) return;
  File file = LittleFS.open("/index.html", "r");
  if (file) {
    server.streamFile(file, "text/html");
    file.close();
  } else {
    // Fallback HTML wenn Datei nicht existiert
    String html = "<!DOCTYPE html><html><head><title>smartLight Config</title></head><body>";
    html += "<h1>smartLight Configuration</h1>";
    html += "<p>Web interface files not found. Please upload the LittleFS files.</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  }
}

void handleCSS() {
  if (!webAuthenticate()) return;
  File file = LittleFS.open("/style.css", "r");
  if (file) {
    server.streamFile(file, "text/css");
    file.close();
  } else {
    server.send(404, "text/plain", "CSS file not found");
  }
}

void handleJS() {
  if (!webAuthenticate()) return;
  File file = LittleFS.open("/script.js", "r");
  if (file) {
    server.streamFile(file, "application/javascript");
    file.close();
  } else {
    server.send(404, "text/plain", "JavaScript file not found");
  }
}

void handleStatus() {
  JsonDocument doc;
  doc["chip_id"] = getChipId();
  doc["user_name"] = getDeinName();
  doc["uptime"] = millis() / 1000; // Uptime in Sekunden
  doc["mode"] = ap_mode ? "Access Point" : "Station";
  doc["ip"] = ap_mode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
  doc["wifi_status"] = ap_mode ? "AP Mode" : (WiFi.status() == WL_CONNECTED ? "Verbunden" : "Getrennt");
  doc["mqtt_status"] = mqtt_enabled ? (mqttClient.connected() ? "Verbunden" : "Getrennt") : "Deaktiviert";
  doc["current_effect"] = getEffectName(currentEffect);
  doc["wifi_enabled"] = wifi_enabled;
  doc["wifi_ssid"] = wifi_ssid;
  doc["mqtt_enabled"] = mqtt_enabled;
  doc["mqtt_server"] = mqtt_server;
  doc["mqtt_port"] = mqtt_port;
  doc["mqtt_user"] = mqtt_user;
  doc["mqtt_topic"] = mqtt_topic;
  doc["effect"] = currentEffect;
  doc["brightness"] = currentBrightness;
  doc["auto_timer"] = currentAutoTimer;
  doc["version"] = currentVersion;
  doc["led_enabled"] = statusLedEnabled;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleWiFiScan() {
  JsonDocument doc;
  JsonArray networks = doc.to<JsonArray>();
  
  int n = WiFi.scanNetworks();
  
  for (int i = 0; i < n; i++) {
    JsonObject network = networks.add<JsonObject>();
    network["ssid"] = WiFi.SSID(i);
    network["rssi"] = WiFi.RSSI(i);
    network["encryption"] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "open" : "encrypted";
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleWiFiConfig() {
  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));
  
  wifi_enabled = doc["enabled"];
  wifi_ssid = doc["ssid"].as<String>();
  wifi_password = doc["password"].as<String>();
  
  saveConfig();
  
  JsonDocument response;
  response["success"] = true;
  response["message"] = "WiFi konfiguriert";
  
  String responseStr;
  serializeJson(response, responseStr);
  server.send(200, "application/json", responseStr);
  
  // Restart nach kurzer Verzögerung
  delay(1000);
  ESP.restart();
}

void handleMQTTConfig() {
  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));
  
  mqtt_enabled = doc["enabled"];
  mqtt_server = doc["server"].as<String>();
  mqtt_port = doc["port"];
  mqtt_user = doc["user"].as<String>();
  mqtt_password = doc["password"].as<String>();
  mqtt_topic = doc["topic"].as<String>();
  
  saveConfig();
  loadConfig(); // Konfiguration neu laden
  
  // MQTT neu initialisieren wenn aktiviert
  if (mqtt_enabled && !ap_mode) {
    setupMQTT();
  }
  
  JsonDocument response;
  response["success"] = true;
  response["message"] = "MQTT konfiguriert";
  
  String responseStr;
  serializeJson(response, responseStr);
  server.send(200, "application/json", responseStr);
}

void handleEffectConfig() {
  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));
  
  int requested = doc["effect"] | 0;
  if (requested < 0) requested = 0;
  if (requested > 7) requested = 7;
  currentEffect = requested;
  autoMode = (currentEffect == 0);
  saveConfig();
  
  JsonDocument response;
  response["success"] = true;
  response["message"] = "Effekt geändert";
  
  String responseStr;
  serializeJson(response, responseStr);
  server.send(200, "application/json", responseStr);
}

void handleBrightness() {
  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));
  
  int brightness = doc["brightness"];
  if (brightness >= 0 && brightness <= 100) {
    currentBrightness = brightness;
    ring.setBrightness(currentBrightness);
    saveConfig();
    
    // Publish to MQTT if enabled
    if (mqtt_enabled && mqttClient.connected()) {
      mqttClient.publish((mqtt_topic + "/brightness/status").c_str(), String(brightness).c_str());
    }
    
    JsonDocument response;
    response["success"] = true;
    
    String responseStr;
    serializeJson(response, responseStr);
    server.send(200, "application/json", responseStr);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid brightness\"}");
  }
}

void handleAutoTimer() {
  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));
  
  int auto_timer = doc["auto_timer"];

  if (auto_timer >= 0 && auto_timer <= 600) {
    currentAutoTimer = auto_timer;
    saveConfig();
    
    // Publish to MQTT if enabled
    if (mqtt_enabled && mqttClient.connected()) {
      mqttClient.publish((mqtt_topic + "/autotimer/status").c_str(), String(auto_timer).c_str());
    }
    
    JsonDocument response;
    response["success"] = true;
    
    String responseStr;
    serializeJson(response, responseStr);
    server.send(200, "application/json", responseStr);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid auto-timer\"}");
  }
}

void handleStatusLed() {
  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));
  
  statusLedEnabled = doc["enabled"];
  saveConfig();
  
  JsonDocument response;
  response["success"] = true;
  response["message"] = "Status LED geändert";
  
  String responseStr;
  serializeJson(response, responseStr);
  server.send(200, "application/json", responseStr);
}

void handleReset() {
  clearConfig();
  
  JsonDocument response;
  response["success"] = true;
  response["message"] = "Konfiguration gelöscht";
  
  String responseStr;
  serializeJson(response, responseStr);
  server.send(200, "application/json", responseStr);
  
  // Restart nach kurzer Verzögerung
  delay(1000);
  ESP.restart();
}

void handleLogout() {
  JsonDocument response;
  // Erfolgreiche Abmeldung - 401 senden um Browser-Credentials zu löschen
  response["success"] = true;
  response["message"] = "Erfolgreich abgemeldet";
  
  String responseStr;
  serializeJson(response, responseStr);

  Serial.println("Benutzer erfolgreich abgemeldet");
  String www_realm = "ESP32 LED Controller";

  // 401 Unauthorized senden um Browser zum Löschen der Credentials zu zwingen
  server.sendHeader("WWW-Authenticate", "Basic realm=\"" + String(www_realm) + "\"");
  server.send(401, "application/json", responseStr);
}

void handleNotFound() {
  // Versuche, statische Dateien aus LittleFS zu liefern
  String path = server.uri();
  if (path.length() == 0) path = "/";
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    if (file) {
      String contentType = "text/plain";
      if (path.endsWith(".html")) contentType = "text/html";
      else if (path.endsWith(".css")) contentType = "text/css";
      else if (path.endsWith(".js")) contentType = "application/javascript";
      else if (path.endsWith(".ico")) contentType = "image/x-icon";
      server.streamFile(file, contentType);
      file.close();
      return;
    }
  }
  if (ap_mode) {
    // Im AP-Modus auf AP-IP umleiten (robuster als mDNS)
    String target = String("http://") + WiFi.softAPIP().toString() + "/";
    server.sendHeader("Location", target, true);
    server.send(302, "text/plain", "");
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.println(message);
  server.send(404, "text/plain", message);
}

void handleFavicon() {
  server.send(204);
}

// Captive-Portal Redirect: leite auf mDNS Hostname
void handleCaptiveRedirect() {
  if (ap_mode) {
    // Auf die AP-IP umleiten, damit alle Clients (auch ohne mDNS) funktionieren
    String target = String("http://") + WiFi.softAPIP().toString() + "/";
    server.sendHeader("Location", target, true);
    server.send(302, "text/plain", "");
  } else {
    handleRoot();
  }
}

String getEffectName(int effect) {
  switch (effect) {
    case 0: return "Auto-Modus";
    case 1: return "Dein Effekt 1";
    case 2: return "Dein Effekt 2";
    case 3: return "Dein Effekt 3";
    case 4: return "Regenbogen";
    case 5: return "Pixelrennen";
    case 6: return "Feuer";
    case 7: return "Alle aus";
    default: return "Unbekannt";
  }
}

String getChipId() {
  uint64_t chipid = ESP.getEfuseMac(); // Liest die MAC-Adresse (Chip-ID)
  char chipid_str[13];
  snprintf(chipid_str, sizeof(chipid_str), "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  return String(chipid_str);
}

// Authentifizierungsfunktion
bool webAuthenticate() {
  if (!useAuthentication) return true; // Auth deaktiviert -> direkt weiter
  if (!server.authenticate(http_username, http_password)) {
    server.requestAuthentication(); // Authentifizierung anfordern
    return false;
  }
  return true;
}

void fingerLedEffect() {
  ring.clear();
  
  if (fingerCount > 0) {
    uint32_t color = hexToNeoPixelColor(fingerColor.c_str());
    
    for (int finger = 0; finger < fingerCount; finger++) {
      int startPos = finger * 5; // Jeder Finger beginnt bei Position 0, 5, 10, 15, 20
      
      // 2 LEDs pro Finger einschalten
      for (int led = 0; led < 2; led++) {
        int ledPos = startPos + led;
        if (ledPos < ring.numPixels()) {
          ring.setPixelColor(ledPos, color);
        }
      }
    }
  }
}

// Hilfsfunktion zum Parsen der Farbe:
u_int32_t hexToNeoPixelColor(const char* hexColor) {
  // Überprüfen auf ungültige Eingabe: Nullzeiger, falsche Länge oder fehlendes '#'-Präfix
  if (hexColor == nullptr || strlen(hexColor) != 7 || hexColor[0] != '#') {
    // Ungültiges Eingabeformat, gib ROT zurück (oder eine andere Fehlerfarbe)
    return ring.Color(255, 0, 0);
  }

  // Parsen der Hexadezimal-Teile für Rot, Grün, Blau
  // Erstellen von temporären Zeichenketten für jede Farbkomponente
  char hexRed[3] = {hexColor[1], hexColor[2], '\0'};    // Zeichen 1 und 2 für Rot
  char hexGreen[3] = {hexColor[3], hexColor[4], '\0'};  // Zeichen 3 und 4 für Grün
  char hexBlue[3] = {hexColor[5], hexColor[6], '\0'};   // Zeichen 5 und 6 für Blau

  // Konvertieren der Hex-Teile in Integer-Werte (0-255)
  // strtol (string to long) wird verwendet, um eine Zeichenkette in eine Zahl umzuwandeln.
  // Der dritte Parameter '16' gibt an, dass die Basis Hexadezimal ist.
  uint8_t r = strtol(hexRed, nullptr, 16);
  uint8_t g = strtol(hexGreen, nullptr, 16);
  uint8_t b = strtol(hexBlue, nullptr, 16);

  // Den kombinierten Farbwert mit strip.Color() zurückgeben
  // strip.Color() kombiniert die 8-Bit-RGB-Werte zu einem 32-Bit-Farbwert
  return ring.Color(r, g, b);
}
