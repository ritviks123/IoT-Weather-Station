#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiConfig.h>

#define I2C_SDA 8
#define I2C_SCL 9

LiquidCrystal_I2C lcd(0x27, 16, 2);
WeatherReport report;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");

  wifi_setup();
  set_report_receiver(&report);

  Serial.println("Receiver ready.");
}

void loop() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(report.temperature);
  lcd.print("C H:");
  lcd.print(report.humidity);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(report.light ? "Day" : "Night");

  delay(10000);
}