# IoT-Based Sensor Monitoring System (ESP32 + Ubidots)

An ESP32-based system that reads analog sensor data and publishes it to the
cloud in real time using MQTT, enabling remote monitoring and logging. Built
with an ECG/pulse sensor front-end in mind, but works with any analog sensor.

## How it works

1. The ESP32 connects to WiFi and establishes an MQTT connection to Ubidots'
   industrial broker.
2. The analog sensor is sampled on pin `A0`.
3. Each reading is packaged as a JSON payload and published to a
   device-specific MQTT topic.
4. Ubidots ingests the data, making it viewable on a live dashboard and
   available for historical logging.

## Hardware

- ESP32 development board
- Analog sensor (e.g. ECG/pulse sensor module) connected to pin A0

## Software / Libraries

- Arduino core for ESP32
- [PubSubClient](https://github.com/knolleary/pubsubclient) (MQTT)
- Ubidots (cloud IoT platform)

## Setup

1. Copy `config.example.h` to `config.h`.
2. Fill in your WiFi credentials and Ubidots token in `config.h`.
3. Flash `sensor_publisher.ino` to the ESP32 via the Arduino IDE.
4. Create a matching device/variable on your Ubidots account to visualize
   incoming data.

> `config.h` is gitignored — never commit real credentials to this repo.

## My role

Implemented the firmware for WiFi/MQTT connectivity and sensor data
publishing pipeline, and integrated it with the Ubidots dashboard for live

<img width="940" height="529" alt="image" src="https://github.com/user-attachments/assets/7956d6e9-d59c-4ee8-972a-bb32c03ac1cc" />

<img width="940" height="529" alt="image" src="https://github.com/user-attachments/assets/3c10f1a7-a6e9-460e-a543-051a47e7b3a0" />


visualization.

## Notes

This was built as a learning project to understand end-to-end IoT data
pipelines — from embedded sensor acquisition to cloud visualization.
