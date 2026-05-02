/**
 * @file MQTTHADeviceManager.h
 * @brief Core Auto-Discovery Logic utilizing `cmps` approach.
 * @version 0.1
 * @date 2026-05-02
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>
#include "HADevice.h"
#include "HAEntity.h"
#include "entities/HASensor.h"
#include "entities/HASwitch.h"

/**
 * @class MQTTHADeviceManager
 * @brief Main controller for Home Assistant MQTT Auto Discovery via device components (`cmps`).
 */
class MQTTHADeviceManager {
public:
    MQTTHADeviceManager(PubSubClient& mqttClient, HADevice& device);

    /**
     * @brief Set the Discovery Prefix used globally in Home Assistant.
     * @param prefix Usually "homeassistant".
     */
    void setDiscoveryPrefix(const char* prefix);

    /**
     * @brief Add an entity to the device discovery package.
     * @param entity Pointer to the instantiated HAEntity base or derivative.
     */
    void addEntity(HAEntity* entity);

    /**
     * @brief Call this once during setup. Sets up the Birth/LWT configuration.
     */
    void begin();

    /**
     * @brief Publishes the device configuration to the broker (`cmps` array).
     * @return true if successfully published.
     */
    bool publishDiscovery();

    /**
     * @brief Should be called periodically in the `loop()` function.
     * Manages MQTT state, reconnection logic, and callback distribution.
     */
    void loop();

    /**
     * @brief Centralized MQTT message handler.
     * Route this from your `mqttClient.setCallback`.
     */
    void handleMessage(char* topic, byte* payload, unsigned int length);

    /**
     * @brief Publish the state of a switch entity.
     * @param entity The HASwitch entity.
     * @param state true for ON, false for OFF.
     */
    void publishState(HASwitch& entity, bool state);

    /**
     * @brief The payload sent on disconnected LWT. Default: "offline"
     * @param payload string value
     */
    void setOfflinePayload(const char* payload) { _offlinePayload = payload; }

    /**
     * @brief The payload sent on successful connect Birth. Default: "online"
     * @param payload string value
     */
    void setOnlinePayload(const char* payload) { _onlinePayload = payload; }

    /**
     * @brief Set MQTT credentials for authentication.
     * @param user MQTT username.
     * @param pass MQTT password.
     */
    void setCredentials(const char* user, const char* pass) {
        _mqttUser = user;
        _mqttPass = pass;
    }

private:
    PubSubClient& _mqttClient;
    HADevice& _device;
    const char* _discoveryPrefix;
    const char* _onlinePayload;
    const char* _offlinePayload;
    const char* _mqttUser;
    const char* _mqttPass;
    std::vector<HAEntity*> _entities;

    String _availabilityTopic;
    bool _discoveryPublished;

    void _connectAndEnsureDiscovery();
};
