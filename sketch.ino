#define BLYNK_TEMPLATE_ID "TMPL3NKb-nHvl"
#define BLYNK_TEMPLATE_NAME "Smart disaster monitoring system"
#define BLYNK_AUTH_TOKEN "Iw40YjdU2JlobkgBjXIMpby8LYO7JuwT"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define ssid"Wokwi-GUEST"
#define pass""

// ---------------- Pin Definitions ----------------
#define DHTPIN 4
#define DHTTYPE DHT22

#define GAS_PIN 34
#define RAIN_PIN 35

#define LED_PIN 2
#define BUZZER_PIN 18

// ---------------- Sensor Object ----------------
DHT dht(DHTPIN, DHTTYPE);

// ---------------- Variables ----------------
float temperature = 0;
int gasValue = 0;
int rainValue = 0;

// ---------------- Threshold Values ----------------
const float TEMP_THRESHOLD = 40.0;
const int GAS_THRESHOLD = 2000;
const int RAIN_THRESHOLD = 2500;

// ---------------- Function Prototypes ----------------
void readSensors();
void checkEmergency();
void displayValues();

void setup() {

  Serial.begin(115200);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("==========================================");
  Serial.println(" Smart Disaster Monitoring System");
  Serial.println(" ESP32 Initialized Successfully");
  Serial.println("==========================================");
}

void loop() {

  Blynk.run();

  readSensors();

  displayValues();

  checkEmergency();

  delay(1000);
}

//--------------------------------------------------

void readSensors() {

  // Read Temperature
  temperature = dht.readTemperature();

  // Read Gas Sensor
  gasValue = analogRead(GAS_PIN);

  // Read Rain Sensor
  rainValue = analogRead(RAIN_PIN);

}

//--------------------------------------------------

void displayValues() {

  Serial.println("----------------------------------");

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Gas Value   : ");
  Serial.println(gasValue);

  Serial.print("Rain Value  : ");
  Serial.println(rainValue);

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, gasValue);
  Blynk.virtualWrite(V2, rainValue);

  Serial.println("----------------------------------");

}

//--------------------------------------------------

void checkEmergency() {

  bool emergency = false;

  // Fire Detection
  if (temperature >= TEMP_THRESHOLD) {
    Serial.println("🔥 FIRE ALERT!");
    Blynk.virtualWrite(V3, "FIRE ALERT");
    Blynk.logEvent("fire_alert");
    emergency = true;
  }

  // Gas Leak Detection
  if (gasValue >= GAS_THRESHOLD) {
    Serial.println("☣ GAS LEAK ALERT!");
    Blynk.virtualWrite(V3, "GAS LEAK ALERT");
    Blynk.logEvent("gas_alert");
    emergency = true;
  }

  // Heavy Rain Detection
  if (rainValue >= RAIN_THRESHOLD) {
    Serial.println("🌧 HEAVY RAIN ALERT!");
    Blynk.virtualWrite(V3, "HEAVY RAIN ALERT");
    Blynk.logEvent("rain_alert");
    emergency = true;
  }

  if (emergency) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
  } 
  else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    Serial.println("SYSTEM STATUS : SAFE");
    Serial.println("All sensor readings are within normal limits.");

    Blynk.virtualWrite(V3, "SAFE");

  }

}