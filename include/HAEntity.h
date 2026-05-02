/**
 * @file HAEntity.h
 * @brief Base representation for single Home Assistant component (sensor, switch).
 * @version 0.1
 * @date 2026-05-02
 */
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @class HAEntity
 * @brief Base class for any Home Assistant entity.
 */
class HAEntity {
public:
    /**
     * @brief Construct a new HAEntity object.
     * @param componentType The type of HA component (e.g. "sensor", "switch").
     * @param uniqueId A unique identifier (appended to the device id).
     * @param name The friendly name of the entity.
     */
    HAEntity(const char* componentType, const char* uniqueId, const char* name);

    /**
     * @brief Build the configuration JSON node for generating the `cmps` array.
     * @param doc The JsonObject instance that will hold the configuration.
     * @param baseTopic The base topic of the device (useful for `~` abbreviation).
     */
    virtual void buildConfig(JsonObject& doc, const char* baseTopic);

    const char* getUniqueId() const { return _uniqueId; }
    const char* getName() const { return _name; }
    const char* getComponentType() const { return _componentType; }

    /**
     * @brief Set the Device Class.
     * @param deviceClass e.g. "temperature", "humidity".
     */
    void setDeviceClass(const char* deviceClass) { _deviceClass = deviceClass; }

    /**
     * @brief Get the configured Device Class.
     */
    const char* getDeviceClass() const { return _deviceClass; }

protected:
    const char* _componentType;
    const char* _uniqueId;
    const char* _name;
    const char* _deviceClass;
};
