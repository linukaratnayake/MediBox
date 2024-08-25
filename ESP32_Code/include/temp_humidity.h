#ifndef TEMP_HUMIDITY_H
#define TEMP_HUMIDITY_H

#include <DHTesp.h>
#define DHTPIN 12
#define LED_WARNING 2

extern DHTesp dhtSensor;

void check_temp_humidity();

#endif