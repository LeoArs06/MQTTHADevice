/**
 * @file HADevice.cpp
 * @brief Implementation of Home Assistant Device class.
 */
#include "HADevice.h"

HADevice::HADevice(const char* id, const char* name)
    : _id(id), _name(name), _manufacturer(nullptr), _model(nullptr), _swVersion(nullptr) {
}

void HADevice::setManufacturer(const char* manufacturer) {
    _manufacturer = manufacturer;
}

void HADevice::setModel(const char* model) {
    _model = model;
}

void HADevice::setSoftwareVersion(const char* swVersion) {
    _swVersion = swVersion;
}
