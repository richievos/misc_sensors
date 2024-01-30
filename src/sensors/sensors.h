#pragma once

namespace sensors {

class AnalogSignal {
   public:
    virtual float readFloat() = 0;
};

#include <Arduino.h>

// logic based on https://wiki.keyestudio.com/KS0429_keyestudio_TDS_Meter_V1.0
// ugly implementation, but going with it for starters
class TDSSignal : public AnalogSignal {
   private:
    const short _tdsInputPin;

    static int getMedianNum(uint16_t bArray[], int iFilterLen) {
        uint16_t bTab[iFilterLen];
        for (byte i = 0; i < iFilterLen; i++)
            bTab[i] = bArray[i];
        uint16_t bTemp;
        for (int j = 0; j < iFilterLen - 1; j++) {
            for (int i = 0; i < iFilterLen - j - 1; i++) {
                if (bTab[i] > bTab[i + 1]) {
                    bTemp = bTab[i];
                    bTab[i] = bTab[i + 1];
                    bTab[i + 1] = bTemp;
                }
            }
        }
        if ((iFilterLen & 1) > 0)
            bTemp = bTab[(iFilterLen - 1) / 2];
        else
            bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
        return bTemp;
    }

   public:
    TDSSignal(const short tdsInputPin) : _tdsInputPin(tdsInputPin) {}

    // TODO: should do the same buffering as ph
    float readFloat() override {
        const float VREF = 5.0;
        int count = 40;
        int delayTime = 50;

        uint16_t readings[count];
        for (int i = 0; i < count; i++) {
            readings[i] = analogRead(_tdsInputPin);
            delay(delayTime);
        }
        float averageVoltage = getMedianNum(readings, sizeof(readings)) * VREF / 1024.0;

        float temperature = 25.0;
        float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);                                     // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVoltage = averageVoltage / compensationCoefficient;                                  // temperature compensation
        auto tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage              //
                         - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage)  //
                        * 0.5;                                                                                 // convert voltage value to tds value

        Serial.print("TDS Value: ");
        Serial.print(tdsValue, 0);
        Serial.print("ppm");
        Serial.print(", averageVoltage= ");
        Serial.print(averageVoltage);
        Serial.println();

        return tdsValue;
    }
};

#include "readings/ph-controller.h"

template <size_t PH_READ_LENGTH>
class PHSignal : public AnalogSignal {
   private:
    std::shared_ptr<buff::ph::controller::PHReadingStats<PH_READ_LENGTH>> _phReadingStats;

   public:
    PHSignal(std::shared_ptr<buff::ph::controller::PHReadingStats<PH_READ_LENGTH>> phReadingStats) : _phReadingStats(phReadingStats) {}

    float readFloat() override {
        auto phValue = _phReadingStats->mostRecentReading();
        return phValue.calibratedPH_mavg;
    }
};

}  // namespace sensors
