import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from esphome.const import (
    CONF_ADDRESS,
    CONF_ID,
    CONF_INTERVAL
)

DEPENDENCIES = []
MULTI_CONF = True

ph_sensor_ns = cg.esphome_ns.namespace('ph_robotank')
PHSensor = ph_sensor_ns.class_('PHSensor', cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema(PHSensor, unit_of_measurement='pH', accuracy_decimals=2).extend({
    cv.GenerateID(): cv.declare_id(PHSensor),
    cv.Required(CONF_ADDRESS): cv.positive_int,
}).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config.get(CONF_INTERVAL), config[CONF_ADDRESS])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
