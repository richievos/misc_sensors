#ifdef MKS_TEST
#include <Arduino.h>

struct {
    short X_STEP_PIN = 12;
    short X_DIRECTION_PIN = 14;
    short Y_STEP_PIN = 26;
    short Y_DIRECTION_PIN = 15;
    short Z_STEP_PIN = 27;
    short Z_DIRECTION_PIN = 33;
    short X_LIMIT_PIN = 17;
    short Y_LIMIT_PIN = 4;
    short Z_LIMIT_PIN = 16;

    // OK to comment out to use pin for other features
    short STEPPERS_DISABLE_PIN = 13;

    // short SPINDLE_TYPE = SpindleType::PWM;
    short SPINDLE_OUTPUT_PIN = 2;   // labeled SpinPWM
    short SPINDLE_ENABLE_PIN = 22;  // labeled SpinEnbl

    short COOLANT_MIST_PIN = 21;   // labeled Mist
    short COOLANT_FLOOD_PIN = 25;  // labeled Flood
    short PROBE_PIN = 32;          // labeled Probe
} MKS_PINS;


void setup() {
    ledcSetup(_pwm_chan_num, (double)_pwm_freq, _pwm_precision);  // setup the channel
    ledcAttachPin(_output_pin, _pwm_chan_num);                    // attach the PWM to the pin
}

void loop() {

}
#endif