#include "temp_humidity.h"
#include "oled.h"

const int TEMP_MAX = 32;
const int TEMP_MIN = 25;
const int HUMD_MAX = 80;
const int HUMD_MIN = 60;

DHTesp dhtSensor;

void check_temp_humidity()
{
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    if (data.temperature >= TEMP_MAX)
    {
        display.clearDisplay();
        displayLine("TEMP HIGH", 0, 40, 1);
        delay(200);
    }
    else if (data.temperature <= TEMP_MIN)
    {
        display.clearDisplay();
        displayLine("TEMP LOW", 0, 40, 1);
        delay(200);
    }
    if (data.humidity >= HUMD_MAX)
    {
        display.clearDisplay();
        displayLine("HUMIDITY HIGH", 0, 50, 1);
        delay(200);
    }
    else if (data.humidity <= HUMD_MIN)
    {
        display.clearDisplay();
        displayLine("HUMIDITY LOW", 0, 50, 1);
        delay(200);
    }
}