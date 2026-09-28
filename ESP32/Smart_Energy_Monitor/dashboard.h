// Generated from the Dashboard/ folder by tools/generate_dashboard_header.py. Do not edit by hand.
#pragma once
#include <pgmspace.h>

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 Energy Monitor Dashboard</title>
  <style>
* { margin: 0; padding: 0; box-sizing: border-box; }

:root {
  --bg: #060b1e;
  --panel-top: rgba(32, 44, 76, 0.62);
  --panel-bottom: rgba(16, 25, 50, 0.78);
  --border: rgba(120, 140, 190, 0.18);
  --text: #f1f5f9;
  --muted: #a9b6d0;
  --track: #232f4a;
}

html, body { min-height: 100%; }

body {
  font-family: 'Inter', 'Segoe UI', system-ui, -apple-system, sans-serif;
  color: var(--text);
  background:
    radial-gradient(900px 500px at 100% 0%, rgba(109, 40, 217, 0.38), transparent 60%),
    radial-gradient(800px 600px at 90% 100%, rgba(37, 99, 235, 0.14), transparent 60%),
    linear-gradient(160deg, #070d22 0%, #060b1e 60%, #050918 100%);
  background-attachment: fixed;
}

.app {
  display: flex;
  min-height: 100vh;
}

/* ---------- Sidebar ---------- */
.sidebar {
  width: 27%;
  min-width: 350px;
  max-width: 440px;
  padding: 22px 18px;
  display: flex;
  flex-direction: column;
  gap: 18px;
  background: rgba(7, 12, 30, 0.55);
  border-right: 1px solid var(--border);
}

.panel {
  background: linear-gradient(165deg, var(--panel-top), var(--panel-bottom));
  border: 1px solid var(--border);
  border-radius: 22px;
  padding: 22px 22px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.25);
}

.brand {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 14px 16px;
}

.brand-icon {
  width: 58px;
  height: 58px;
  flex: none;
  border-radius: 17px;
  display: grid;
  place-items: center;
  background: linear-gradient(145deg, #2563eb, #7c3aed);
  box-shadow: 0 6px 18px rgba(79, 70, 229, 0.45);
}

.brand > div:last-child { min-width: 0; }
.brand-title { font-size: 1.08rem; font-weight: 700; line-height: 1.2; }
.brand-sub { font-size: 0.84rem; line-height: 1.3; color: var(--muted); margin-top: 4px; }

.panel-title {
  display: flex;
  align-items: center;
  gap: 12px;
  font-size: 1.45rem;
  font-weight: 700;
  margin-bottom: 20px;
  color: #dbe4f5;
}

.status-badge {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 12px;
  height: 68px;
  border-radius: 14px;
  font-size: 1.7rem;
  font-weight: 800;
  letter-spacing: 0.3px;
  border: 1px solid;
  transition: background 0.3s, color 0.3s, border-color 0.3s;
}

.status-badge.normal  { color: #22f08a; background: linear-gradient(90deg, #0f4a33, #0d3a2b); border-color: #1f8a5c; }
.status-badge.over    { color: #ff6b6b; background: linear-gradient(90deg, #4d1a22, #3a141b); border-color: #a33a48; }
.status-badge.under   { color: #fbbf24; background: linear-gradient(90deg, #4a3712, #3a2c10); border-color: #a1761d; }
.status-badge.noread  { color: #94a3b8; background: linear-gradient(90deg, #1f2a44, #1a2339); border-color: #3b4a6b; }

.status-note {
  margin-top: 16px;
  font-size: 1.15rem;
  line-height: 1.4;
  color: var(--muted);
}

.field-label {
  display: block;
  font-size: 1.15rem;
  color: #d0daee;
  margin: 4px 0 10px;
}

.field-input {
  width: 100%;
  height: 58px;
  padding: 0 16px;
  margin-bottom: 20px;
  font-size: 1.4rem;
  font-family: inherit;
  color: var(--text);
  background: rgba(12, 19, 40, 0.8);
  border: 1px solid rgba(120, 140, 190, 0.28);
  border-radius: 12px;
  outline: none;
  color-scheme: dark;
}

.field-input:focus { border-color: #3b82f6; box-shadow: 0 0 0 3px rgba(59, 130, 246, 0.25); }
.field-input::-webkit-inner-spin-button,
.field-input::-webkit-outer-spin-button { opacity: 1; height: 34px; }

.save-btn {
  width: 100%;
  height: 68px;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 12px;
  margin-top: 4px;
  font-family: inherit;
  font-size: 1.35rem;
  font-weight: 700;
  color: #fff;
  background: linear-gradient(90deg, #2f80ff, #7c3aed);
  border: none;
  border-radius: 14px;
  cursor: pointer;
  box-shadow: 0 8px 22px rgba(79, 70, 229, 0.35);
  transition: filter 0.2s, transform 0.1s;
}

.save-btn:hover { filter: brightness(1.1); }
.save-btn:active { transform: scale(0.99); }
.save-btn:focus-visible { outline: 3px solid #93c5fd; outline-offset: 3px; }

.info-box {
  display: flex;
  align-items: center;
  gap: 18px;
  margin-top: 22px;
  padding: 18px 20px;
  border-radius: 14px;
  background: rgba(12, 19, 40, 0.6);
  border: 1px solid rgba(120, 140, 190, 0.2);
}

.info-rows { flex: 1; display: flex; flex-direction: column; gap: 10px; font-size: 1.2rem; color: #d0daee; }
.info-row { display: flex; align-items: baseline; gap: 14px; }
.info-row .op { width: 20px; text-align: center; color: var(--muted); }
.info-row b { color: #fff; font-weight: 700; }

/* ---------- Main ---------- */
.main {
  flex: 1;
  min-width: 0;
  padding: 34px 30px 40px;
}

.topbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  flex-wrap: wrap;
  margin-bottom: 26px;
}

.topbar h1 { font-size: clamp(1.7rem, 2.6vw, 2.5rem); font-weight: 800; letter-spacing: -0.3px; }

.conn {
  display: inline-flex;
  align-items: center;
  gap: 8px;
  padding: 8px 16px;
  border-radius: 20px;
  font-size: 0.95rem;
  color: #22c55e;
  background: rgba(34, 197, 94, 0.12);
  border: 1px solid rgba(34, 197, 94, 0.3);
}

.conn .dot { width: 9px; height: 9px; border-radius: 50%; background: currentColor; }
.conn.connecting { color: #fbbf24; background: rgba(251, 191, 36, 0.1); border-color: rgba(251, 191, 36, 0.3); }
.conn.offline { color: #f87171; background: rgba(248, 113, 113, 0.1); border-color: rgba(248, 113, 113, 0.3); }

.cards {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  grid-template-rows: auto auto;
  gap: 20px;
}

.card {
  background: linear-gradient(165deg, var(--panel-top), var(--panel-bottom));
  border: 1px solid var(--border);
  border-radius: 22px;
  padding: 20px 22px;
  box-shadow: 0 10px 30px rgba(0, 0, 0, 0.25);
}

.card-head { display: flex; align-items: center; gap: 14px; }
.card-name { font-size: 1.15rem; font-weight: 600; color: #e6edf9; }

.chip {
  width: 54px;
  height: 54px;
  border-radius: 16px;
  display: grid;
  place-items: center;
  flex: none;
}

.chip.blue   { color: #3b9bff; background: rgba(59, 130, 246, 0.14); }
.chip.green  { color: #22e6a0; background: rgba(16, 185, 129, 0.16); }
.chip.purple { color: #9d6bff; background: rgba(139, 92, 246, 0.18); }
.chip.orange { color: #fb923c; background: rgba(249, 115, 22, 0.16); }

/* gauges */
.gauge { position: relative; margin: -6px auto 0; max-width: 330px; }
.gauge-svg { display: block; width: 100%; height: auto; }

.g-track { fill: none; stroke: var(--track); stroke-width: 22; stroke-linecap: round; }
.g-fill {
  fill: none;
  stroke-width: 22;
  stroke-linecap: round;
  stroke-dasharray: 0 100;
  transition: stroke-dasharray 0.6s ease;
}

.gauge-value {
  position: absolute;
  left: 0;
  right: 0;
  top: 78%;
  transform: translateY(-50%);
  text-align: center;
  font-size: clamp(1.9rem, 2.6vw, 2.6rem);
  font-weight: 800;
  font-variant-numeric: tabular-nums;
}

.gauge-scale {
  display: flex;
  justify-content: space-between;
  align-items: baseline;
  max-width: 330px;
  margin: 2px auto 0;
  padding: 0 10px;
  font-size: 1.1rem;
  color: var(--muted);
}

.gauge-label { font-size: 1.15rem; color: #d0daee; }

/* value cards */
.value-card { display: flex; flex-direction: column; }

.big-value {
  margin: auto 0;
  padding-top: 8px;
  text-align: center;
  font-size: clamp(1.8rem, 2.7vw, 2.9rem);
  font-weight: 800;
  font-variant-numeric: tabular-nums;
}

.value-label { text-align: center; font-size: 1.15rem; color: #d0daee; padding-bottom: 6px; }

.power-card .big-value { margin-top: 40px; font-size: clamp(2.2rem, 3.4vw, 3.5rem); }
.power-card .value-label { margin-bottom: auto; }

/* charts */
.charts {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 20px;
  margin-top: 20px;
}

.chart-card { padding: 20px 22px; }
.chart-title { font-size: 1.25rem; font-weight: 700; margin-bottom: 14px; color: #e6edf9; }
.chart-wrap { position: relative; height: 260px; }
.chart-note { grid-column: 1 / -1; color: #fbbf24; font-size: 0.95rem; }

/* ---------- Responsive ---------- */
@media (max-width: 1100px) {
  .app { flex-direction: column; }
  .sidebar { width: 100%; max-width: none; min-width: 0; border-right: none; border-bottom: 1px solid var(--border); }
  .cards { grid-template-columns: repeat(2, 1fr); }
}

@media (max-width: 700px) {
  .main { padding: 24px 16px 32px; }
  .cards, .charts { grid-template-columns: 1fr; }
}

@media (prefers-reduced-motion: reduce) {
  .g-fill, .status-badge, .save-btn { transition: none; }
}

</style>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.9.1/chart.min.js"></script>
</head>
<body>
  <div class="app">

    <aside class="sidebar">
      <div class="panel brand">
        <div class="brand-icon">
          <svg viewBox="0 0 24 24" width="30" height="30"><path d="M13.5 2 5 13.5h5.5L9.5 22 19 9.5h-5.7z" fill="#fbbf24"/></svg>
        </div>
        <div>
          <div class="brand-title">Smart Energy Meter</div>
          <div class="brand-sub" id="brandSub">PZEM-004T + Voltage Monitoring</div>
        </div>
      </div>

      <div class="panel">
        <h2 class="panel-title">
          <svg viewBox="0 0 24 24" width="26" height="26" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3.2"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
          <span id="statusHeading">Voltage Status</span>
        </h2>
        <div class="status-badge normal" id="statusBadge">
          <svg viewBox="0 0 24 24" width="34" height="34"><path d="M12 2 4 5v6c0 5 3.4 9 8 11 4.6-2 8-6 8-11V5z" fill="currentColor"/><path d="m8.4 12.2 2.5 2.5 4.8-5" fill="none" stroke="#0b2a1c" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round"/></svg>
          <span id="statusText">NORMAL</span>
        </div>
        <p class="status-note" id="statusNote">Waiting for data...</p>
      </div>

      <div class="panel">
        <h2 class="panel-title">
          <svg viewBox="0 0 24 24" width="26" height="26" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="3.2"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
          Threshold Settings
        </h2>

        <label class="field-label" for="ovInput">Over Voltage (V)</label>
        <input class="field-input" type="number" id="ovInput" min="1" max="500" step="1" value="260" />

        <label class="field-label" for="uvInput">Under Voltage (V)</label>
        <input class="field-input" type="number" id="uvInput" min="1" max="500" step="1" value="180" />

        <button class="save-btn" id="saveBtn" type="button">
          <svg viewBox="0 0 24 24" width="24" height="24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
          <span id="saveBtnText">Save Settings</span>
        </button>

        <div class="info-box">
          <svg viewBox="0 0 24 24" width="30" height="30" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round"><circle cx="12" cy="12" r="10"/><line x1="12" y1="11" x2="12" y2="17"/><circle cx="12" cy="7.4" r="0.6" fill="#38bdf8"/></svg>
          <div class="info-rows">
            <div class="info-row"><span id="ovLabel">OV limit</span><span class="op">&gt;</span><b id="ovSaved">260</b></div>
            <div class="info-row"><span id="uvLabel">UV limit</span><span class="op">&lt;</span><b id="uvSaved">180</b></div>
          </div>
        </div>
      </div>
    </aside>

    <main class="main">
      <header class="topbar">
        <h1>ESP32 Energy Monitor Dashboard</h1>
        <div class="conn" id="conn"><span class="dot"></span><span id="wsStatus">Connecting...</span></div>
      </header>

      <section class="cards">
        <div class="card gauge-card">
          <div class="card-head">
            <span class="chip blue"><svg viewBox="0 0 24 24" width="26" height="26"><path d="M13.5 2 5 13.5h5.5L9.5 22 19 9.5h-5.7z" fill="currentColor"/></svg></span>
            <span class="card-name">Voltage</span>
          </div>
          <div class="gauge">
            <svg viewBox="0 0 300 170" class="gauge-svg">
              <defs>
                <linearGradient id="gradVoltage" x1="0" y1="1" x2="1" y2="0">
                  <stop offset="0%" stop-color="#2f6bff"/><stop offset="100%" stop-color="#3ea0ff"/>
                </linearGradient>
              </defs>
              <path class="g-track" d="M 35 150 A 115 115 0 0 1 265 150" pathLength="100"/>
              <path class="g-fill" id="gaugeVoltage" d="M 35 150 A 115 115 0 0 1 265 150" pathLength="100" stroke="url(#gradVoltage)"/>
            </svg>
            <div class="gauge-value"><span id="voltage">--</span> V</div>
          </div>
          <div class="gauge-scale"><span>0</span><span class="gauge-label">Voltage</span><span id="voltageMax">300</span></div>
        </div>

        <div class="card gauge-card">
          <div class="card-head">
            <span class="chip green"><svg viewBox="0 0 24 24" width="26" height="26" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><polyline points="2 12 6.5 12 9.5 4.5 14 19.5 17 12 22 12"/></svg></span>
            <span class="card-name">Current</span>
          </div>
          <div class="gauge">
            <svg viewBox="0 0 300 170" class="gauge-svg">
              <defs>
                <linearGradient id="gradCurrent" x1="0" y1="1" x2="1" y2="0">
                  <stop offset="0%" stop-color="#10d98c"/><stop offset="100%" stop-color="#34f0a8"/>
                </linearGradient>
              </defs>
              <path class="g-track" d="M 35 150 A 115 115 0 0 1 265 150" pathLength="100"/>
              <path class="g-fill" id="gaugeCurrent" d="M 35 150 A 115 115 0 0 1 265 150" pathLength="100" stroke="url(#gradCurrent)"/>
            </svg>
            <div class="gauge-value"><span id="current">--</span> A</div>
          </div>
          <div class="gauge-scale"><span>0</span><span class="gauge-label">Current</span><span id="currentMax">20</span></div>
        </div>

        <div class="card value-card power-card">
          <div class="card-head">
            <span class="chip purple"><svg viewBox="0 0 24 24" width="26" height="26"><rect x="3" y="12" width="4.5" height="9" rx="1.2" fill="currentColor"/><rect x="9.8" y="7" width="4.5" height="14" rx="1.2" fill="currentColor"/><rect x="16.5" y="2.5" width="4.5" height="18.5" rx="1.2" fill="currentColor"/></svg></span>
            <span class="card-name">Power</span>
          </div>
          <div class="big-value"><span id="power">--</span> W</div>
          <div class="value-label">Power</div>
        </div>

        <div class="card value-card">
          <div class="card-head">
            <span class="chip green"><svg viewBox="0 0 24 24" width="26" height="26" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round"><ellipse cx="12" cy="5" rx="8" ry="3" fill="currentColor"/><path d="M4 5v14c0 1.7 3.6 3 8 3s8-1.3 8-3V5"/><path d="M4 12c0 1.7 3.6 3 8 3s8-1.3 8-3"/></svg></span>
            <span class="card-name">Energy</span>
          </div>
          <div class="big-value"><span id="energy">--</span> kWh</div>
          <div class="value-label">Energy</div>
        </div>

        <div class="card value-card">
          <div class="card-head">
            <span class="chip orange"><svg viewBox="0 0 24 24" width="26" height="26" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><polyline points="2 12 6.5 12 9.5 4.5 14 19.5 17 12 22 12"/></svg></span>
            <span class="card-name">Frequency</span>
          </div>
          <div class="big-value"><span id="frequency">--</span> Hz</div>
          <div class="value-label">Frequency</div>
        </div>

        <div class="card value-card">
          <div class="card-head">
            <span class="chip purple"><svg viewBox="0 0 24 24" width="26" height="26" fill="none" stroke="currentColor" stroke-width="2.6" stroke-linecap="round"><ellipse cx="12" cy="12" rx="5" ry="6"/><line x1="12" y1="2.5" x2="12" y2="21.5"/></svg></span>
            <span class="card-name">Power Factor</span>
          </div>
          <div class="big-value"><span id="pf">--</span></div>
          <div class="value-label">Power Factor</div>
        </div>
      </section>

      <section class="charts">
        <div class="panel chart-card">
          <h3 class="chart-title">Power &amp; Current Trend</h3>
          <div class="chart-wrap"><canvas id="powerChart"></canvas></div>
        </div>
        <div class="panel chart-card">
          <h3 class="chart-title">Voltage &amp; Frequency Trend</h3>
          <div class="chart-wrap"><canvas id="voltageChart"></canvas></div>
        </div>
        <p class="chart-note" id="chartNote" hidden>Trend graphs need Chart.js. Connect this browser to the internet and reload.</p>
      </section>
    </main>

  </div>
  <script>
// ---------- Configuration (change here) ----------
const CONFIG = {
  maxDataPoints: 20,
  reconnectDelayMs: 3000,
  voltageMax: 300,          // voltage gauge full scale (V)
  currentMax: 20,           // current gauge full scale (A), match your CT rating
  relayProtectionEnabled: false,  // set to true after a relay is added and implemented in firmware
  demoMode: false           // true, or open index.html?demo, to preview with simulated data (no ESP32)
};

const $ = (id) => document.getElementById(id);

let ws;
let thresholdDirty = false;
let pendingSave = null;

// Chart data arrays
const timeLabels = [];
const powerData = [];
const currentData = [];
const voltageData = [];
const frequencyData = [];
let powerChart = null;
let voltageChart = null;

// ---------- Static labels ----------
$('voltageMax').textContent = CONFIG.voltageMax;
$('currentMax').textContent = CONFIG.currentMax;

if (CONFIG.relayProtectionEnabled) {
  $('brandSub').textContent = 'PZEM-004T + Voltage Protection';
  $('statusHeading').textContent = 'Protection Status';
  $('ovLabel').textContent = 'Send OV';
  $('uvLabel').textContent = 'Send UV';
}

const STATUS_VIEW = {
  'NORMAL': {
    cls: 'normal',
    text: 'NORMAL',
    note: CONFIG.relayProtectionEnabled
      ? 'System is running normally. Load protection is ready.'
      : 'Supply voltage is within the set limits.'
  },
  'OVER VOLTAGE': { cls: 'over', text: 'OVER VOLTAGE', note: 'Voltage is above the over voltage limit.' },
  'UNDER VOLTAGE': { cls: 'under', text: 'UNDER VOLTAGE', note: 'Voltage is below the under voltage limit.' },
  'NO READING': { cls: 'noread', text: 'NO READING', note: 'No valid voltage reading from the PZEM.' }
};

// ---------- Charts ----------
function chartOptions(leftTitle, leftColor, rightTitle, rightColor) {
  return {
    responsive: true,
    maintainAspectRatio: false,
    plugins: { legend: { labels: { color: '#e2e8f0' } } },
    scales: {
      x: {
        ticks: { color: '#94a3b8' },
        grid: { color: 'rgba(148, 163, 184, 0.1)' }
      },
      y: {
        ticks: { color: '#94a3b8' },
        grid: { color: 'rgba(148, 163, 184, 0.1)' },
        title: { display: true, text: leftTitle, color: leftColor }
      },
      y1: {
        position: 'right',
        ticks: { color: '#94a3b8' },
        grid: { display: false },
        title: { display: true, text: rightTitle, color: rightColor }
      }
    }
  };
}

function initCharts() {
  if (typeof Chart === 'undefined') {
    $('chartNote').hidden = false;
    return;
  }

  powerChart = new Chart($('powerChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: timeLabels,
      datasets: [{
        label: 'Power (W)',
        data: powerData,
        borderColor: '#9d6bff',
        backgroundColor: 'rgba(157, 107, 255, 0.12)',
        tension: 0.4,
        fill: true,
        borderWidth: 2
      }, {
        label: 'Current (A)',
        data: currentData,
        borderColor: '#22e6a0',
        backgroundColor: 'rgba(34, 230, 160, 0.10)',
        tension: 0.4,
        fill: true,
        borderWidth: 2,
        yAxisID: 'y1'
      }]
    },
    options: chartOptions('Power (W)', '#9d6bff', 'Current (A)', '#22e6a0')
  });

  voltageChart = new Chart($('voltageChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: timeLabels,
      datasets: [{
        label: 'Voltage (V)',
        data: voltageData,
        borderColor: '#3b9bff',
        backgroundColor: 'rgba(59, 155, 255, 0.12)',
        tension: 0.4,
        fill: true,
        borderWidth: 2
      }, {
        label: 'Frequency (Hz)',
        data: frequencyData,
        borderColor: '#fb923c',
        backgroundColor: 'rgba(251, 146, 60, 0.10)',
        tension: 0.4,
        fill: true,
        borderWidth: 2,
        yAxisID: 'y1'
      }]
    },
    options: chartOptions('Voltage (V)', '#3b9bff', 'Frequency (Hz)', '#fb923c')
  });
}

// ---------- WebSocket ----------
function setConnection(state, text) {
  $('conn').className = 'conn ' + state;
  $('wsStatus').textContent = text;
}

function connectWebSocket() {
  ws = new WebSocket('ws://' + window.location.hostname + ':81');

  ws.onopen = () => {
    setConnection('', 'Connected');
    console.log('WebSocket Connected');
  };

  ws.onmessage = (event) => {
    let data;
    try {
      data = JSON.parse(event.data);
    } catch (err) {
      console.error('Invalid JSON:', err);
      return;
    }
    handleData(data);
  };

  ws.onclose = () => {
    setConnection('offline', 'Disconnected');
    console.log('WebSocket Disconnected. Reconnecting...');
    setTimeout(connectWebSocket, CONFIG.reconnectDelayMs);
  };

  ws.onerror = (error) => {
    console.error('WebSocket Error:', error);
  };
}

function handleData(data) {
  updateDisplay(data);
  updateStatus(data);
  updateCharts(data);
}

// ---------- Display ----------
function setGauge(id, value, max) {
  const el = $(id);
  if (!(value > 0)) {
    el.style.strokeOpacity = 0;
    return;
  }
  el.style.strokeOpacity = 1;
  const pct = Math.max(1.5, Math.min(100, (value / max) * 100));
  el.style.strokeDasharray = pct + ' 100';
}

function updateDisplay(data) {
  $('voltage').textContent = data.voltage.toFixed(1);
  $('current').textContent = data.current.toFixed(2);
  $('power').textContent = data.power.toFixed(1);
  $('energy').textContent = data.energy.toFixed(3);
  $('frequency').textContent = data.frequency.toFixed(1);
  $('pf').textContent = parseFloat(data.pf.toFixed(2)).toString();

  setGauge('gaugeVoltage', data.voltage, CONFIG.voltageMax);
  setGauge('gaugeCurrent', data.current, CONFIG.currentMax);
}

function updateStatus(data) {
  const view = STATUS_VIEW[data.status] || STATUS_VIEW['NO READING'];
  $('statusBadge').className = 'status-badge ' + view.cls;
  $('statusText').textContent = view.text;
  $('statusNote').textContent = view.note;

  if (data.ov === undefined || data.uv === undefined) return;

  $('ovSaved').textContent = data.ov;
  $('uvSaved').textContent = data.uv;

  if (!thresholdDirty) {
    $('ovInput').value = data.ov;
    $('uvInput').value = data.uv;
  }

  if (pendingSave && data.ov === pendingSave.ov && data.uv === pendingSave.uv) {
    pendingSave = null;
    flashButton('Saved');
  }
}

function updateCharts(data) {
  const now = new Date();
  const timeStr = now.getHours() + ':' +
                  String(now.getMinutes()).padStart(2, '0') + ':' +
                  String(now.getSeconds()).padStart(2, '0');

  if (timeLabels.length >= CONFIG.maxDataPoints) {
    timeLabels.shift();
    powerData.shift();
    currentData.shift();
    voltageData.shift();
    frequencyData.shift();
  }

  timeLabels.push(timeStr);
  powerData.push(data.power);
  currentData.push(data.current);
  voltageData.push(data.voltage);
  frequencyData.push(data.frequency);

  if (powerChart && voltageChart) {
    powerChart.update('none');
    voltageChart.update('none');
  }
}

// ---------- Threshold settings ----------
let flashTimer = null;

function flashButton(text) {
  $('saveBtnText').textContent = text;
  clearTimeout(flashTimer);
  flashTimer = setTimeout(() => { $('saveBtnText').textContent = 'Save Settings'; }, 2000);
}

$('ovInput').addEventListener('input', () => { thresholdDirty = true; });
$('uvInput').addEventListener('input', () => { thresholdDirty = true; });

$('saveBtn').addEventListener('click', () => {
  const ov = parseInt($('ovInput').value, 10);
  const uv = parseInt($('uvInput').value, 10);

  if (!(uv > 0) || !(ov > uv) || ov > 500) {
    flashButton('Invalid: UV must be below OV');
    return;
  }
  if (!ws || ws.readyState !== WebSocket.OPEN) {
    flashButton('Not connected');
    return;
  }

  pendingSave = { ov, uv };
  ws.send(JSON.stringify({ ov: ov, uv: uv }));
  thresholdDirty = false;

  setTimeout(() => {
    if (pendingSave) {
      pendingSave = null;
      flashButton('Not saved');
    }
  }, 3000);
});

// ---------- Demo mode (simulated ESP32, for previewing without hardware) ----------
function startDemo() {
  let ov = 260;
  let uv = 180;
  let energy = 0.013;

  function tick() {
    const t = Date.now() / 1000;
    const voltage = 225 + 3 * Math.sin(t / 7) + (Math.random() - 0.5);
    const current = 0.7 + 0.05 * Math.sin(t / 5) + (Math.random() - 0.5) * 0.01;
    const pf = 0.98 + 0.02 * Math.sin(t / 11);
    const power = voltage * current * pf;
    const frequency = 49.9 + (Math.random() - 0.5) * 0.1;
    energy += power / 1000 / 3600;

    let status = 'NORMAL';
    if (voltage > ov) status = 'OVER VOLTAGE';
    else if (voltage < uv) status = 'UNDER VOLTAGE';

    handleData({ voltage, current, power, energy, frequency, pf, status, ov, uv });
  }

  ws = {
    readyState: WebSocket.OPEN,
    send: (msg) => {
      const m = JSON.parse(msg);
      ov = m.ov;
      uv = m.uv;
      tick();
    }
  };

  setConnection('', 'Demo mode');
  tick();
  setInterval(tick, 1000);
}

// ---------- Start ----------
initCharts();
if (CONFIG.demoMode || new URLSearchParams(window.location.search).has('demo')) {
  startDemo();
} else {
  setConnection('connecting', 'Connecting...');
  connectWebSocket();
}

</script>
</body>
</html>
)rawliteral";
