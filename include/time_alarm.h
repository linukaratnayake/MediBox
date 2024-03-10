#ifndef TIME_ALARM_H
#define TIME_ALARM

#include <WiFi.h>

#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_DST 0

#define BUZZER 23
#define LED_ALARM 4

extern const int n_alarms;
extern long int UTC_OFFSET; // In seconds
extern bool alarm_enabled;
extern bool alarm_triggered[];

struct datetime
{
    int days = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
};

void print_time_now(struct datetime *);
struct datetime set_utc_offset();
void update_time(struct datetime *);
void set_alarm(int);
void update_time_with_check_alarm(struct datetime *);
void ring_alarm();

#endif