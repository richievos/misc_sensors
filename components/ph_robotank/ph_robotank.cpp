#include "ph_robotank.h"

#include <Wire.h>

#include "esphome/core/log.h"

namespace esphome {
namespace ph_robotank {

// TODO: figure out how to avoid copy-pasting this file
// read the current ph directly from the board
float esphome_readPHSignal_RoboTankPHBoard(const uint8_t i2cAddress) {
    ::Wire.beginTransmission(i2cAddress);  // start transmission
    ::Wire.write("R");                     // ask for pH
    ::Wire.write(0);                       // send closing byte
    ::Wire.endTransmission();              // end transmission

    uint8_t count = 0;  // count imcoming bytes
    uint8_t byteCountToRead = 8;
    String i = "";                                    // create string of incoming data
    ::Wire.requestFrom(i2cAddress, byteCountToRead);  // request 8 bytes from pH circuit
    while (::Wire.available())                        // read the 8 incoming bytes
    {
        char c = Wire.read();  // receive a byte as character
        if (count > 0) {
            i = i + c;
        }         // ignore first btye and combine remaining into a string
        count++;  // count byte
    }
    float pH = i.toFloat();  // convert pH string to float
    return pH;               // return pH
}

void PHSensor::setup() {
    ESP_LOGI("ph_robotank", "Setting up with sda=%i, scl=%i", _sda, _scl);

    ::Wire.begin(static_cast<int>(_sda), static_cast<int>(_scl), (int)10000);
    // setupPH_RoboTankPHBoard();
    ::Wire.setClock(10000);
    // ::Wire.setTimeout(500); // milliseconds

    _hasBeenSetup = true;
}

void PHSensor::update() {
    ESP_LOGD("ph_robotank", "Reading ph with sda=%i, scl=%i, address=%i", _sda, _scl, _i2cAddress);
    const auto ph = esphome_readPHSignal_RoboTankPHBoard(_i2cAddress);
    ESP_LOGD("ph_robotank", "Read ph=%f", ph);

    publish_state(ph);
}

void PHSensor::set_address(uint8_t address) { _i2cAddress = address; }
void PHSensor::set_sda(uint8_t sda) { _sda = sda; }
void PHSensor::set_scl(uint8_t scl) { _scl = scl; }

}  // namespace ph_robotank

}  // namespace esphome
