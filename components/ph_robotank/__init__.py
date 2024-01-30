import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

DEPENDENCIES = []
MULTI_CONF = True

ph_sensor_ns = cg.esphome_ns.namespace('ph_sensor')
PHSensor = ph_sensor_ns.class_('PHSensor', cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema(PHSensor, cg.uint32_t, unit_of_measurement='pH', accuracy_decimals=2).extend({
    cv.GenerateID(): cv.declare_id(PHSensor),
}).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config.get(CONF_INTERVAL))
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
