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
