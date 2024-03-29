import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome import pins


from esphome.const import (
    CONF_ADDRESS,
    CONF_ID,
    CONF_SCL,
    CONF_SDA
)

DEPENDENCIES = []
# MULTI_CONF = True

ph_sensor_ns = cg.esphome_ns.namespace('ph_robotank')
PHSensor = ph_sensor_ns.class_('PHSensor', sensor.Sensor, cg.PollingComponent)

# sensor.sensor_schema(
#     ADCMPXSensor,
#     unit_of_measurement=UNIT_VOLT,
#     icon=ICON_EMPTY,
#     accuracy_decimals=2,
#     device_class=DEVICE_CLASS_VOLTAGE
# ).extend(
#     {
#         cv.Required(CONF_SENSOR): cv.use_id(voltage_sampler.VoltageSampler),
#         cv.Required(CONF_PIN): pins.gpio_output_pin_schema
#     }
# ).extend(cv.polling_component_schema("60s"))

# state_class="measurement"
CONFIG_SCHEMA = sensor.sensor_schema(PHSensor, unit_of_measurement='pH', accuracy_decimals=2).extend({
    cv.GenerateID(): cv.declare_id(PHSensor),
    cv.Required(CONF_ADDRESS): cv.positive_int,
    cv.Optional(CONF_SDA, 21): pins.internal_gpio_output_pin_number,
    cv.Optional(CONF_SCL, 22): pins.internal_gpio_output_pin_number,
}).extend(cv.polling_component_schema('60s'))

async def to_code(config):
    var = await sensor.new_sensor(config)
    cg.add_library("Wire", None)
    cg.add(var.set_sda(config[CONF_SDA]))
    cg.add(var.set_scl(config[CONF_SCL]))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    await cg.register_component(var, config)
    # await sensor.register_sensor(var, config)
