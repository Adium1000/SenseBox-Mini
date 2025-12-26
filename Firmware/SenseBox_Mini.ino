#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_CCS811.h>

/* WiFi Access Point configuration */
const char* ssid = "SenseBox-Mini";
const char* password = "sense1234";

/* Sensor objects */
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_CCS811 ccs;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("SenseBox Mini starting");

  /* Start WiFi in Access Point mode */
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("WiFi Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Initialize I2C bus */
  Wire.begin();
  Serial.println("I2C initialized");

  /* Initialize SHT31 sensor */
  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 not detected");
  } else {
    Serial.println("SHT31 initialized");
  }

  /* Initialize CCS811 sensor */
  if (!ccs.begin()) {
    Serial.println("CCS811 not detected");
  } else {
    Serial.println("CCS811 initialized");
  }

  Serial.println("Setup complete");
}

void loop() {

  /* Read temperature and humidity */
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read SHT31");
  }

  /* Read air quality data */
  if (ccs.available()) {
    if (!ccs.readData()) {
      Serial.print("eCO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print(" ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.println(" ppb");
    } else {
      Serial.println("Failed to read CCS811");
    }
  }

  delay(2000);
}