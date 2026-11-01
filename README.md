#  Real-Time Vaccine Storage Temperature Control System

An embedded temperature control system that maintains stable vaccine storage conditions between **10–15°C** using a Peltier cooling module controlled by an Arduino Uno.

---

##  Overview

Vaccines require precise temperature control to remain effective. This project implements a real-time feedback control system using a DS18B20 temperature sensor and an ON/OFF control algorithm to switch a Peltier cooling module via MOSFET, maintaining temperature within the safe storage range of 10–15°C. A 16x2 I2C LCD displays live temperature and system status.

> **Note on temperature range**: Standard vaccine cold-chain storage is 2-8°C. This prototype targets 10-15°C because reaching and stabilizing at 2-8°C with a single TEC1-12706 and limited insulation took too long within the project’s time and hardware constraints. The control logic is range-independent, so the thresholds can be changed to 2-8°C with better insulation, a larger heat sink, or a more capable cooling stage.

---

##  Components Used

| Component | Description |
|-----------|-------------|
| Arduino Uno | Microcontroller for control logic |
| DS18B20 | 1-Wire digital temperature sensor |
| Peltier Module (TEC1-12706) | Thermoelectric cooler |
| Cooling Fan | Attached to Peltier hot side for heat dissipation |
| IRLZ44N MOSFET | Switching element for Peltier control |
| SMPS (12V) | Powers the Peltier module and fan |
| LCD 16x2 I2C (0x27) | Real-time display of temperature and status |
| 1N4007 Diode | Flyback protection across fan |
| 4.7kΩ Resistor | Pull-up for DS18B20 data line |
| 100–220Ω Resistor | Gate resistor for MOSFET |
| 10kΩ Resistor | Gate pull-down resistor for MOSFET |
| Breadboard + Jumper Wires | Circuit assembly |

---

##  Circuit Connections

###  SMPS (Power Supply)
- **AC Input:** L → Live, N → Neutral
- **DC Output:** V+ → +12V rail, V- → GND rail

###  Peltier Module
- Red wire → SMPS V+
- Black wire → MOSFET Drain

###  MOSFET (IRLZ44N)
- **Drain** → Peltier black wire
- **Source** → SMPS V- (GND)
- **Gate** → Arduino D9 via 100–220Ω resistor
- 10kΩ resistor from Gate to GND

###  Fan
- Fan+ → SMPS V+
- Fan- → SMPS V-
- 1N4007 diode across fan (stripe side to Fan+)

###  DS18B20 Temperature Sensor
- VCC → Arduino 5V
- GND → Arduino GND
- DATA → Arduino D2
- 4.7kΩ pull-up resistor between DATA and 5V

###  LCD (16x2 I2C)
- VCC → Arduino 5V
- GND → Arduino GND
- SDA → Arduino A4
- SCL → Arduino A5

###  Common Ground (Important!)
Connect together: SMPS V-, Arduino GND, MOSFET Source, DS18B20 GND, LCD GND

---

##  How It Works

The system implements an **ON/OFF control algorithm**:

```
If temperature > 15°C → Turn ON Peltier (start cooling)
If temperature < 10°C → Turn OFF Peltier (stop cooling)
```

This hysteresis band (10–15°C) prevents rapid switching and maintains a stable storage window for vaccines.

---

##  Control Logic

```c
sensors.requestTemperatures();
float tempC = sensors.getTempCByIndex(0);

if (tempC > 15) {
    peltierState = true;   // Turn ON cooling
} else if (tempC < 10) {
    peltierState = false;  // Turn OFF cooling
}

analogWrite(PELTIER_PIN, peltierState ? 255 : 0);
```

---

##  Results

- Successfully maintained temperature between **10°C and 15°C**
- Real-time temperature and status displayed on LCD
- Serial monitor logs temperature and Peltier state continuously
- Stable switching with hysteresis — no rapid ON/OFF cycling

---

##  Tools & Technologies

- **Arduino IDE** — Firmware development
- **C/C++** — Programming language
- **OneWire + DallasTemperature Library** — DS18B20 sensor interfacing
- **LiquidCrystal_I2C Library** — LCD display control
- **Breadboard Prototyping** — Circuit assembly

---

##  Future Improvements

- Implement **PID control** for smoother temperature regulation
- Add **data logging** to SD card or PC via serial + Python
- Design a **custom PCB** to replace breadboard prototype
- Add **wireless monitoring** via ESP8266/ESP32
---

##  Repository Structure

```
vaccine-temp-control/
│
├── src/
│   └── vaccine_temp_control.ino    # Main Arduino sketch
├── circuit/
│   └── connections.md              # Full wiring details
└── README.md
```

