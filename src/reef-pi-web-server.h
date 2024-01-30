#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <string>

#include "readings/ph-common.h"
#include "readings/ph-controller.h"
#include "readings/ph.h"
#include "sensors/sensors.h"

namespace misc_sensors {
namespace web_server {
using namespace buff;

class ReefPiWebServer {
   private:
    AsyncWebServer _server;
    const std::map<unsigned int, std::shared_ptr<sensors::AnalogSignal>> _signals;

   public:
    ReefPiWebServer(const std::map<unsigned int, std::shared_ptr<sensors::AnalogSignal>> signals, int port = 80) : _signals(signals), _server(port) {}

    void handlePHRead(AsyncWebServerRequest *request) {
        String inputNumberStr = request->pathArg(0);
        unsigned int inputNumber = inputNumberStr.toInt();

        if(_signals.count(inputNumber) == 0) {
            request->send(400, "text/plain", "Invalid input number");
            return;
        }

        auto phValue = _signals.at(inputNumber)->readFloat();

        // TODO: should not return a value if we just rebooted and haven't captured enough readings yet
        if(std::isnan(phValue)) {
            request->send(503, "text/plain", "Sensor reading not available yet");
            return;
        }

        char tmp[42];
        snprintf(tmp, sizeof(tmp), "%.1f\n", phValue);

        request->send(200, "text/plain", tmp);
    }

    void handleNotFound(AsyncWebServerRequest *request) {
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += request->url();
        message += "\nMethod: ";
        message += (request->method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += request->args();
        message += "\n";

        for (int i = 0; i < request->args(); i++) {
            message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
        }

        request->send(404, "text/plain", message);
    }

    void setupWebServer() {
        _server.on("^\\/analog_inputs\\/(\\d+)$", HTTP_GET, [&](AsyncWebServerRequest *request){ handlePHRead(request); });
        _server.onNotFound([&](AsyncWebServerRequest *request){ handleNotFound(request); });
        _server.begin();
        Serial.println("HTTP server started");
    }
};

}  // namespace web_server
}  // namespace misc_sensors