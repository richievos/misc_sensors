#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
// #include "esphome/components/i2c/i2c.h"

#include <Wire.h>

#include <memory>

namespace esphome {
namespace ph_robotank {

class PHSensor : public PollingComponent {
   public:
    std::unique_ptr<Sensor> _ph_sensor;
    uint8_t _i2cAddress;

    // constructor
    PHSensor(uint32_t update_interval) : PollingComponent(update_interval), address_(address), _ph_sensor(std::make_unique<Sensor>()) {}

    void setup() override;
    void update() override;

    void set_address(uint8_t address) { _i2cAddress = address; }
};

}  // namespace ph_robotank
}  // namespace esphome
