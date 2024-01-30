#ifdef REEF_PI_TDS
#include <Arduino.h>
#include <Wire.h>

#include <map>

#include "mywifi.h"
#include "ota.h"
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
const short TDS_INPUT_PIN = GPIO_NUM_32;

std::shared_ptr<sensors::AnalogSignal> tdsSignal = std::make_shared<sensors::TDSSignal>(TDS_INPUT_PIN);

// this certainly could be an array/vect, but using a map to ensure the
// ids stay fixed
const std::map<unsigned int, std::shared_ptr<sensors::AnalogSignal>> PIN_TO_INPUT_SIGNAL = {
    // {0, phSignal}
    { 1, tdsSignal },
};

// Web Server
misc_sensors::web_server::ReefPiWebServer webServer(PIN_TO_INPUT_SIGNAL, 80);

/******************
 * Arduino integrations
 *******************/
void setup() {
    Serial.begin(115200);
    Wire.begin(inputs::PIN_CONFIG.I2C_SDA, inputs::PIN_CONFIG.I2C_SCL);

    richiev::connectWifi(inputs::hostname, inputs::wifiSSID, inputs::wifiPassword);
    richiev::ota::setupOTA(inputs::hostname);

    webServer.setupWebServer();

    pinMode(TDS_INPUT_PIN, INPUT);
}

void loop() {
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