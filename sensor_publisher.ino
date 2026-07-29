/*
 * IoT Sensor Data Publisher (ESP32 + Ubidots)
 * ---------------------------------------------
 * Reads an analog sensor (configured here for an analog front-end such as
 * an ECG/pulse sensor on pin A0) and publishes readings to the Ubidots
 * cloud platform over MQTT, so data can be viewed and logged remotely.
 *
 * Credentials are pulled from config.h (NOT committed to this repo).
 * Copy config.example.h to config.h and fill in your own values before
 * building.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"   // WIFI_SSID, WIFI_PASSWORD, UBIDOTS_TOKEN, MQTT_CLIENT_NAME

/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "sensor"   // Ubidots variable label
#define DEVICE_LABEL   "esp32"    // Ubidots device label
#define SENSOR_PIN     A0         // Analog input pin

char mqttBroker[] = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char str_sensor[10];

/****************************************
 * MQTT Client Setup
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = '\0';
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_NAME, UBIDOTS_TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  pinMode(SENSOR_PIN, INPUT);

  Serial.print("Waiting for WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL);

  float sensor = analogRead(SENSOR_PIN);
  dtostrf(sensor, 4, 2, str_sensor);  // width 4, precision 2
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor);

  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(500);
}
