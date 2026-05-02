#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <PubSubClient.h>
#include <MQTTHADevice.h>

// Your Wi-Fi & MQTT credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Initialize the parent device
HADevice device("my_unique_123", "My Custom Relay");

// Create the manager
MQTTHADeviceManager manager(mqttClient, device);

// Attach entities
HASwitch myRelay("relay_1", "Relay 1");

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  manager.handleMessage(topic, payload, length);
}

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
}

void onRelayCommand(bool state) {
  Serial.print("Relay state changed to: ");
  Serial.println(state ? "ON" : "OFF");
  
  // Apply logic (e.g. digitalWrite)
  
  // Publish state back to HA
  manager.publishState(myRelay, state);
}

void setup() {
  Serial.begin(115200);

  // Configure Relay
  myRelay.onCommand(onRelayCommand);
  myRelay.setStateTopic("home/relay1/state");
  myRelay.setCommandTopic("home/relay1/set");

  // Add entities to manager
  manager.addEntity(&myRelay);

  // Setup WiFi
  setup_wifi();

  // Configure MQTT
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqttCallback);

  // Optional: Set MQTT credentials
  // manager.setCredentials("user", "pass");

  Serial.println("Setup completed.");
}

void loop() {
  // manager.loop() handles connection and discovery automatically
  manager.loop();
}
