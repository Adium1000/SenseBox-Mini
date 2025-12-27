// SenseBox Mini
// ESP32 firmware with WiFi Access Point and test web page

#include <WiFi.h>
#include <WebServer.h>
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

// WiFi Access Point credentials
const char* ssid = "SenseBox-Mini";
const char* password = "sensebox";

// Web server
WebServer server(80);

// Simple test page
const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>SenseBox Mini</title>
</head>
<body style="font-family: Arial; text-align: center;">
  <h1>SenseBox Mini</h1>
  <h2>Dashboard</h2>
  <p>Test web interface</p>
</body>
</html>
)rawliteral";

// Buzzer function
void beepOnce() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}

// Setup
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH);

  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("SenseBox Mini starting");

  // Start WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("WiFi AP started, IP: ");
  Serial.println(IP);

  // Web server route
  server.on("/", []() {
    server.send(200, "text/html", PAGE);
  });

  server.begin();
  Serial.println("Web server started");
}

// Main loop
void loop() {

  server.handleClient();

  temperature += 0.1;
  humidity += 0.2;
  airQuality += 5;

  Serial.println("Sensor readings");
  Serial.print("Temperature "); Serial.println(temperature);
  Serial.print("Humidity "); Serial.println(humidity);
  Serial.print("Air quality "); Serial.println(airQuality);

  touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch value "); Serial.println(touchValue);

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
