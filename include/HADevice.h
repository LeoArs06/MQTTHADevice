/**
 * @file HADevice.h
 * @brief Home Assistant Device representation.
 * @version 0.1
 * @date 2026-05-02
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <Arduino.h>

/**
 * @class HADevice
 * @brief Represents a physical device in Home Assistant.
 * 
 * This class holds the metadata for the device, such as its name, model, 
 * manufacturer, and firmware version. This data is bundled into the `dev` 
 * node of the MQTT discovery payload.
 */
class HADevice {
public:
    /**
     * @brief Construct a new HADevice object.
     * 
     * @param id Unique identifier for the device (e.g., MAC address).
     * @param name Friendly name of the device.
     */
    HADevice(const char* id, const char* name);

    /**
     * @brief Set the Manufacturer name.
     * @param manufacturer The manufacturer string.
     */
    void setManufacturer(const char* manufacturer);

    /**
     * @brief Set the Model name.
     * @param model The model string.
     */
    void setModel(const char* model);

    /**
     * @brief Set the Software Version.
     * @param swVersion Firmware version string.
     */
    void setSoftwareVersion(const char* swVersion);

    const char* getId() const { return _id; }
    const char* getName() const { return _name; }
    const char* getManufacturer() const { return _manufacturer; }
    const char* getModel() const { return _model; }
    const char* getSoftwareVersion() const { return _swVersion; }

private:
    const char* _id;
    const char* _name;
    const char* _manufacturer;
    const char* _model;
    const char* _swVersion;
};
