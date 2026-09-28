#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LittleFS.h>
#include <Preferences.h>

#define PZEM_RX_PIN 23
#define PZEM_TX_PIN 22
PZEM004Tv30 pzem(Serial2, PZEM_RX_PIN, PZEM_TX_PIN);

#define DEFAULT_OV 250
#define DEFAULT_UV 200

#define GREEN_LED_PIN 25
#define RED_LED_PIN 26
#define BUZZER_PIN 27
#define RELAY_PIN 21

#define RELAY_ON LOW
#define RELAY_OFF HIGH

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

LiquidCrystal_I2C lcd(0x27, 16, 2);
Preferences prefs;

float voltage = 0;
float current = 0;
float power = 0;
float energy = 0;
float frequency = 0;
float pf = 0;

int ovThreshold = DEFAULT_OV;
int uvThreshold = DEFAULT_UV;

unsigned long bootTime = 0;
unsigned long lastUpdate = 0;
unsigned long lastBuzzerTime = 0;

bool pzemDetected = false;
bool buzzerState = false;

const unsigned long BUZZER_INTERVAL = 500;

const char* getStatus() {
  if (!pzemDetected) return "NO READING";
  if (voltage > ovThreshold) return "OVER VOLTAGE";
  if (voltage < uvThreshold) return "UNDER VOLTAGE";
  return "NORMAL";
}

void updateProtection() {
  if (!pzemDetected) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, RELAY_OFF);
    buzzerState = false;
    return;
  }

  const char* status = getStatus();

  if (strcmp(status, "NORMAL") == 0) {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, RELAY_ON);
    buzzerState = false;
  } else {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, RELAY_OFF);

    if (millis() - lastBuzzerTime >= BUZZER_INTERVAL) {
      lastBuzzerTime = millis();
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  }
}

String buildJson() {
  String json = "{";
  json += "\"voltage\":" + String(voltage, 2) + ",";
  json += "\"current\":" + String(current, 3) + ",";
  json += "\"power\":" + String(power, 2) + ",";
  json += "\"energy\":" + String(energy, 3) + ",";
  json += "\"frequency\":" + String(frequency, 2) + ",";
  json += "\"pf\":" + String(pf, 2) + ",";
  json += "\"status\":\"" + String(getStatus()) + "\",";
  json += "\"ov\":" + String(ovThreshold) + ",";
  json += "\"uv\":" + String(uvThreshold);
  json += "}";
  return json;
}

void handleRoot() {
  File file = LittleFS.open("/index.html", "r");

  if (!file) {
    server.send(404, "text/plain", "Dashboard files not found. Upload the Dashboard files to LittleFS.");
    return;
  }

  server.streamFile(file, "text/html");
  file.close();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_DISCONNECTED) {
    Serial.printf("[%u] Disconnected!\n", num);
  } else if (type == WStype_CONNECTED) {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);

    String json = buildJson();
    webSocket.sendTXT(num, json);
  } else if (type == WStype_TEXT) {
    char msg[64];
    size_t n = length < sizeof(msg) - 1 ? length : sizeof(msg) - 1;
    memcpy(msg, payload, n);
    msg[n] = '\0';

    int ov;
    int uv;

    if (sscanf(msg, "{\"ov\":%d,\"uv\":%d}", &ov, &uv) == 2 && uv > 0 && ov > uv && ov <= 500) {
      ovThreshold = ov;
      uvThreshold = uv;

      prefs.putInt("ov", ovThreshold);
      prefs.putInt("uv", uvThreshold);

      Serial.printf("Thresholds saved: OV=%d UV=%d\n", ovThreshold, uvThreshold);

      updateProtection();
    } else {
      Serial.println("Invalid threshold message ignored");
    }

    String json = buildJson();
    webSocket.broadcastTXT(json);
  }
}

void sendSensorData() {
  float v = pzem.voltage();
  float i = pzem.current();
  float p = pzem.power();
  float e = pzem.energy();
  float f = pzem.frequency();
  float pfv = pzem.pf();

  voltage = (isnan(v) || isinf(v)) ? 0.0 : v;
  current = (isnan(i) || isinf(i)) ? 0.0 : i;
  power = (isnan(p) || isinf(p)) ? 0.0 : p;
  energy = (isnan(e) || isinf(e)) ? 0.0 : e;
  frequency = (isnan(f) || isinf(f)) ? 0.0 : f;
  pf = (isnan(pfv) || isinf(pfv)) ? 0.0 : pfv;

  if (voltage > 0 || current > 0 || power > 0 || frequency > 0) {
    pzemDetected = true;
  }

  Serial.printf("PZEM -> V:%.2f I:%.3f P:%.2f E:%.3f F:%.2f PF:%.2f\n", voltage, current, power, energy, frequency, pf);

  updateProtection();

  String json = buildJson();
  webSocket.broadcastTXT(json);
}

void updateLCD() {
  static unsigned long lastLCDUpdate = 0;

  if (millis() - lastLCDUpdate < 1000) return;

  lastLCDUpdate = millis();

  if (millis() - bootTime <= 90000) {
    lcd.setCursor(0, 0);
    lcd.print("IP Address:     ");

    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    lcd.print("      ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(voltage, 1);
    lcd.print(" I:");
    lcd.print(current, 2);
    lcd.print("   ");

    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print(power, 1);
    lcd.print(" PF:");
    lcd.print(pf, 2);
    lcd.print("   ");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP32 Smart Energy Monitor");

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  Wire.begin(33, 32);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Energy Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
  }

  prefs.begin("energy", false);

  ovThreshold = prefs.getInt("ov", DEFAULT_OV);
  uvThreshold = prefs.getInt("uv", DEFAULT_UV);

  Serial2.begin(9600, SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  Serial.printf("Connecting to: %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Connected!");
  Serial.print("Dashboard: http://");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP Address:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  bootTime = millis();

  server.on("/", handleRoot);
  server.serveStatic("/style.css", LittleFS, "/style.css");
  server.serveStatic("/script.js", LittleFS, "/script.js");
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("Servers Started!");
}

void loop() {
  server.handleClient();
  webSocket.loop();

  if (millis() - lastUpdate >= 1000) {
    sendSensorData();
    updateLCD();
    lastUpdate = millis();
  }
}
