// SenseBox Mini
// Initial firmware for ESP32-C3
// Features: mock sensors, serial output, capacitive touch, single alert buzzer

#include <Wire.h>

// Pin definitions
#define LED_PIN        5
#define TOUCH_PIN      6
#define I2C_SCL        7
#define I2C_SDA        8
#define BUZZER_PIN     9

// Mock sensor values
float temperature = 22.5;
float humidity = 48.0;
int airQuality = 350;

// Threshold values
float tempThreshold = 28.0;
float humidityThreshold = 70.0;
int airThreshold = 600;

// Alert state
bool alertTriggered = false;

// Touch sensor
int touchValue = 0;
int touchThreshold = 30;

// Setup function
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH);

  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("SenseBox Mini starting");
  Serial.println("System initialized");
}

// Buzzer beep function
void beepOnce() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}

// Main loop
void loop() {

  temperature += 0.1;
  humidity += 0.2;
  airQuality += 5;

  Serial.println("Sensor readings");

  Serial.print("Temperature ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Air quality ");
  Serial.println(airQuality);

  touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch value ");
  Serial.println(touchValue);

  if (touchValue < touchThreshold) {
    Serial.println("Touch detected");
  }

  if (!alertTriggered &&
      (temperature > tempThreshold ||
       humidity > humidityThreshold ||
       airQuality > airThreshold)) {

    Serial.println("Threshold exceeded");
    beepOnce();
    alertTriggered = true;
  }

  if (temperature < tempThreshold &&
      humidity < humidityThreshold &&
      airQuality < airThreshold) {

    alertTriggered = false;
  }

  Serial.println();

  delay(2000);
}