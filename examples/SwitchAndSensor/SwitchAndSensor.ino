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

HADevice device("sensor_switch_1");
HASwitch myRelay("Main Relay", "relay_1");
HASensor myTemperature("Room Temperature", "temp_1", "temperature", "°C");

long lastMsg = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages (e.g., turning the switch on/off)
  // Check if topic matches the command topic for the relay
  // and control the RELAY_PIN accordingly
}

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
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(device.getId().c_str())) {
      Serial.println("connected");
      
      // Send auto-discovery configuration
      // // Example with manager (assuming it's initialized as `MQTTHADeviceManager manager(mqttClient, device);`)
      // manager.publishDiscovery(); 
      
      // Subscribe to command topic for switch
      // mqttClient.subscribe(myRelay.getCommandTopic().c_str());
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
  pinMode(RELAY_PIN, OUTPUT);

  // Configure Home Assistant Device
  device.setName("Room Controller");
  device.setModel("ESP32 Test Board");

  // Attach entities
  device.addEntity(&myRelay);
  device.addEntity(&myTemperature);

  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  long now = millis();
  // Publish temperature every 10 seconds
  if (now - lastMsg > 10000) {
    lastMsg = now;
    float simulatedTemp = 20.0 + random(-5, 5); // Example fake data
    
    // Publish sensor state
    // mqttClient.publish(myTemperature.getStateTopic().c_str(), String(simulatedTemp).c_str());
    Serial.print("Published Temp: ");
    Serial.println(simulatedTemp);
  }
}
