# AI-Based Smart Terrace Irrigation System

**IoT-enabled automated irrigation with real-time environmental monitoring and a machine learning extension.**

![ESP32](https://img.shields.io/badge/ESP32-IoT%20Controller-red?logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-Embedded%20C%2FC%2B%2B-00979D?logo=arduino)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Embedded%20Development-orange?logo=platformio)
![Python](https://img.shields.io/badge/Python-Data%20Processing-blue?logo=python)
![Machine Learning](https://img.shields.io/badge/ML-Random%20Forest-green)
![IoT](https://img.shields.io/badge/IoT-Wi--Fi-lightgrey)
![DHT11](https://img.shields.io/badge/Sensor-DHT11-yellow)
![Soil Moisture](https://img.shields.io/badge/Sensor-Soil%20Moisture-brown)

This project combines embedded systems, IoT, sensor data acquisition, automation, and machine learning to make terrace irrigation respond to measured environmental conditions rather than a fixed schedule or manual observation.

---

## Table of Contents

1. [Overview](#overview)
2. [Problem Statement](#problem-statement)
3. [Design Approach](#design-approach)
4. [Core Capabilities](#core-capabilities)
5. [System Architecture](#system-architecture)
6. [Hardware Components](#hardware-components)
7. [ESP32 Pin Configuration](#esp32-pin-configuration)
8. [Rule-Based Control Algorithm](#rule-based-control-algorithm)
9. [Mathematical Model](#mathematical-model)
10. [Machine Learning Extension](#machine-learning-extension)
11. [IoT Connectivity](#iot-connectivity)
12. [Historical Data and Data Pipeline](#historical-data-and-data-pipeline)
13. [Dashboard](#dashboard)
14. [Safety and Reliability](#safety-and-reliability)
15. [Technology Stack](#technology-stack)
16. [Implementation Workflow](#implementation-workflow)
17. [Experimental Validation](#experimental-validation)
18. [Performance Evaluation](#performance-evaluation)
19. [Example Serial Monitor Output](#example-serial-monitor-output)
20. [Repository Structure](#repository-structure)
21. [Installation](#installation)
22. [Project Status](#project-status)
23. [Limitations](#limitations)
24. [Future Scope](#future-scope)
25. [Author](#author)
26. [License](#license)

---

## Overview

Terrace irrigation is commonly performed manually or on a fixed schedule. Neither approach observes the actual condition of the soil.

This project implements a **Smart Terrace Irrigation System** using an **ESP32 microcontroller**, **soil-moisture sensing**, **DHT11 temperature and humidity monitoring**, and **relay-controlled pumping**.

**Current embedded system (implemented):**

```text
Soil + Environment
       ↓
     Sensors
       ↓
      ESP32
       ↓
Dry/Wet Decision
       ↓
     Relay
       ↓
   Water Pump
       ↓
    Irrigation
```

**Proposed extended architecture:**

```text
Sensors
   ↓
ESP32
   ↓
Wi-Fi
   ↓
IoT Backend
   ├── Database
   ├── Dashboard
   └── Historical Dataset
             ↓
       Machine Learning
             ↓
    Irrigation Prediction
             ↓
       Control Policy
             ↓
           Pump
```

The prototype is deliberately kept simple so that it can be validated at the hardware level first. The IoT and ML layers are built on top of that foundation.

---

## Problem Statement

Manual and fixed-schedule irrigation can result in:

- Watering soil that is already sufficiently wet.
- Delayed watering when soil becomes dry.
- Unnecessary pump operation.
- Dependence on human observation.
- No continuous environmental monitoring.
- No historical data for analysis.
- No predictive capability.
- No quantitative basis for verifying water savings.

This project addresses these issues by building a system that can:

1. Measure soil moisture.
2. Measure temperature.
3. Measure humidity.
4. Classify the soil as dry or wet.
5. Automatically control a water pump.
6. Display real-time measurements.
7. Provide an IoT-ready communication architecture.
8. Collect structured historical sensor data.
9. Support machine-learning-based irrigation prediction.

---

## Design Approach

The project follows a progression from simple automation to data-driven decision-making:

```text
Manual Watering
      ↓
Fixed Schedule
      ↓
Sensor-Based Automation
      ↓
IoT Monitoring
      ↓
Historical Data Collection
      ↓
Predictive Irrigation
```

**Key principle:** use measured environmental conditions, rather than a fixed timetable, to make irrigation decisions.

---

## Core Capabilities

### Soil Moisture Monitoring

The soil-moisture probe measures the electrical characteristics of the soil. Its analog output is connected to **ESP32 GPIO34** (ADC input). The firmware reads the raw value and uses it to decide whether irrigation is required.

```text
Moisture Value: 4095
Soil Condition: DRY
Motor Condition: ON
```

> The raw ADC value is sensor-specific and must be calibrated before it is interpreted as a physical moisture percentage.

### Temperature Monitoring

A **DHT11** sensor measures ambient temperature (e.g. `29.0 °C`). It is currently reported as environmental information and is a candidate input feature for the ML model.

### Humidity Monitoring

The DHT11 also measures relative humidity (e.g. `64.0 %`). It provides additional context that may help a predictive model characterize drying conditions.

### Automatic Pump Control

The ESP32 drives a relay through **GPIO26**. The relay is the switching interface between the low-power controller and the pump's external power circuit.

```text
ESP32 GPIO26
     ↓
Relay
     ↓
External Pump Power
     ↓
Water Pump
```

> The ESP32 GPIO must not be used as the pump's power source.

---

## System Architecture

```text
┌─────────────────────────────────────────────┐
│              APPLICATION LAYER              │
│ Dashboard / Monitoring / Alerts             │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│                 AI LAYER                    │
│ Random Forest / Prediction / Analytics      │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│              IoT / DATA LAYER               │
│ API / MQTT / Database / Historical Data     │
└──────────────────────┬──────────────────────┘
                       │
┌──────────────────────▼──────────────────────┐
│             ESP32 PROCESSING                │
│ ADC / GPIO / Sensor Reading / Control       │
└───────────────┬─────────────────┬───────────┘
                │                 │
        ┌───────▼──────┐    ┌─────▼──────┐
        │   Sensors    │    │   Relay    │
        │              │    │            │
        │ Soil Moisture│    │ Pump Switch│
        │ DHT11        │    └─────┬──────┘
        └──────────────┘          │
                                  ▼
                              Water Pump
                                  │
                                  ▼
                                Plant
```

**End-to-end data and control flow:**

```text
                  SMART TERRACE
                  IRRIGATION SYSTEM
                         │
                         ▼
                ┌─────────────────┐
                │     Sensors     │
                │ Soil Moisture   │
                │ Temperature     │
                │ Humidity        │
                └────────┬────────┘
                         │
                         ▼
                ┌─────────────────┐
                │      ESP32      │
                │ ADC / GPIO      │
                │ Processing      │
                │ Wi-Fi           │
                └───────┬─────────┘
                        │
           ┌────────────┼─────────────┐
           ▼            ▼             ▼
       Local Rule     Wi-Fi        Serial
        Control        │          Monitor
           │           ▼
           │       IoT Backend
           │           │
           │       ┌───┴────┐
           │       ▼        ▼
           │   Database  Dashboard
           │       │
           │       ▼
           │ Historical Data
           │       │
           │       ▼
           │ Machine Learning
           │       │
           │       ▼
           └── Prediction
                   │
                   ▼
                 Relay → Pump → Irrigation
```

---

## Hardware Components

| Component              | Role                                   |
| ---------------------- | -------------------------------------- |
| **ESP32 DevKit**       | Main controller and Wi-Fi device       |
| **Soil Moisture Sensor** | Measures soil condition              |
| **DHT11**              | Temperature and humidity sensing       |
| **1-Channel Relay**    | Pump switching                         |
| **DC Water Pump**      | Irrigation actuator                    |
| **Breadboard**         | Prototype circuit assembly             |
| **Jumper Wires**       | Electrical connections                 |
| **USB Cable**          | Programming and serial communication   |
| **External Pump Supply** | Supplies appropriate pump power      |
| **Water Container**    | Irrigation water source                |
| **Tubing**             | Delivers water to the plant            |

---

## ESP32 Pin Configuration

| Component        | ESP32 Pin | Function                    |
| ---------------- | --------- | --------------------------- |
| DHT11 DATA       | GPIO4     | Temperature/humidity data   |
| Soil Sensor AO   | GPIO34    | Analog moisture input       |
| Relay IN         | GPIO26    | Pump control                |
| DHT11 VCC        | 3V3       | Sensor supply               |
| DHT11 GND        | GND       | Ground                      |
| Soil Sensor GND  | GND       | Ground                      |
| Relay GND        | GND       | Ground                      |

> Verify the relay's power requirements and the pump's ratings against the physical modules before final deployment.

---

## Rule-Based Control Algorithm

The currently implemented controller is deterministic and threshold-based.

```text
Read Soil Moisture
       ↓
Compare With Threshold
       ↓
 ┌─────┴─────┐
 ▼           ▼
DRY          WET
 │            │
 ▼            ▼
Relay ON    Relay OFF
 │            │
 ▼            ▼
Pump ON     Pump OFF
```

```cpp
if (soilMoisture > dryThreshold) {
    soilCondition = "DRY";
    motor = "ON";
} else {
    soilCondition = "WET";
    motor = "OFF";
}
```

The threshold must be calibrated against the actual sensor and soil.

---

## Mathematical Model

Let `A` be the raw ADC soil sensor reading. After calibration with dry and wet reference measurements, the normalized moisture is:

```text
M = 100 × (A - A_dry) / (A_wet - A_dry)
```

Constrained to a valid range:

```text
M_clipped = min(100, max(0, M))
```

Threshold controller, where `T` is experimentally calibrated:

```text
IF M < T
    Irrigation Required = TRUE
ELSE
    Irrigation Required = FALSE
```

For the ML layer, the feature vector is:

```text
X = [Soil Moisture, Temperature, Humidity, Time,
     Moisture Trend, Pump Runtime, Water Availability]
```

The model estimates `P(Irrigation Required | X)`. The final control policy can combine this prediction with hard safety rules.

---

## Machine Learning Extension

The proposed intelligent layer uses historical sensor data to predict whether irrigation is required.

**Candidate features:**

| Feature            | Description                           |
| ------------------ | ------------------------------------- |
| Soil Moisture      | Current soil condition                |
| Temperature        | Ambient temperature                   |
| Humidity           | Relative humidity                     |
| Time of Day        | Daily environmental pattern           |
| Moisture Trend     | Recent drying/wetting rate            |
| Pump Runtime       | Recent irrigation history             |
| Water Availability | Whether irrigation water is available |
| Rainfall           | Future weather-related input          |

**Initial model:** Random Forest Classifier.

```text
Sensor Features
      ↓
Random Forest
      ↓
Probability / Class
      ↓
Irrigation Required?
   ┌──────┴──────┐
  YES            NO
   ↓              ↓
Pump ON        Pump OFF
```

> The AI layer is intentionally separate from the embedded controller. The demonstrated control is rule-based. Model accuracy should be reported only after collecting project-specific data and evaluating on held-out test data.

---

## IoT Connectivity

The ESP32's built-in Wi-Fi allows communication with a backend service.

```text
ESP32
  │ Wi-Fi
  ▼
Backend / API
  ├── Database
  ├── Dashboard
  └── ML Service
```

**Candidate technologies:** HTTP/REST, MQTT, JSON, Wi-Fi.

This layer allows the system to evolve from a local prototype into a remotely monitored one.

---

## Historical Data and Data Pipeline

Each sensor observation can be stored for later analysis.

```json
{
  "timestamp": "2026-10-03T17:00:00",
  "soil_adc": 4095,
  "temperature_c": 29.0,
  "humidity_pct": 64.0,
  "pump_state": 1,
  "irrigation_required": 1
}
```

Stored data supports sensor calibration, trend analysis, visualization, irrigation analysis, model training and evaluation, anomaly detection, and water-consumption analysis.

**Pipeline:**

```text
Sensor Data
     ↓
Data Collection
     ↓
Data Storage
     ↓
Data Cleaning
     ↓
Data Preprocessing
     ↓
Feature Engineering
     ↓
Train / Test Split
     ↓
Model Training
     ↓
Model Evaluation
     ↓
Prediction
     ↓
Irrigation Decision
```

---

## Dashboard

A planned dashboard can display soil moisture, temperature, humidity, soil condition, motor status, irrigation status, water availability, AI prediction, historical trends, pump runtime, and water consumption.

```text
┌──────────────────────────────────────────┐
│       SMART TERRACE IRRIGATION           │
├──────────────┬──────────────┬────────────┤
│ Soil Moist.  │ Temperature  │ Humidity   │
│    42 %      │    29 °C     │    64 %    │
├──────────────┴──────────────┴────────────┤
│ Soil Status: WET                         │
│ Motor: OFF                               │
│ AI Decision: NO IRRIGATION               │
├──────────────────────────────────────────┤
│          Historical Sensor Data          │
└──────────────────────────────────────────┘
```

---

## Safety and Reliability

The pump is switched through a relay rather than driven directly from an ESP32 GPIO.

```text
ESP32 GPIO → Relay Control → Separate Pump Power Path → Water Pump
```

Design considerations:

- Use the pump's rated external supply.
- Never power the pump directly from an ESP32 GPIO.
- Maintain a common ground on the low-voltage control side.
- Initialize the relay to a safe OFF state.
- Verify relay logic polarity (active-high vs. active-low).
- Keep water away from electronics.
- Add water-level sensing in a future version.
- Use hysteresis to reduce rapid ON/OFF switching.
- Retain local control if Wi-Fi fails.

---

## Technology Stack

**Embedded development:** C/C++, Arduino framework, Arduino IDE, PlatformIO, ESP32, Serial Monitor

**Sensors and hardware:** DHT11, analog ADC, soil moisture sensor, GPIO, relay, DC pump, breadboard prototyping

**IoT:** ESP32 Wi-Fi, HTTP/REST, MQTT, JSON, IoT backend

**Data science:** Python, pandas, NumPy, Matplotlib, Jupyter

**Machine learning:** scikit-learn, Random Forest, classification, model validation, confusion matrix, precision, recall, F1-score

**Development tools:** Visual Studio Code, PlatformIO, Arduino IDE, Git, GitHub

---

## Implementation Workflow

**Phase 1 — Hardware**

```text
ESP32 → Connect Soil Sensor → Connect DHT11 → Connect Relay → Connect Pump → Verify Power
```

**Phase 2 — Sensor Validation**

```text
Soil Sensor → ADC Reading → Dry/Wet Testing
DHT11       → Temperature + Humidity Testing
```

**Phase 3 — Automation**

```text
Sensor Reading → Threshold Comparison → Dry/Wet Decision → Relay → Pump
```

**Phase 4 — IoT**

```text
ESP32 → Wi-Fi → API / MQTT → Backend → Database → Dashboard
```

**Phase 5 — AI**

```text
Historical Data → Cleaning → Feature Engineering → Training → Validation → Prediction → Irrigation Decision
```

---

## Experimental Validation

| Test | Objective | Procedure / Record |
| ---- | --------- | ------------------ |
| 1. ESP32 | Verify board detection, USB connection, firmware upload, serial communication | — |
| 2. Soil sensor | Characterize response | Test dry, moderately wet, and wet soil; record timestamp, ADC value, soil condition |
| 3. DHT11 | Verify readings | Record temperature, humidity, timestamp |
| 4. Relay | Verify switching **without the pump connected** | GPIO command → relay state |
| 5. Pump | Verify actuation | Relay ON → pump ON → water flow |
| 6. Full system | End-to-end validation | See sequence below |

**Full-system sequence:**

```text
Dry Soil
   ↓
ESP32 Detects Dry Condition
   ↓
Relay ON → Pump ON → Water Delivered
   ↓
Soil Becomes Wet
   ↓
Relay OFF → Pump OFF
```

---

## Performance Evaluation

Evaluation is organized at three levels.

**Hardware metrics:** sensor response time, reading stability, relay response, pump response, system latency.

**Irrigation metrics:** correct dry detection, correct wet detection, false irrigation events, missed irrigation events, pump runtime, water consumption.

**Machine learning metrics:**

- Classification: accuracy, precision, recall, F1-score, confusion matrix, ROC-AUC where appropriate.
- Future soil-moisture forecasting: MAE, RMSE, R².

```text
Accuracy  = (TP + TN) / (TP + TN + FP + FN)
Precision = TP / (TP + FP)
Recall    = TP / (TP + FN)
F1        = 2 × Precision × Recall / (Precision + Recall)
```

---

## Example Serial Monitor Output

```text
SMART TERRACE IRRIGATION SYSTEM

Soil Moisture Value: 4095
Temperature: 29.0 C
Humidity: 64.0 %
Soil Condition: DRY
Motor Condition: ON
----------------------------

Soil Moisture Value: 1800
Temperature: 29.0 C
Humidity: 65.0 %
Soil Condition: WET
Motor Condition: OFF
----------------------------
```

Actual readings depend on the physical environment and calibration.

---

## Repository Structure

```text
AI-Smart-Terrace-Irrigation/
│
├── firmware/
│   ├── src/
│   │   └── main.cpp
│   └── platformio.ini
│
├── hardware/
│   ├── wiring_diagram.png
│   ├── circuit_diagram.png
│   └── component_list.md
│
├── data/
│   ├── raw/
│   └── processed/
│
├── ml/
│   ├── notebooks/
│   ├── models/
│   └── training.py
│
├── dashboard/
│   └── README.md
│
├── images/
│   ├── prototype.jpg
│   ├── architecture.png
│   ├── wiring.png
│   └── dashboard.png
│
├── docs/
│   └── project_report.pdf
│
├── README.md
├── requirements.txt
└── LICENSE
```

---

## Installation

### ESP32 Firmware

**Arduino IDE**

1. Install Arduino IDE.
2. Install ESP32 board support.
3. Select the appropriate ESP32 board.
4. Select the correct COM port.
5. Install the DHT sensor library.
6. Open the firmware.
7. Upload the program.
8. Open Serial Monitor at `115200` baud.

**PlatformIO**

Install Visual Studio Code and the PlatformIO IDE extension, then create an ESP32 Arduino project and place the firmware in `src/main.cpp`.

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

lib_deps =
    adafruit/DHT sensor library
    adafruit/Adafruit Unified Sensor
```

### Python Environment (Data and ML Layer)

```bash
pip install pandas numpy scikit-learn matplotlib jupyter
```

Suggested `requirements.txt`:

```text
pandas
numpy
scikit-learn
matplotlib
jupyter
requests
```

---

## Project Status

| Module                        | Status                          |
| ----------------------------- | ------------------------------- |
| ESP32 setup                   | ✅ Working                      |
| Firmware upload               | ✅ Working                      |
| Serial monitoring             | ✅ Working                      |
| Soil moisture sensing         | ✅ Prototype working            |
| Dry/Wet classification        | ✅ Implemented                  |
| Relay control logic           | ✅ Implemented                  |
| Temperature sensing           | ✅ Implemented                  |
| Humidity sensing              | ✅ Implemented                  |
| Pump automation               | ✅ Prototype architecture       |
| Wi-Fi communication           | 🔄 Extension                    |
| Database                      | 🔄 Extension                    |
| Dashboard                     | 🔄 Extension                    |
| AI dataset                    | 🔄 Data collection required     |
| Random Forest model           | 🔄 Proposed / development stage |
| Water consumption measurement | 🔄 Future enhancement           |
| Mobile application            | 🔮 Future scope                 |

---

## Limitations

- The soil sensor requires calibration for meaningful moisture interpretation.
- Raw ADC values are not universal moisture percentages.
- The DHT11 has limited precision compared with higher-end sensors.
- The current embedded decision logic is rule-based.
- AI performance cannot be claimed without actual model training and validation.
- Water-saving percentages require measured water volumes.
- The prototype is intended for small-scale academic demonstration.
- Remote monitoring requires a backend implementation.
- Water-level sensing is not part of the minimum current hardware.
- Long-term outdoor deployment requires a proper enclosure and power protection.

---

## Future Scope

1. **Multi-zone irrigation** — multiple sensors and valves independently controlling terrace sections (`Zone N → Sensor → Valve`).
2. **Weather-aware irrigation** — combine soil moisture, temperature, humidity, and rain forecast.
3. **Water-level monitoring** — a reservoir sensor to prevent dry-running the pump.
4. **Flow-based water measurement** — a flow sensor to quantify delivered volume and enable water-saving analysis.
5. **Plant-specific models** — per-plant moisture targets and irrigation policies.
6. **Edge AI** — run a lightweight model locally instead of depending on a cloud server.
7. **Mobile application** — live monitoring, alerts, historical graphs, and system status.
8. **Solar-powered operation** — `Solar Panel → Charge Controller → Battery → ESP32 + Pump`.
9. **Predictive soil-moisture forecasting** — Random Forest, Gradient Boosting, XGBoost, LSTM, or other time-series models.
10. **Fully autonomous irrigation** — move from "Is the soil dry?" to "When should irrigation happen, how much water is required, and how long should the pump operate?"

---

## Author

**Srimayi Golajapu**
AI / Data Science Engineering Student

Interests: Artificial Intelligence, Machine Learning, Data Science, IoT, Embedded Systems, Data Analytics

---

## License

This project is intended for academic, educational, research, and portfolio purposes.
