#pragma once

#include "ph-robotank-sensor.h"

#include "readings/ph-common.h"
#include "readings/ph-controller.h"
#include "readings/ph.h"

//
#include "../inputs/creds.h"
// #include "creds.h"

namespace inputs {
using namespace buff;

struct ArduinoPinConfig {
    short I2C_SDA;
    short I2C_SCL;
};

const ArduinoPinConfig ESP32_CONFIG = {
    .I2C_SDA = 14,
    .I2C_SCL = 12,
};

const auto PIN_CONFIG = ESP32_CONFIG;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define BUFF_NAME TOSTRING(OPT_SYS_NAME)

// hostname attempted to be used for wifi dhcp
const std::string hostname = BUFF_NAME;

// I2C address of the PH reading board (from the circuit board's docs)
const auto roboTankPHSensorI2CAddress = 98l;
// const auto roboTankPHSensorI2CAddress = 95l;
defineRoboTankSignalReaderFunc(roboTankPHSensorI2CAddress);

const ph::PHReadConfig phReadConfig = {
    // how often to read a new ph value
    .readIntervalMS = 1000,

    .phReadFunc = nameForRoboTankSignalReaderFunc(roboTankPHSensorI2CAddress)};

// esp32 on board, Board4 (inconsistent?), black cheap pH probe
// const ph::PHCalibrator::CalibrationPoint phHighPoint = {.actualPH = 10.0, .readPH = 9.08};
// const ph::PHCalibrator::CalibrationPoint phLowPoint = {.actualPH = 7.0, .readPH = 6.76};

// esp32 on board, Board4 (inconsistent?), BRS pH probe
const ph::PHCalibrator::CalibrationPoint phHighPoint = {.actualPH = 10.0, .readPH = 9.35};
const ph::PHCalibrator::CalibrationPoint phLowPoint = {.actualPH = 7.0, .readPH = 6.99};


ph::PHCalibrator phCalibrator(phLowPoint, phHighPoint);
}  // namespace inputs
