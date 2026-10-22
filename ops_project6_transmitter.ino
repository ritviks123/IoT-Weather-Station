#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <WiFiConfig.h>

#define I2C_SDA 6
#define I2C_SCL 7
#define PHOTO_PIN 4

uint8_t receiverAddress[] = {0xA8, 0x46, 0x74, 0x5C, 0x16, 0x14};

Adafruit_AHTX0 aht;
WeatherReport report;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA, I2C_SCL);

  if (aht.begin()) {
    Serial.println("AHT20: Ready");
  } else {
    Serial.println("AHT20: FAILED");
  }

  wifi_setup();
  peer_setup(receiverAddress);

  Serial.println("Transmitter ready.");
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  int lightVal = analogRead(PHOTO_PIN);

  report.temperature = (int)temp.temperature;
  report.humidity = (int)humidity.relative_humidity;
  report.light = (lightVal > 2000) ? 1 : 0;

  send_report(receiverAddress, report);

  Serial.print("Temp: "); Serial.print(report.temperature); Serial.println("C");
  Serial.print("Humidity: "); Serial.print(report.humidity); Serial.println("%");
  Serial.print("Time: "); Serial.println(report.light ? "Day" : "Night");
  Serial.println("--- Sent ---");

  delay(10000);
}