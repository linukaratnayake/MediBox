#include <Arduino.h>

#include "buttons.h"
#include "oled.h"

int max_modes = 5;
String modes[] = {"1 - Set UTC Offset", "2 - Set Alarm 1", "3 - Set Alarm 2", "4 - Set Alarm 3", "5 - Disable Alarms"};

int current_mode = 0;

int wait_for_button_press()
{
    unsigned long int time = millis();

    while (millis() - time <= 5000) // Times-out in 5 seconds.
    {
        if (digitalRead(PB_UP) == LOW)
        {
            delay(200);
            return PB_UP;
        }
        else if (digitalRead(PB_DOWN) == LOW)
        {
            delay(200);
            return PB_DOWN;
        }
        else if (digitalRead(PB_CANCEL) == LOW)
        {
            delay(200);
            return PB_CANCEL;
        }
        else if (digitalRead(PB_OK) == LOW)
        {
            delay(200);
            return PB_OK;
        }
    }
    return PB_CANCEL;
}

bool go_to_menu()
{
    int temp_mode = current_mode;

    unsigned long int time = millis();

    // Time-outs after 5 seconds.
    // This is important for alarms and warnings to work,
    // if the user leaves the device unattended in the 'menu'.
    while (digitalRead(PB_CANCEL) == HIGH && millis() - time <= 5000)
    {
        display.clearDisplay();
        displayLine(modes[temp_mode], 0, 0, 2);

        int pressed = wait_for_button_press();
        time = millis(); // Refreshes time for another 5 seconds.

        if (pressed == PB_UP)
        {
            delay(200);
            temp_mode += 1;
            temp_mode = temp_mode % max_modes;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_mode -= 1;
            if (temp_mode < 0)
            {
                temp_mode = max_modes - 1;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            current_mode = temp_mode;
            return true;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }
    return false;
}