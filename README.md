# MQTTHADevice

A professional, lightweight Arduino/ESP library for Home Assistant MQTT Device Auto-Discovery utilizing the new `cmps` array feature.

## Features
- **Easy Setup:** Simple API to register devices and entities to Home Assistant.
- **Auto-Discovery:** Automatically exposes entities to Home Assistant without YAML configuration.
- **Optimized:** Uses modern JSON structuring (`cmps`) to keep payload sizes small.
- **Compatible:** Supports ESP32 and ESP8266 platforms.

## Installation

### PlatformIO
Add the following to your `platformio.ini` file's `lib_deps`:

```ini
lib_deps =
    LeonardoArsie/MQTTHADevice
    bblanchon/ArduinoJson @ ^7.0.0
    knolleary/PubSubClient @ ^2.8.0
```

### Arduino IDE
Download this repository as a `.zip` file and install it via `Sketch` -> `Include Library` -> `Add .ZIP Library...`

## Dependencies
- [ArduinoJson](https://arduinojson.org/) (v7.0+)
- [PubSubClient](https://pubsubclient.knolleary.net/) (v2.8+)

## Usage

Check the `examples` folder for detailed usage instructions.

## License
MIT License. See the `LICENSE` file.
