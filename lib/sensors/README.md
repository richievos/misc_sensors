# CO2 / Humidity / Temperature (SCD30)

Example components:
* [SCD30 Air Quality Sensors Module for CO2 and RH/T Measurements I2C Modbus PWM](https://www.aliexpress.us/item/3256804350040376.html?spm=a2g0o.order_list.order_list_main.34.56d918029Cx8fN&gatewayAdapt=glo2usa&_randl_shipto=US) ($20 + shipping)
* [CO₂ Humidity and Temperature Sensor - SCD30](https://www.sparkfun.com/products/15112)
* [Docs](https://sensirion.com/products/catalog/SCD30/)

Pins:
* Vin -- 3.3V to 5.5V
* Gnd
* TX/SCL
* RX/SDA
* RDY - Data ready pin. High when data is ready for read-out
* PWM - PWM output of CO2 concentration measurement
* SEL - Interface select pin. Pull to VDD (do not exceed 4V, use voltage divider in case your VDD is >4V) for selecting Modbus, leave floating or connect to GND for selecting I2C.

# TDS

Example components: 
* [TDS Sensor Water Conductivity Sensor for Arduino Liquid Detection Water Quality Monitoring Module DIY TDS Online Monitor](https://www.aliexpress.us/item/3256801512027885.html?spm=a2g0o.order_list.order_list_main.35.56d91802LRd8k2&gatewayAdapt=glo2usa&_randl_shipto=US) ($4.50 + shipping)
* [Gravity: Analog TDS Sensor/ Meter for Arduino](https://www.dfrobot.com/product-1662.html) ($12 + shipping)

Pins:
* Ground
* +5V
* Analog In
