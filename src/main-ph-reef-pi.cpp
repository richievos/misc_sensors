#ifdef REEF_PI_PH
#include <Arduino.h>
#include <Wire.h>

#include <map>

#include "mywifi.h"
#include "ota.h"
#include "ph-robotank-sensor.h"
#include "readings/ph-common.h"
#include "readings/ph-controller.h"
#include "readings/ph.h"
#include "reef-pi-web-server.h"

//
#include "sensors/sensors.h"

//
#include "inputs.h"

namespace sensor_main {
using namespace buff;

/******************
 * Common vars
 *******************/
// PH Readings
const size_t STANDARD_PH_MAVG_LENGTH = 30;
auto phReader = std::make_shared<buff::ph::controller::PHReader>(inputs::phReadConfig, inputs::phCalibrator);
auto phReadingStats = std::make_shared<ph::controller::PHReadingStats<STANDARD_PH_MAVG_LENGTH>>();

std::shared_ptr<sensors::AnalogSignal> phSignal = std::make_shared<sensors::PHSignal<STANDARD_PH_MAVG_LENGTH>>(phReadingStats);

// this certainly could be an array/vect, but using a map to ensure the
// ids stay fixed
const std::map<unsigned int, std::shared_ptr<sensors::AnalogSignal>> PIN_TO_INPUT_SIGNAL = {
    {0, phSignal}
    // { 1, sensors::TDSSignal() },
};

// Web Server
misc_sensors::web_server::ReefPiWebServer webServer(PIN_TO_INPUT_SIGNAL, 80);

/******************
 * Arduino integrations
 *******************/
void setup() {
    Serial.begin(115200);
    Wire.begin(static_cast<int>(inputs::PIN_CONFIG.I2C_SDA), static_cast<int>(inputs::PIN_CONFIG.I2C_SCL), (int) 10000);

    richiev::connectWifi(inputs::hostname, inputs::wifiSSID, inputs::wifiPassword);
    richiev::ota::setupOTA(inputs::hostname);

    setupPH_RoboTankPHBoard();

    webServer.setupWebServer();
}

void loop() {
    auto phReadingPtr = phReader->readNewPHSignalIfTimeAndUpdate<STANDARD_PH_MAVG_LENGTH>(*phReadingStats);
    if (phReadingPtr) {
        Serial.print("ph");
        Serial.print(" asOfMS=");
        Serial.print(phReadingPtr->asOfMS);
        Serial.print(" asOfAdjustedSec=");
        Serial.print(phReadingPtr->asOfAdjustedSec);
        Serial.print(" rawPH=");
        Serial.print(phReadingPtr->rawPH);
        Serial.print(" rawPH_mavg=");
        Serial.print(phReadingPtr->rawPH_mavg);
        Serial.print(" calibratedPH=");
        Serial.print(phReadingPtr->calibratedPH);
        Serial.print(" calibratedPH_mavg=");
        Serial.print(phReadingPtr->calibratedPH_mavg);
        Serial.println();
    }

    richiev::ota::loopOTA();
}

}  // namespace sensor_main

void setup() {
    sensor_main::setup();
}

void loop() {
    sensor_main::loop();
}

#endif