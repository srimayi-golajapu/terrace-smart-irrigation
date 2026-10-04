/*
  ESP32 + DHT11 + Soil Moisture Sensor + 1-Channel Relay (Active LOW)
  Motor runs when soil is DRY, turns off when soil is WET.

  Wiring:
    DHT11        VCC -> 3V3   GND -> GND   DATA -> GPIO4
    Soil Sensor  VCC -> 3V3   GND -> GND   AOUT -> GPIO34
    Relay Module VCC -> 5V    GND -> GND   IN   -> GPIO26
    Motor -> wired through relay COM/NO with its own external power supply

  Library required (PlatformIO):
    lib_deps =
        adafruit/DHT sensor library
        adafruit/Adafruit Unified Sensor
*/

#include <Arduino.h>
#include <DHT.h>

// ---------- Pin definitions ----------
#define DHTPIN        4
#define DHTTYPE       DHT11
#define SOIL_PIN      34
#define RELAY_PIN     26

// ---------- Relay logic ----------
// Active LOW relay: LOW = relay energized (ON), HIGH = relay de-energized (OFF)
#define RELAY_ON      LOW
#define RELAY_OFF     HIGH

// ---------- Soil moisture threshold ----------
// Raw ADC reading (0-4095). Higher raw value = drier soil for most analog modules.
// Calibrate these two values for your specific sensor/soil.
const int DRY_THRESHOLD = 2800;   // above this => considered DRY -> motor ON
const int WET_THRESHOLD = 2000;   // below this => considered WET -> motor OFF

DHT dht(DHTPIN, DHTTYPE);

bool motorRunning = false;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);   // start with motor OFF

  dht.begin();

  Serial.println("System initialized. Monitoring soil moisture...");
}

void loop() {
  // --- Read temperature & humidity (for logging/monitoring) ---
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" C  Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // --- Read soil moisture ---
  int soilRaw = analogRead(SOIL_PIN);
  Serial.print("Soil raw value: ");
  Serial.println(soilRaw);

  // --- Motor control with hysteresis to avoid relay chatter ---
  if (soilRaw >= DRY_THRESHOLD && !motorRunning) {
    digitalWrite(RELAY_PIN, RELAY_ON);
    motorRunning = true;
    Serial.println("Soil is DRY -> Motor ON");
  } else if (soilRaw <= WET_THRESHOLD && motorRunning) {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    motorRunning = false;
    Serial.println("Soil is WET -> Motor OFF");
  }

  delay(2000); // poll every 2 seconds
}
