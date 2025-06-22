// TODO: 
// OTA updates
// README.md aktualisieren

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

// Hardware Definitionen
#define NEOPIXEL_PIN 3
#define NEOPIXEL_COUNT 24
#define BRIGHTNESS  20    // Standard Neopixel Helligkeit (0-100)
#define STATUS_LED_PIN 2
#define BUTTON_PIN 9

// Authentifizierungsdaten (hardcoded)
const char* http_username = "admin";
const char* http_password = "admin";

// WiFi und Server
WebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Preferences preferences;

// NeoPixel
Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Konfigurationsvariablen
String wifi_ssid = "";
String wifi_password = "";
bool ap_mode = false;
String ap_ssid = "ESP32-Config";
String ap_password = "12345678";

String mqtt_server = "";
int mqtt_port = 8883;
String mqtt_user = "";
String mqtt_password = "";
String mqtt_topic = "esp32/status";
bool mqtt_enabled = false;

// LED Effekte
int currentEffect = 0;
int currentBrightness = BRIGHTNESS;
unsigned long lastEffectUpdate = 0;
int effectSpeed = 50;
bool autoMode = false;
unsigned long autoModeTimer = 0;
int autoModeIndex = 0;
int currentAutoTimer = 30;

// Button
bool buttonPressed = false;
unsigned long buttonPressStart = 0;
unsigned long lastButtonChange = 0;
bool lastButtonState = true;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Status LED
bool statusLedState = false;
unsigned long statusLedTimer = 0;

// Funktionen Prototypen
void loadConfig();
void saveConfig();
void clearConfig();
void startWiFi();
void startAPMode();
void setupWebServer();
void setupMQTT();
void reconnectMQTT();
void publishStatus();
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
void handleNotFound();
String getEffectName(int effect);
String getChipId();
bool webAuthenticate();

void setup() {
  Serial.begin(115200);
  delay(3000); // Wartezeit für serielle Verbindung
  Serial.println("Starting ESP32 LED Controller");
  
  // Hardware initialisieren
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  strip.begin();
  strip.show();
  strip.setBrightness(20);
  
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
  if (!ap_mode) {
    if (MDNS.begin("esp32-config")) {
      Serial.println("mDNS responder started");
      MDNS.addService("http", "tcp", 80);
    }
  }
  
  // Webserver starten
  setupWebServer();
  server.begin();
  
  // MQTT initialisieren
  if (mqtt_enabled && !ap_mode) {
    setupMQTT();
  }
  
  Serial.println("Setup complete");
  printStatus();
}

void loop() {
  server.handleClient();
  
  handleButton();
  updateStatusLED();
  updateNeoPixels();
  
  if (mqtt_enabled && !ap_mode) {
    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();
  }
  
  delay(10);
}

void loadConfig() {
  preferences.begin("config", false);
  
  wifi_ssid = preferences.getString("wifi_ssid", "");
  wifi_password = preferences.getString("wifi_pass", "");
  
  mqtt_server = preferences.getString("mqtt_server", "");
  mqtt_port = preferences.getInt("mqtt_port", 8883);
  mqtt_user = preferences.getString("mqtt_user", "");
  mqtt_password = preferences.getString("mqtt_pass", "");
  mqtt_topic = preferences.getString("mqtt_topic", "esp32/status");
  mqtt_enabled = preferences.getBool("mqtt_enabled", false);
  
  currentEffect = preferences.getInt("effect", 0);
  currentBrightness = preferences.getInt("brightness", BRIGHTNESS);
  currentAutoTimer = preferences.getInt("auto_timer", 30);
  
  strip.setBrightness(currentBrightness);

  if(currentEffect == 8) {
    autoMode = true; // Auto-Modus aktivieren, wenn Effekt 8
  } else {
    autoMode = false;
  }
  
  preferences.end();
}

void saveConfig() {
  preferences.begin("config", false);
  
  preferences.putString("wifi_ssid", wifi_ssid);
  preferences.putString("wifi_pass", wifi_password);
  
  preferences.putString("mqtt_server", mqtt_server);
  preferences.putInt("mqtt_port", mqtt_port);
  preferences.putString("mqtt_user", mqtt_user);
  preferences.putString("mqtt_pass", mqtt_password);
  preferences.putString("mqtt_topic", mqtt_topic);
  preferences.putBool("mqtt_enabled", mqtt_enabled);
  
  preferences.putInt("effect", currentEffect);
  preferences.putInt("brightness", currentBrightness);
  preferences.putInt("auto_timer", currentAutoTimer);
  
  preferences.end();
}

void clearConfig() {
  preferences.begin("config", false);
  preferences.clear();
  preferences.end();
  
  // Reset zu Standardwerten
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
  
  Serial.println("Configuration cleared");
}

void startWiFi() {
  if (wifi_ssid.length() > 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("Connected! IP: ");
      Serial.println(WiFi.localIP());
      ap_mode = false;
    } else {
      Serial.println();
      Serial.println("WiFi connection failed, starting AP mode");
      startAPMode();
    }
  } else {
    Serial.println("No WiFi credentials, starting AP mode");
    startAPMode();
  }
}

void startAPMode() {
  ap_mode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid.c_str(), ap_password.c_str());
  
  Serial.print("AP Mode started. IP: ");
  Serial.println(WiFi.softAPIP());
}

void setupWebServer() {
  // Statische Dateien
  server.on("/", HTTP_GET, handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleJS);
  
  // API Endpoints
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/scan", HTTP_GET, handleWiFiScan);
  server.on("/api/wifi", HTTP_POST, handleWiFiConfig);
  server.on("/api/mqtt", HTTP_POST, handleMQTTConfig);
  server.on("/api/effect", HTTP_POST, handleEffectConfig);
  server.on("/api/brightness", HTTP_POST, handleBrightness);
  server.on("/api/auto-timer", HTTP_POST, handleAutoTimer);
  server.on("/api/reset", HTTP_POST, handleReset);

  server.onNotFound(handleNotFound);
}

void setupMQTT() {
  if (mqtt_server.length() > 0) {
    mqttClient.setServer(mqtt_server.c_str(), mqtt_port);
    reconnectMQTT();
  }
}

void reconnectMQTT() {
  if (mqtt_server.length() == 0) return;
  
  static unsigned long lastAttempt = 0;
  if (millis() - lastAttempt < 5000) return;
  lastAttempt = millis();
  
  String clientId = "ESP32-" + getChipId();
  
  bool connected = false;
  if (mqtt_user.length() > 0) {
    connected = mqttClient.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_password.c_str());
  } else {
    connected = mqttClient.connect(clientId.c_str());
  }
  
  if (connected) {
    Serial.println("MQTT connected");
    publishStatus();
    mqttClient.subscribe((mqtt_topic + "/effect").c_str());
    mqttClient.subscribe((mqtt_topic + "/brightness").c_str());
  } else {
    Serial.print("MQTT connection failed, rc=");
    Serial.println(mqttClient.state());
  }
}

void publishStatus() {
  if (mqttClient.connected()) {
    JsonDocument doc;
    doc["chip_id"] = getChipId();
    doc["ip"] = ap_mode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
    doc["effect"] = currentEffect;
    doc["uptime"] = millis();
    
    String payload;
    serializeJson(doc, payload);
    
    mqttClient.publish(mqtt_topic.c_str(), payload.c_str());
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
      
      if (pressDuration > 5000) {
        // Lange Taste gedrückt (>5 Sekunden) = Reset
        Serial.println("Reset durch Button ausgelöst");
        clearConfig();
        ESP.restart();
      } else if (pressDuration > 100) {
        // Kurze Taste gedrückt = LED Effekt wechseln
        currentEffect = (currentEffect + 1) % 9; // 0-8 Effekte
        if(currentEffect == 8) {
          autoMode = true; // Auto-Modus aktivieren, wenn Effekt 8
        } else {
          autoMode = false;
        }
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
  unsigned long interval = ap_mode ? 250 : (WiFi.status() == WL_CONNECTED ? 1000 : 100);
  
  if (millis() - statusLedTimer >= interval) {
    statusLedState = !statusLedState;
    digitalWrite(STATUS_LED_PIN, statusLedState);
    statusLedTimer = millis();
  }
}

void updateNeoPixels() {
  if (millis() - lastEffectUpdate < effectSpeed) return;
  lastEffectUpdate = millis();
  
  if (autoMode) {
    // Auto-Modus: alle Effekte nacheinander
    if (millis() - autoModeTimer > currentAutoTimer * 1000) { // konfigurierte Wechelzeit pro Effekt
      autoModeIndex = (autoModeIndex + 1) % 8;
      autoModeTimer = millis();
    }
    if(autoModeIndex == 0) autoModeIndex = 1; // Effekt 0 = Alle aus überspringen
    runEffect(autoModeIndex);
  } else {
    runEffect(currentEffect);
  }
  
  strip.show();
}

void runEffect(int effect) {
  static int step = 0;
  step++;
  
  switch (effect) {
    case 0: // Alle aus
      strip.clear();
      break;
      
    case 1: // Rainbow
      rainbowEffect(step);
      break;
      
    case 2: // Color Wipe
      colorWipeEffect(step, strip.Color(255, 0, 0));
      break;
      
    case 3: // Theater Chase
      theaterChaseEffect(step, strip.Color(127, 127, 127));
      break;
      
    case 4: // Breathing
      breathingEffect(step, strip.Color(0, 255, 0));
      break;
      
    case 5: // Sparkle
      sparkleEffect(strip.Color(255, 255, 255));
      break;
      
    case 6: // Running Light
      runningLightEffect(step, strip.Color(0, 0, 255));
      break;
      
    case 7: // Fire
      fireEffect(step);
      break;
  }
}

void rainbowEffect(int step) {
  for (int i = 0; i < strip.numPixels(); i++) {
    int pixelHue = (i * 65536L / strip.numPixels()) + (step * 256);
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
}

void colorWipeEffect(int step, uint32_t color) {
  int pos = (step / 3) % (strip.numPixels() * 2);
  if (pos < strip.numPixels()) {
    strip.setPixelColor(pos, color);
  } else {
    strip.setPixelColor(pos - strip.numPixels(), 0);
  }
}

void theaterChaseEffect(int step, uint32_t color) {
  strip.clear();
  for (int i = 0; i < strip.numPixels(); i += 3) {
    strip.setPixelColor(i + ((step / 3) % 3), color);
  }
}

void breathingEffect(int step, uint32_t color) {
  float breath = (sin(step * 0.1) + 1.0) / 2.0;
  uint8_t brightness = (uint8_t)(breath * 255);
  
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(brightness, brightness/2, brightness/4));
  }
}

void sparkleEffect(uint32_t color) {
  strip.clear();
  int pos = random(strip.numPixels());
  strip.setPixelColor(pos, color);
}

void runningLightEffect(int step, uint32_t color) {
  strip.clear();
  int pos = step % strip.numPixels();
  strip.setPixelColor(pos, color);
}

void fireEffect(int step) {
  for (int i = 0; i < strip.numPixels(); i++) {
    int heat = random(0, 255);
    heat = heat - random(0, 50);
    if (heat < 0) heat = 0;
    
    uint8_t r = heat;
    uint8_t g = heat / 3;
    uint8_t b = 0;
    
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

void printStatus() {
  Serial.println("=== ESP32 Status ===");
  Serial.print("Chip ID: ");
  Serial.println(String(ESP.getEfuseMac(), HEX));
  Serial.print("Mode: ");
  Serial.println(ap_mode ? "Access Point" : "Station");
  Serial.print("IP: ");
  Serial.println(ap_mode ? WiFi.softAPIP().toString() : WiFi.localIP().toString());
  Serial.print("MQTT: ");
  Serial.println(mqtt_enabled ? "Enabled" : "Disabled");
  Serial.print("Effect: ");
  Serial.println(currentEffect);
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
    String html = "<!DOCTYPE html><html><head><title>ESP32 Config</title></head><body>";
    html += "<h1>ESP32 Configuration</h1>";
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
  doc["mode"] = ap_mode ? "Access Point" : "Station";
  doc["ip"] = ap_mode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
  doc["wifi_status"] = ap_mode ? "AP Mode" : (WiFi.status() == WL_CONNECTED ? "Verbunden" : "Getrennt");
  doc["mqtt_status"] = mqtt_enabled ? (mqttClient.connected() ? "Verbunden" : "Getrennt") : "Deaktiviert";
  doc["current_effect"] = getEffectName(currentEffect);
  doc["wifi_ssid"] = wifi_ssid;
  doc["mqtt_enabled"] = mqtt_enabled;
  doc["mqtt_server"] = mqtt_server;
  doc["mqtt_port"] = mqtt_port;
  doc["mqtt_user"] = mqtt_user;
  doc["mqtt_topic"] = mqtt_topic;
  doc["effect"] = currentEffect;
  doc["brightness"] = currentBrightness;
  doc["auto_timer"] = currentAutoTimer;
  
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
  
  currentEffect = doc["effect"];
  autoMode = (currentEffect == 8);
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
    strip.setBrightness(currentBrightness);
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

void handleNotFound() {
  server.send(404, "text/plain", "File Not Found");
}

String getEffectName(int effect) {
  switch (effect) {
    case 0: return "Alle aus";
    case 1: return "Rainbow";
    case 2: return "Color Wipe";
    case 3: return "Theater Chase";
    case 4: return "Breathing";
    case 5: return "Sparkle";
    case 6: return "Running Light";
    case 7: return "Fire";
    case 8: return "Auto-Modus";
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
  if (!server.authenticate(http_username, http_password)) {
    server.requestAuthentication(); // Authentifizierung anfordern
    return false;
  }
  return true;
}