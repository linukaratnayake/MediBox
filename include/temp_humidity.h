#ifndef TEMP_HUMIDITY_H
#define TEMP_HUMIDITY_H

#include <DHTesp.h>
#define DHTPIN 12

extern DHTesp dhtSensor;

void check_temp_humidity();

#endif