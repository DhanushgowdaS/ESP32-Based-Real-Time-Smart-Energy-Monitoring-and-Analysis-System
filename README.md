# ESP32-Based Real-Time Smart Energy Monitoring and Analysis System

A real-time electrical energy monitoring system built on the ESP32. The PZEM-004T V3.0 measures the AC load, the ESP32 reads it over UART, shows it on a 16x2 I2C LCD, and serves a web dashboard that updates live over WebSocket.

## Features

- Measures voltage, current, power, energy, frequency and power factor
- Local display on a 16x2 I2C LCD (shows the IP address for the first 90 seconds)
- Web dashboard hosted by the ESP32 (no cloud server or database)
- Real-time updates over WebSocket, once per second
- Voltage and current gauges, plus Power and Current and Voltage and Frequency trend graphs (Chart.js)
- Over-voltage / under-voltage thresholds set from the dashboard and stored in ESP32 flash
- Voltage status indicator (NORMAL, OVER VOLTAGE, UNDER VOLTAGE, NO READING)
- Invalid (NaN) readings from the PZEM are handled and shown as 0

## System Architecture

```
AC Load
   |
PZEM-004T + CT
   | UART
ESP32
   |-- 16x2 I2C LCD
   |
   +-- Wi-Fi
         |
     ESP32 Web Server (port 80)  -> serves the dashboard files from LittleFS
         |
     WebSocket (port 81)
         |
     Web Dashboard (browser)
```

## Hardware

- ESP32 development board
- PZEM-004T V3.0 energy monitoring module with CT
- 16x2 I2C LCD (address 0x27)
- Regulated power supply, connecting wires, terminals, enclosure and electrical protection for the final assembly

## Pin Connections (as defined in the firmware)

| Function | ESP32 pin |
|----------|-----------|
| PZEM UART RX (ESP32 receives) | GPIO23 |
| PZEM UART TX (ESP32 transmits) | GPIO22 |
| LCD SDA | GPIO33 |
| LCD SCL | GPIO32 |

See [docs/wiring.md](docs/wiring.md) for details and safety notes.

**Safety:** this project involves AC mains voltage. Do not connect any mains wiring unless you are qualified and the arrangement has been checked. Install the CT on the phase wire only.

## Software and Libraries

- Arduino IDE with the ESP32 Arduino core
- WebSockets (Markus Sattler)
- PZEM004Tv30
- LiquidCrystal I2C
- Built in with the ESP32 core: WiFi, WebServer, Wire, LittleFS, Preferences
- Dashboard: HTML, CSS, JavaScript, Chart.js 3.9.1 (loaded from a CDN)

## Repository Structure

```
esp32-smart-energy-monitor/
├── README.md
├── ESP32/
│   └── Smart_Energy_Monitor/
│       └── Smart_Energy_Monitor.ino
├── Dashboard/
│   ├── index.html
│   ├── style.css
│   └── script.js
└── docs/
    ├── wiring.md
    └── circuit-diagram.png   (to be added)
```

The sketch sits in a folder with the same name as the `.ino` file, as the Arduino IDE requires.

## Setup

1. Install the libraries listed above.
2. Open `Smart_Energy_Monitor.ino` and set your Wi-Fi details:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
   Never commit real credentials.
3. Copy `index.html`, `style.css` and `script.js` from `Dashboard/` into a `data` folder next to the sketch:
   `ESP32/Smart_Energy_Monitor/data/`
4. Upload the `data` folder to the ESP32 with a LittleFS upload tool (for example the arduino-littlefs-upload plugin for Arduino IDE 2.x).
5. Upload the sketch.
6. Open the Serial Monitor (115200 baud) or read the IP address from the LCD.
7. Open `http://<ESP32_IP>` in a browser on the same network. The trend graphs need the browser to reach the internet, because Chart.js is loaded from a CDN.

## Data Interface

The ESP32 sends this JSON over WebSocket (port 81) every second:

```json
{
  "voltage": 0.00,
  "current": 0.000,
  "power": 0.00,
  "energy": 0.000,
  "frequency": 0.00,
  "pf": 0.00,
  "status": "NORMAL",
  "ov": 260,
  "uv": 180
}
```

The dashboard sends the thresholds back when Save Settings is pressed:

```json
{"ov":260,"uv":180}
```

## Dashboard Configuration

Gauge limits and behaviour are set at the top of `Dashboard/script.js`:

```js
const CONFIG = {
  maxDataPoints: 20,
  reconnectDelayMs: 3000,
  voltageMax: 300,
  currentMax: 20,
  relayProtectionEnabled: false
};
```

## Current Scope

Implemented: ESP32, PZEM-004T, CT, 16x2 LCD, Wi-Fi, ESP32 WebServer, WebSocket, web dashboard, Chart.js graphs, real-time parameter display, voltage threshold settings and status indicator.

The threshold status is an on-screen indicator only. No relay is connected or controlled by the current firmware.

## Future Scope

- Relay-based load control (over/under voltage trip)
- Automated alerts
- MQTT
- Cloud database and long-term data storage
- Mobile application
- Energy bill estimation
- OTA firmware updates


