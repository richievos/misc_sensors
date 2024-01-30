#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
// #include "esphome/components/i2c/i2c.h"

#include <Wire.h>

#include <memory>

namespace esphome {
namespace ph_robotank {

class PHSensor : public sensor::Sensor, public PollingComponent {
   public:
    // constructor
    // PHSensor(uint32_t update_interval) : PollingComponent(update_interval) {}

    void setup() override;
    void update() override;

    void set_address(uint8_t address);
    void set_sda(uint8_t sda);
    void set_scl(uint8_t scl);

   private:
    uint8_t _i2cAddress;
    uint8_t _sda;
    uint8_t _scl;

};

}  // namespace ph_robotank
}  // namespace esphome
