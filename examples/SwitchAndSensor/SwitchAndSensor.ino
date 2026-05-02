#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <PubSubClient.h>
#include <MQTTHADevice.h>

// Hardware settings
#define RELAY_PIN 4

// Network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.10";

// Objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Initialize Device & Entities
HADevice device("sensor_switch_1", "Room Controller");
HASwitch myRelay("relay_1", "Main Relay");
HASensor myTemperature("temp_1", "Room Temperature");
MQTTHADeviceManager manager(mqttClient, device);

long lastMsg = 0;

void onRelayCommand(bool state) {
  digitalWrite(RELAY_PIN, state ? HIGH : LOW);
  Serial.print("Relay set to: ");
  Serial.println(state ? "ON" : "OFF");
  
  // Update state in HA
  manager.publishState(myRelay, state);
}

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

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);

  // Configure Device
  device.setModel("ESP32 Test Board");
  device.setManufacturer("Leonardo Arsie");

  // Configure Switch
  myRelay.setCommandTopic("home/room/relay/set");
  myRelay.setStateTopic("home/room/relay/state");
  myRelay.onCommand(onRelayCommand);

  // Configure Sensor
  myTemperature.setStateTopic("home/room/temp/state");
  myTemperature.setDeviceClass("temperature");
  myTemperature.setUnitOfMeasurement("°C");

  // Add entities to manager
  manager.addEntity(&myRelay);
  manager.addEntity(&myTemperature);

  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqttCallback);
}

void loop() {
  manager.loop();

  long now = millis();
  // Publish temperature every 10 seconds
  if (now - lastMsg > 10000) {
    lastMsg = now;
    float simulatedTemp = 20.0 + (random(-50, 50) / 10.0);
    
    // Publish sensor state manually (manager handles Switch but user handles Sensor values)
    mqttClient.publish(myTemperature.getStateTopic(), String(simulatedTemp).c_str(), true);
    
    Serial.print("Published Temp: ");
    Serial.println(simulatedTemp);
  }
}
