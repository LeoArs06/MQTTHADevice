/**
 * @file HASwitch.h
 * @brief Home Assistant Switch component.
 * @version 0.1
 */
#pragma once

#include "HAEntity.h"
#include <functional>

/**
 * @class HASwitch
 * @brief Represents a switch actuator in Home Assistant.
 */
class HASwitch : public HAEntity {
public:
    typedef std::function<void(bool state)> CommandCallback;

    HASwitch(const char* uniqueId, const char* name)
        : HAEntity("switch", uniqueId, name), _stateTopic(nullptr), _commandTopic(nullptr), _state(false), _callback(nullptr) {}

    void setStateTopic(const char* topic) { _stateTopic = topic; }
    void setCommandTopic(const char* topic) { _commandTopic = topic; }

    const char* getStateTopic() const { return _stateTopic; }
    const char* getCommandTopic() const { return _commandTopic; }

    /**
     * @brief Set the internal state.
     * @param state true for ON, false for OFF.
     */
    void setState(bool state) { _state = state; }

    /**
     * @brief Get the current internal state.
     * @return true if ON.
     */
    bool getState() const { return _state; }

    /**
     * @brief Set the callback for when the state is commanded from Home Assistant.
     * @param callback std::function to handle state change (boolean).
     */
    void onCommand(CommandCallback callback) { _callback = callback; }

    void handleCommand(const char* payload) {
        if (_callback) {
            String p(payload);
            p.toUpperCase();
            _callback(p == "ON");
        }
    }

    void buildConfig(JsonObject& doc, const char* baseTopic) override {
        HAEntity::buildConfig(doc, baseTopic);
        if (_stateTopic != nullptr) doc["state_topic"] = _stateTopic;
        if (_commandTopic != nullptr) doc["command_topic"] = _commandTopic;
    }

private:
    const char* _stateTopic;
    const char* _commandTopic;
    bool _state;
    CommandCallback _callback;
};
