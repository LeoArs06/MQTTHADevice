#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <PubSubClient.h>

#include "HADevice.h"
#include "HAMQTT.h"
#include "entities/HASwitch.h"

// Your Wi-Fi & MQTT credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Initialize the parent device
HADevice device("my_unique_123");

// Attach entities
HASwitch myRelay("Relay 1", "relay_1");

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(device.getId().c_str())) {
      Serial.println("connected");
      
      // Publish discovery messages once connected
      // HAMQTT::publishDiscovery(&mqttClient, device); // Assuming this is how it works
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // 1. Setup the device name
  device.setName("My Custom Relay");

  // 2. Attach entities
  device.addEntity(&myRelay);

  // 3. Connect to WiFi
  setup_wifi();

  // 4. Set MQTT Server
  mqttClient.setServer(mqtt_server, 1883);

  Serial.println("Setup completed.");
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  // Your logic here...
}