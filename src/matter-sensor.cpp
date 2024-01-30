#ifdef MATTER_ARDUINO_SENSOR
#include <Arduino.h>

#include "Matter-include.h"
// must be after the matter include
#include <app/server/OnboardingCodesUtil.h>
using namespace chip;
using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

// Debounce for toggle button
const int DEBOUNCE_DELAY = 5000;
int last_toggle;

// Cluster and attribute ID used by Matter plugin unit device
const uint32_t CLUSTER_ID = TemperatureMeasurement::Id;                               // OnOff::Id;
const uint32_t ATTRIBUTE_ID = TemperatureMeasurement::Attributes::MeasuredValue::Id;  // OnOff::Attributes::OnOff::Id;

// Endpoint and attribute ref that will be assigned to Matter device
uint16_t plugin_unit_endpoint_id_1 = 0;
attribute_t *attribute_ref_1;

// There is possibility to listen for various device events, related for example
// to setup process. Leaved as empty for simplicity.
static void on_device_event(const ChipDeviceEvent *event, intptr_t arg) {}
static esp_err_t on_identification(identification::callback_type_t type,
                                   uint16_t endpoint_id, uint8_t effect_id,
                                   uint8_t effect_variant, void *priv_data) {
    return ESP_OK;
}

// Listener on attribute update requests.
// In this example, when update is requested, path (endpoint, cluster and
// attribute) is checked if it matches plugin unit attribute. If yes, LED changes
// state to new one.
static esp_err_t on_attribute_update(attribute::callback_type_t type,
                                     uint16_t endpoint_id, uint32_t cluster_id,
                                     uint32_t attribute_id,
                                     esp_matter_attr_val_t *val,
                                     void *priv_data) {
    // if (type == attribute::PRE_UPDATE && cluster_id == CLUSTER_ID && attribute_id == ATTRIBUTE_ID) {
    //   // We got an plugin unit on/off attribute update!
    //   boolean new_state = val->val.b;
    //   if (endpoint_id == plugin_unit_endpoint_id_1) {
    //     digitalWrite(LED_PIN_1, new_state);
    //   } else if (endpoint_id == plugin_unit_endpoint_id_2) {
    //     digitalWrite(LED_PIN_2, new_state);
    //   }
    // }
    return ESP_OK;
}


// Represents the temperature in degrees Celsius as follows:
// MeasuredValue = 100 x temperature [°C]
// Where -273.15°C ≤ temperature ≤ 327.67°C, with a resolution of 0.01°C.
// The null value indicates that the temperature is unknown.
// https://csa-iot.org/wp-content/uploads/2022/11/22-27350-001_Matter-1.0-Application-Cluster-Specification.pdf
uint16_t readingToTemp(uint16_t temp) {
    return temp * 100;
}

const int16_t MAX_TDS = readingToTemp(2000);
const int16_t DEFAULT_TDS = MAX_TDS;
const int16_t MIN_TDS = 0;

#include "monitoring-display.h"

void setup() {
    Serial.begin(115200);

    // Enable debug logging
    esp_log_level_set("*", ESP_LOG_DEBUG);

    // Setup Matter node
    node::config_t node_config;
    node_t *node =
        node::create(&node_config, on_attribute_update, on_identification);

    // Setup Plugin unit endpoint / cluster / attributes with default values
    temperature_sensor::config_t temp_config;
    temp_config.temperature_measurement.min_measured_value = (nullable<int16_t>)MIN_TDS;
    temp_config.temperature_measurement.max_measured_value = (nullable<int16_t>)MAX_TDS;
    temp_config.temperature_measurement.measured_value = (nullable<int16_t>)DEFAULT_TDS;
    endpoint_t *endpoint_1 = temperature_sensor::create(node, &temp_config,
                                                        ENDPOINT_FLAG_NONE, NULL);

    attribute_ref_1 =
        attribute::get(cluster::get(endpoint_1, CLUSTER_ID), ATTRIBUTE_ID);

    // Save generated endpoint id
    plugin_unit_endpoint_id_1 = endpoint::get_id(endpoint_1);

    // Start Matter device
    esp_matter::start(on_device_event);

    // Print codes needed to setup Matter device
    PrintOnboardingCodes(
        chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
    
    monitoring_display::setupDisplay();
}

esp_matter_attr_val_t get_tds_value(esp_matter::attribute_t *attribute_ref) {
    esp_matter_attr_val_t tds_value = esp_matter_invalid(NULL);
    attribute::get_val(attribute_ref, &tds_value);
    return tds_value;
}

void set_tds_attribute_value(esp_matter_attr_val_t *tds_value, uint16_t plugin_unit_endpoint_id) {
    // attribute::update(plugin_unit_endpoint_id, CLUSTER_ID, ATTRIBUTE_ID, tds_value);
    attribute::update(plugin_unit_endpoint_id, CLUSTER_ID, ATTRIBUTE_ID, tds_value);
}

struct Pins {
    uint8_t tdsInput1 = 32;
} pinConfig;

uint16_t lastTDSValue = 0;


// When toggle plugin unit button is pressed (with debouncing),
// plugin unit attribute value is changed
void loop() {
    auto asOf = millis();
    if ((asOf - last_toggle) > DEBOUNCE_DELAY) {
        auto tdsValue = analogRead(pinConfig.tdsInput1);

        if (lastTDSValue != tdsValue) {
            Serial.print("New tdsValue=");
            Serial.println(tdsValue);
            last_toggle = asOf;
            esp_matter_attr_val_t tds_value = get_tds_value(attribute_ref_1);
            tds_value.val.i16 = readingToTemp(
                std::max<int16_t>(
                    std::min<int16_t>(tdsValue, MAX_TDS),
                    MIN_TDS));

            set_tds_attribute_value(&tds_value, plugin_unit_endpoint_id_1);

            monitoring_display::displayTDS(asOf, tdsValue);
        }

        lastTDSValue = tdsValue;
    }
}

#endif
