// #include "esphome.h"
// #include <Wire.h>
#include "ph_robotank.h"

#include "ph-robotank-sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ph_robotank {

void PHSensor::setup() {
    // This will be called by ESPHome upon startup, initialize the sensor
    setupPH_RoboTankPHBoard();
}

void PHSensor::update() {
    const auto ph = readPHSignal_RoboTankPHBoard(_i2cAddress);

    publish_state(ph);
}

void PHSensor::set_address(uint8_t address) { _i2cAddress = address; }

}  // namespace ph_robotank

}  // namespace esphome
