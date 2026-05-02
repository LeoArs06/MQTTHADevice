/**
 * @file MQTTHADeviceManager.cpp
 * @brief Implementation for Core MQTTHADeviceManager Auto-Discovery Controller.
 */
#include "MQTTHADeviceManager.h"

MQTTHADeviceManager::MQTTHADeviceManager(PubSubClient& mqttClient, HADevice& device)
    : _mqttClient(mqttClient), _device(device), _discoveryPrefix("homeassistant"),
      _onlinePayload("online"), _offlinePayload("offline"), 
      _mqttUser(nullptr), _mqttPass(nullptr), _discoveryPublished(false) {
    
    _availabilityTopic = String(_discoveryPrefix) + "/device/" + _device.getId() + "/status";
}

void MQTTHADeviceManager::setDiscoveryPrefix(const char* prefix) {
    _discoveryPrefix = prefix;
    _availabilityTopic = String(_discoveryPrefix) + "/device/" + _device.getId() + "/status";
}

void MQTTHADeviceManager::addEntity(HAEntity* entity) {
    _entities.push_back(entity);
}

void MQTTHADeviceManager::begin() {
    // _mqttClient.setServer("your_broker_ip", 1883); // Placeholder, user will configure client directly ideally.
}

void MQTTHADeviceManager::loop() {
    if (!_mqttClient.connected()) {
        _connectAndEnsureDiscovery();
    }
    _mqttClient.loop();
}

void MQTTHADeviceManager::handleMessage(char* topic, byte* payload, unsigned int length) {
    String topicStr(topic);
    String payloadStr;
    for (unsigned int i = 0; i < length; i++) {
        payloadStr += (char)payload[i];
    }

    for (size_t i = 0; i < _entities.size(); ++i) {
        const char* cmdTopic = _entities[i]->getCommandTopic();
        if (cmdTopic != nullptr && topicStr == cmdTopic) {
            _entities[i]->handleCommand(payloadStr.c_str());
        }
    }
}

void MQTTHADeviceManager::publishState(HASwitch& entity, bool state) {
    if (entity.getStateTopic() != nullptr) {
        _mqttClient.publish(entity.getStateTopic(), state ? "ON" : "OFF", true);
    }
}

void MQTTHADeviceManager::_connectAndEnsureDiscovery() {
    bool connected = false;
    if (_mqttUser != nullptr && _mqttPass != nullptr) {
        connected = _mqttClient.connect(_device.getId(), _mqttUser, _mqttPass, _availabilityTopic.c_str(), 1, true, _offlinePayload);
    } else {
        connected = _mqttClient.connect(_device.getId(), _availabilityTopic.c_str(), 1, true, _offlinePayload);
    }

    if (connected) {
        // Subscribe to all command topics
        for (size_t i = 0; i < _entities.size(); ++i) {
            const char* cmdTopic = _entities[i]->getCommandTopic();
            if (cmdTopic != nullptr) {
                _mqttClient.subscribe(cmdTopic);
            }
        }

        if (!_discoveryPublished) {
            publishDiscovery();
            _discoveryPublished = true;
        }
        _mqttClient.publish(_availabilityTopic.c_str(), _onlinePayload, true);
    }
}

bool MQTTHADeviceManager::publishDiscovery() {
    String topic = String(_discoveryPrefix) + "/device/" + _device.getId() + "/config";
    
    JsonDocument doc;
    
    JsonObject dev = doc["dev"].to<JsonObject>();
    JsonArray ids = dev["ids"].to<JsonArray>();
    ids.add(_device.getId());
    dev["name"] = _device.getName();
    if (_device.getManufacturer() != nullptr) dev["mf"] = _device.getManufacturer();
    if (_device.getModel() != nullptr) dev["mdl"] = _device.getModel();
    if (_device.getSoftwareVersion() != nullptr) dev["sw"] = _device.getSoftwareVersion();

    JsonObject o = doc["o"].to<JsonObject>();
    o["name"] = "MQTTHADevice_Discovery";
    o["sw"] = "0.1.0";

    JsonObject cmps = doc["cmps"].to<JsonObject>();
    for (size_t i = 0; i < _entities.size(); ++i) {
        String key = String("c_") + _entities[i]->getUniqueId();
        JsonObject cmp = cmps[key].to<JsonObject>();
        _entities[i]->buildConfig(cmp, nullptr);
    }
    
    doc["availability_topic"] = _availabilityTopic;
    doc["payload_available"] = _onlinePayload;
    doc["payload_not_available"] = _offlinePayload;

    String payload;
    serializeJson(doc, payload);

    return _mqttClient.publish(topic.c_str(), payload.c_str(), true);
}
