/**
 * @file HASensor.h
 * @brief Home Assistant Sensor component.
 * @version 0.1
 */
#pragma once

#include "HAEntity.h"

/**
 * @class HASensor
 * @brief Represents a simple sensor in Home Assistant (temperature, humidity, etc.)
 */
class HASensor : public HAEntity {
public:
    HASensor(const char* uniqueId, const char* name)
        : HAEntity("sensor", uniqueId, name), _unitOfMeasurement(nullptr), _stateTopic(nullptr), _valueTemplate(nullptr) {}

    void setUnitOfMeasurement(const char* unit) { _unitOfMeasurement = unit; }
    void setStateTopic(const char* topic) { _stateTopic = topic; }
    void setValueTemplate(const char* valTemplate) { _valueTemplate = valTemplate; }

    void buildConfig(JsonObject& doc, const char* baseTopic) override {
        HAEntity::buildConfig(doc, baseTopic);
        if (_unitOfMeasurement != nullptr) doc["unit_of_measurement"] = _unitOfMeasurement;
        if (_stateTopic != nullptr) doc["state_topic"] = _stateTopic;
        if (_valueTemplate != nullptr) doc["value_template"] = _valueTemplate;
    }

private:
    const char* _unitOfMeasurement;
    const char* _stateTopic;
    const char* _valueTemplate;
};
