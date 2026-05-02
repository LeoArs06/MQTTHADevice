/**
 * @file HAEntity.cpp
 * @brief Implementation of Base HAEntity class.
 */
#include "HAEntity.h"

HAEntity::HAEntity(const char* componentType, const char* uniqueId, const char* name)
    : _componentType(componentType), _uniqueId(uniqueId), _name(name), _deviceClass(nullptr) {
}

void HAEntity::buildConfig(JsonObject& doc, const char* baseTopic) {
    doc["p"] = _componentType;
    if (_name != nullptr) {
        doc["name"] = _name;
    }
    doc["unique_id"] = _uniqueId;
    if (_deviceClass != nullptr) {
        doc["device_class"] = _deviceClass;
    }
}
