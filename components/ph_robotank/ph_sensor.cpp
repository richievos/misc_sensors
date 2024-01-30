// #include "esphome.h"
// #include <Wire.h>
#include "ph_sensor.h"

#include "../../lib/robotank/ph-robotank-sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ph_robotank {

void PHSensor::setup() {
    // This will be called by ESPHome upon startup, initialize the sensor
    setupPH_RoboTankPHBoard();
}

void PHSensor::update() {
    const auto ph = readPHSignal_RoboTankPHBoard(_i2cAddress);

    ph_sensor->publish_state(ph);
}

}  // namespace ph_robotank

}  // namespace esphome
