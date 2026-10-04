/*
  ESP32 + DHT11 + Soil Moisture Sensor + 1-Channel Relay

  Motor ON  -> Soil is DRY
  Motor OFF -> Soil is WET

  Wiring:

    DHT11
      VCC  -> 3V3
      GND  -> GND
      DATA -> GPIO4

    Soil Sensor
      VCC  -> 3V3
      GND  -> GND
      AOUT -> GPIO34

    Relay Module
      VCC -> 5V
      GND -> GND
      IN  -> GPIO26

    Motor
      Wired through relay COM/NO
      using its own external power supply

  Library required:

    adafruit/DHT sensor library
    adafruit/Adafruit Unified Sensor
*/

#include <Arduino.h>
#include <DHT.h>

// ---------- Pin definitions ----------

#define DHTPIN      4
#define DHTTYPE     DHT11
#define SOIL_PIN    34
#define RELAY_PIN   26

// ---------- Relay logic ----------

// YOUR relay behavior:
// HIGH = Relay ON
// LOW  = Relay OFF

#define RELAY_ON    HIGH
#define RELAY_OFF   LOW

// ---------- Soil threshold ----------

// Based on your actual sensor readings:
//
// 4095  -> DRY
// 2554  -> DRY
// 2435  -> WET
// 2410  -> WET
// 1803  -> WET
//
// Therefore:
//
// > 2450  = DRY
// <= 2450 = WET

const int SOIL_THRESHOLD = 2450;

// ---------- DHT object ----------

DHT dht(DHTPIN, DHTTYPE);

// ---------- Motor state ----------

bool motorRunning = false;

// ---------- Setup ----------

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // Start with motor OFF
  digitalWrite(RELAY_PIN, RELAY_OFF);

  dht.begin();

  Serial.println("=================================");
  Serial.println("Smart Terrace Irrigation System");
  Serial.println("=================================");
  Serial.println("System initialized.");
  Serial.println();
}

// ---------- Main loop ----------

void loop() {

  // =================================
  // Read DHT11
  // =================================

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("Failed to read from DHT11!");

  } else {

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // =================================
  // Read soil moisture
  // =================================

  int moistureValue = analogRead(SOIL_PIN);

  Serial.print("Moisture Value: ");
  Serial.println(moistureValue);

  // =================================
  // Motor control
  // =================================

  if (moistureValue > SOIL_THRESHOLD) {

    // -------------------------------
    // SOIL IS DRY
    // -------------------------------

    digitalWrite(RELAY_PIN, RELAY_ON);

    motorRunning = true;

    Serial.println("Soil is DRY");
    Serial.println("Motor is ON");

  } else {

    // -------------------------------
    // SOIL IS WET
    // -------------------------------

    digitalWrite(RELAY_PIN, RELAY_OFF);

    motorRunning = false;

    Serial.println("Soil is WET");
    Serial.println("Motor is OFF");
  }

  Serial.println("----------------------");

  delay(2000);
}