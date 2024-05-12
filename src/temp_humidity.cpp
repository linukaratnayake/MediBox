#include "temp_humidity.h"
#include "oled.h"
#include "connectivity.h"

const int TEMP_MAX = 32;
const int TEMP_MIN = 26;
const int HUMD_MAX = 80;
const int HUMD_MIN = 60;

DHTesp dhtSensor;

void check_temp_humidity()
{
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    bool warning = false;

    if (data.temperature >= TEMP_MAX)
    {
        display.clearDisplay();
        displayLine("TEMP HIGH", 10, 40, 1);
        warning = true;
    }
    else if (data.temperature <= TEMP_MIN)
    {
        display.clearDisplay();
        displayLine("TEMP LOW", 10, 40, 1);
        warning = true;
    }
    else
    {
        warning = false;
    }

    if (data.humidity >= HUMD_MAX)
    {
        display.clearDisplay();
        displayLine("HUMIDITY HIGH", 10, 50, 1);
        warning = true;
    }
    else if (data.humidity <= HUMD_MIN)
    {
        display.clearDisplay();
        displayLine("HUMIDITY LOW", 10, 50, 1);
        warning = true;
    }
    else
    {
        warning = false;
    }

    if (warning)
    {
        digitalWrite(LED_WARNING, HIGH);
    }
    else
    {
        digitalWrite(LED_WARNING, LOW);
    }

    // Publishing temperature and humidity to the broker.
    mqtt_publish("2853_MEDIBOX_TEMP", data.temperature);
    mqtt_publish("2853_MEDIBOX_HUMIDITY", data.humidity);

    delay(200);
}