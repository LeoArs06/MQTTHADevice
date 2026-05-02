/**
 * @file HAMQTT.cpp
 * @brief Implementation for Core HAMQTT Auto-Discovery Controller.
 */
#include "HAMQTT.h"

HAMQTT::HAMQTT(PubSubClient& mqttClient, HADevice& device)
    : _mqttClient(mqttClient), _device(device), _discoveryPrefix("homeassistant"),
      _onlinePayload("online"), _offlinePayload("offline"), _discoveryPublished(false) {
    
    _availabilityTopic = String(_discoveryPrefix) + "/device/" + _device.getId() + "/status";
}

void HAMQTT::setDiscoveryPrefix(const char* prefix) {
    _discoveryPrefix = prefix;
    _availabilityTopic = String(_discoveryPrefix) + "/device/" + _device.getId() + "/status";
}

void HAMQTT::addEntity(HAEntity* entity) {
    _entities.push_back(entity);
}

void HAMQTT::begin() {
    _mqttClient.setServer("your_broker_ip", 1883); // Placeholder, user will configure client directly ideally.
    _mqttClient.setWill(_availabilityTopic.c_str(), _offlinePayload, true, 1);
}

void HAMQTT::loop() {
    if (!_mqttClient.connected()) {
        _connectAndEnsureDiscovery();
    }
    _mqttClient.loop();
}

void HAMQTT::_connectAndEnsureDiscovery() {
    if (_mqttClient.connect(_device.getId(), _availabilityTopic.c_str(), 1, true, _offlinePayload)) {
        if (!_discoveryPublished) {
            publishDiscovery();
            _discoveryPublished = true;
        }
        _mqttClient.publish(_availabilityTopic.c_str(), _onlinePayload, true);
    }
}

bool HAMQTT::publishDiscovery() {
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
    o["name"] = "HAMQTT_Device_Discovery";
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
