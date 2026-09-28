#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Preferences.h>
#include "dashboard.h"

// PZEM UART Pins
#define PZEM_RX_PIN 23
#define PZEM_TX_PIN 22
PZEM004Tv30 pzem(Serial2, PZEM_RX_PIN, PZEM_TX_PIN);

// Default voltage thresholds (V)
#define DEFAULT_OV 260
#define DEFAULT_UV 180

unsigned long bootTime = 0;

// Wi-Fi Credentials (replace before uploading, never commit real values)
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Servers
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Sensor values
float voltage = 0, current = 0, power = 0, energy = 0, frequency = 0, pf = 0;
unsigned long lastUpdate = 0;

// Voltage thresholds (stored in flash)
Preferences prefs;
int ovThreshold = DEFAULT_OV;
int uvThreshold = DEFAULT_UV;

// LCD Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* getStatus() {
  if (voltage <= 0) return "NO READING";
  if (voltage > ovThreshold) return "OVER VOLTAGE";
  if (voltage < uvThreshold) return "UNDER VOLTAGE";
  return "NORMAL";
}

String buildJson() {
  String json = "{";
  json += "\"voltage\":"   + String(voltage, 2)   + ",";
  json += "\"current\":"   + String(current, 3)   + ",";
  json += "\"power\":"     + String(power, 2)     + ",";
  json += "\"energy\":"    + String(energy, 3)    + ",";
  json += "\"frequency\":" + String(frequency, 2) + ",";
  json += "\"pf\":"        + String(pf, 2)        + ",";
  json += "\"status\":\""  + String(getStatus())  + "\",";
  json += "\"ov\":"        + String(ovThreshold)  + ",";
  json += "\"uv\":"        + String(uvThreshold);
  json += "}";
  return json;
}

// Serve the dashboard stored in flash (dashboard.h)
void handleRoot() {
  server.send_P(200, "text/html", DASHBOARD_HTML);
}

// WebSocket Event Handler
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_DISCONNECTED) {
    Serial.printf("[%u] Disconnected!\n", num);
  } else if (type == WStype_CONNECTED) {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
    String json = buildJson();
    webSocket.sendTXT(num, json);
  } else if (type == WStype_TEXT) {
    // Expected message: {"ov":260,"uv":180}
    char msg[64];
    size_t n = length < sizeof(msg) - 1 ? length : sizeof(msg) - 1;
    memcpy(msg, payload, n);
    msg[n] = '\0';

    int ov, uv;
    if (sscanf(msg, "{\"ov\":%d,\"uv\":%d}", &ov, &uv) == 2 && uv > 0 && ov > uv && ov <= 500) {
      ovThreshold = ov;
      uvThreshold = uv;
      prefs.putInt("ov", ovThreshold);
      prefs.putInt("uv", uvThreshold);
      Serial.printf("Thresholds saved: OV=%d UV=%d\n", ovThreshold, uvThreshold);
    } else {
      Serial.println("Invalid threshold message ignored");
    }
    String json = buildJson();
    webSocket.broadcastTXT(json);
  }
}

// Send Data via WebSocket
void sendSensorData() {
  float v  = pzem.voltage();
  float i  = pzem.current();
  float p  = pzem.power();
  float e  = pzem.energy();
  float f  = pzem.frequency();
  float pfv = pzem.pf();

  // Handle NaN / invalid values
  voltage   = (isnan(v)  || isinf(v))  ? 0.0 : v;
  current   = (isnan(i)  || isinf(i))  ? 0.0 : i;
  power     = (isnan(p)  || isinf(p))  ? 0.0 : p;
  energy    = (isnan(e)  || isinf(e))  ? 0.0 : e;
  frequency = (isnan(f)  || isinf(f))  ? 0.0 : f;
  pf        = (isnan(pfv)|| isinf(pfv))? 0.0 : pfv;

  Serial.printf(
    "PZEM -> V:%.2f I:%.3f P:%.2f E:%.3f F:%.2f PF:%.2f\n",
    voltage, current, power, energy, frequency, pf
  );

  String json = buildJson();
  webSocket.broadcastTXT(json);
}

// Update LCD with readings
void updateLCD() {
  static unsigned long lastLCDUpdate = 0;

  if (millis() - lastLCDUpdate < 1000) return;
  lastLCDUpdate = millis();

  // Show IP for 90 seconds
  if (millis() - bootTime <= 90000) {
    lcd.setCursor(0, 0);
    lcd.print("IP Address:     ");

    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    lcd.print("      ");
  }
  else {
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

  // LCD INIT FIRST (SDA = GPIO33, SCL = GPIO32)
  Wire.begin(33, 32);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Energy Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);

  // SAVED THRESHOLDS
  prefs.begin("energy", false);
  ovThreshold = prefs.getInt("ov", DEFAULT_OV);
  uvThreshold = prefs.getInt("uv", DEFAULT_UV);

  // PZEM INIT
  Serial2.begin(9600, SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);
  delay(500);

  // WIFI CONNECT
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

  bootTime = millis();   // Start 90 sec timer

  // SERVER START
  server.on("/", handleRoot);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("Servers Started!");
}

void loop() {
  server.handleClient();
  webSocket.loop();

  // Send data and update LCD every 1 second
  if (millis() - lastUpdate >= 1000) {
    sendSensorData();
    updateLCD();
    lastUpdate = millis();
  }
}
