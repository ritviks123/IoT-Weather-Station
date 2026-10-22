# IoT Weather Station

A two-board weather station. An outdoor ESP32-C3 reads temperature, humidity and light every 10 seconds and sends a report to an indoor ESP32-C3, which shows it on a 16x2 LCD.

It started as Project 6 of IEEE at UC Irvine's Open Project Space. I'm now extending it so the readings go to the cloud and anyone can see them on a live dashboard.

- **Write-up:** https://ritviksingh.net/projects/iot-weather-station

## Status

| Part | Status |
|---|---|
| Sensor station (temperature, humidity, day/night) | ✅ Working |
| ESP-NOW link between the two boards | ✅ Working |
| LCD display | ✅ Working |
| MQTT link to AWS | 🚧 In progress |
| Public live dashboard | 🚧 Planned |

## How it works

1. **Transmitter** reads the AHT20 sensor (temperature + humidity) over I2C and a photoresistor on an analog pin.
2. A light reading above **2000** (out of 4095) counts as **day**, otherwise **night**.
3. It packs the three values into one `WeatherReport` and sends it over **ESP-NOW**, a direct board-to-board Wi-Fi link with no router needed.
4. **Receiver** keeps the latest report and redraws the LCD every 10 seconds.

```
LCD
┌────────────────┐
│T:22C H:48%     │
│Time: Day       │
└────────────────┘
```

## Report format

| Field | Type | Meaning |
|---|---|---|
| `temperature` | int | Degrees Celsius |
| `humidity` | int | Relative humidity, % |
| `light` | int | 1 = day, 0 = night |

## Hardware

**Transmitter (outdoor)**
- ESP32-C3
- AHT20 temperature and humidity sensor (I2C)
- Photoresistor with a voltage divider resistor

**Receiver (indoor)**
- ESP32-C3
- 16x2 LCD with I2C backpack (address `0x27`)

## Pin map

**Transmitter** (`ops_project6_transmitter.ino`)

| Signal | GPIO |
|---|---|
| AHT20 SDA | 6 |
| AHT20 SCL | 7 |
| Photoresistor (analog) | 4 |

**Receiver** (`ops_project6_receiver.ino`)

| Signal | GPIO |
|---|---|
| LCD SDA | 8 |
| LCD SCL | 9 |

## Files

| File | What it is |
|---|---|
| `ops_project6_transmitter.ino` | Reads the sensors and sends a report every 10 seconds |
| `ops_project6_receiver.ino` | Receives reports and shows the latest one on the LCD |

## Setup

1. Install the Arduino IDE (2.x) and ESP32 board support, then select your ESP32-C3 board.
2. Install the libraries: **Adafruit AHTX0**, **LiquidCrystal I2C**, and **WiFiConfig** (handles the ESP-NOW link and defines `WeatherReport`; not my code).
3. In `ops_project6_transmitter.ino`, replace `receiverAddress` with your receiver board's MAC address.
4. If your LCD stays blank, check its I2C address (`0x27` here) and turn the contrast screw on the backpack.
5. Adjust the day/night threshold (`2000`) for your photoresistor and lighting.
6. Upload the transmitter sketch to the outdoor board and the receiver sketch to the indoor board.

## Roadmap

- [ ] Publish each report over MQTT to a broker on AWS
- [ ] Live public dashboard that subscribes to the readings
- [ ] Keep the LCD as a local display

## Credits

- Project spec: IEEE at UC Irvine, Open Project Space (Project 6)
- `WiFiConfig` library: not written by me
