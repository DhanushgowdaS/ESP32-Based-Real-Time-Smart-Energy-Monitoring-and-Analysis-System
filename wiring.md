# Wiring

Pin assignments below are taken from the firmware. Verify them against your physical build before powering the circuit.

## ESP32 to PZEM-004T (UART, low-voltage side)

| ESP32 | PZEM-004T |
|-------|-----------|
| GPIO23 (UART RX) | TX |
| GPIO22 (UART TX) | RX |
| 5V | 5V |
| GND | GND |

## ESP32 to 16x2 I2C LCD

| ESP32 | LCD |
|-------|-----|
| GPIO33 (SDA) | SDA |
| GPIO32 (SCL) | SCL |
| 5V | VCC |
| GND | GND |

The firmware calls `Wire.begin(33, 32)`, which sets SDA to GPIO33 and SCL to GPIO32. LCD I2C address: 0x27.

## Mains side

The AC load connection, CT placement and terminal wiring are to be documented after the final circuit is verified. The CT must be installed on the phase wire only. Do not work on mains wiring unless qualified, and do not assume an unverified arrangement is safe.

## Circuit diagram

`circuit-diagram.png` to be added.
