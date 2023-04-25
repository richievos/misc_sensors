#ifdef BASIC
#include <Arduino.h>

#include "Matter-include.h"


struct Pins {
  uint8_t tdsInput1 = 36;
} pinConfig;

void setup() {
  Serial.begin(115200);
}

unsigned long lastSlice = 0;
const unsigned long SLICE_INTERVAL = 1000;

void loop() {
  auto tdsValue = analogRead(pinConfig.tdsInput1);

  const unsigned long slice = millis() / SLICE_INTERVAL;

  if (slice != lastSlice) {
    Serial.print("tdsReadValue=");
    Serial.println(tdsValue);
  }

  lastSlice = slice;
}

#endif