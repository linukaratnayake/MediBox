#include "time_alarm.h"
#include "oled.h"
#include "buttons.h"

const int n_alarms = 3;
int alarm_hours[n_alarms] = {};   // Initialize to 0.
int alarm_minutes[n_alarms] = {}; // Initialize to 0.
bool alarm_triggered[n_alarms];

const int C = 262;
const int D = 294;
const int E = 330;
const int F = 349;
const int G = 392;
const int A = 440;
const int B = 494;
const int C_H = 523;
const int notes[] = {C, D, E, F, G, A, B, C_H};
int n_notes = 8;

long int UTC_OFFSET = 0; // In seconds
bool alarm_enabled = true;

void print_time_now(struct datetime *dt)
{
    display.clearDisplay();
    displayLine(String(dt->days), 0, 0, 2);
    displayLine(":", 25, 0, 2);
    displayLine(String(dt->hours), 35, 0, 2);
    displayLine(":", 55, 0, 2);
    displayLine(String(dt->minutes), 65, 0, 2);
    displayLine(":", 85, 0, 2);
    displayLine(String(dt->seconds), 95, 0, 2);
}

void set_utc_offset(struct datetime *offset)
{
    int temp_hour = offset->hours;
    while (true)
    {
        display.clearDisplay();
        displayLine("Enter offset hours: " + String(temp_hour), 0, 0, 2);

        int pressed = wait_for_button_press();
        if (pressed == PB_UP)
        {
            delay(200);
            temp_hour += 1;
            temp_hour = temp_hour % 24;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_hour -= 1;
            if (temp_hour < 0)
            {
                temp_hour = 23;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            offset->hours = temp_hour;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    int temp_minute = offset->minutes;
    while (true)
    {
        display.clearDisplay();
        displayLine("Enter offset minutes: " + String(temp_minute), 0, 0, 2);

        int pressed = wait_for_button_press();
        if (pressed == PB_UP)
        {
            delay(200);
            temp_minute += 1;
            temp_minute = temp_minute % 60;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_minute -= 1;
            if (temp_minute < 0)
            {
                temp_minute = 59;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            offset->minutes = temp_minute;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }
    display.clearDisplay();
    displayLine("Offset is set to " + String(temp_hour) + ":" + String(temp_minute), 0, 0, 2);
    delay(500);
}

void update_time(struct datetime *date_time)
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);

    char timeHour[3];
    strftime(timeHour, 3, "%H", &timeinfo);
    date_time->hours = atoi(timeHour);

    char timeMinute[3];
    strftime(timeMinute, 3, "%M", &timeinfo);
    date_time->minutes = atoi(timeMinute);

    char timeSecond[3];
    strftime(timeSecond, 3, "%S", &timeinfo);
    date_time->seconds = atoi(timeSecond);

    char timeDay[3];
    strftime(timeDay, 3, "%d", &timeinfo);
    date_time->days = atoi(timeDay);
}

void set_alarm(int alarm)
{
    unsigned long int time = millis();

    int temp_hour = alarm_hours[alarm];
    while (millis() - time <= 5000) // Times-out in 5 seconds
    {
        display.clearDisplay();
        displayLine("Enter hour: " + String(temp_hour), 0, 0, 2);

        int pressed = wait_for_button_press();
        time = millis(); // Refreshes time for another 5 seconds.

        if (pressed == PB_UP)
        {
            delay(200);
            temp_hour += 1;
            temp_hour = temp_hour % 24;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_hour -= 1;
            if (temp_hour < 0)
            {
                temp_hour = 23;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            alarm_hours[alarm] = temp_hour;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    time = millis();
    int temp_minute = alarm_minutes[alarm];
    while (millis() - time <= 5000) // Times-out in 5 seconds
    {
        display.clearDisplay();
        displayLine("Enter minute: " + String(temp_minute), 0, 0, 2);

        int pressed = wait_for_button_press();
        time = millis(); // Refreshes time for another 5 seconds.

        if (pressed == PB_UP)
        {
            delay(200);
            temp_minute += 1;
            temp_minute = temp_minute % 60;
        }
        else if (pressed == PB_DOWN)
        {
            delay(200);
            temp_minute -= 1;
            if (temp_minute < 0)
            {
                temp_minute = 59;
            }
        }
        else if (pressed == PB_OK)
        {
            delay(200);
            alarm_minutes[alarm] = temp_minute;
            break;
        }
        else if (pressed == PB_CANCEL)
        {
            delay(200);
            break;
        }
    }

    // Setting back the new alarm to not triggered yet.
    alarm_triggered[alarm] = false;

    display.clearDisplay();
    displayLine("Alarm " + String(alarm) + " is set to " + String(temp_hour) + ":" + String(temp_minute), 0, 0, 2);
    delay(500);
}

void update_time_with_check_alarm(struct datetime *date_time)
{
    update_time(date_time);
    print_time_now(date_time);

    if (alarm_enabled == true)
    {
        for (int i = 0; i < n_alarms; i++)
        {
            if (alarm_triggered[i] == false && alarm_hours[i] == date_time->hours && alarm_minutes[i] == date_time->minutes)
            {
                ring_alarm();
                alarm_triggered[i] = true;
            }
        }
    }
}

void ring_alarm()
{
    display.clearDisplay();
    displayLine("MEDICINE TIME!", 0, 0, 2);

    digitalWrite(LED_ALARM, HIGH);

    bool break_happened = false;

    while (break_happened == false && digitalRead(PB_CANCEL) == HIGH)
    {
        for (int i = 0; i < n_notes; i++)
        {
            if (digitalRead(PB_CANCEL) == LOW)
            {
                delay(200);
                break_happened = true;
                break;
            }
            tone(BUZZER, notes[i]);
            delay(500);
            noTone(BUZZER);
            delay(2);
        }
    }

    digitalWrite(LED_ALARM, LOW);
    display.clearDisplay();
}