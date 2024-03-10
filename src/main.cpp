#include <Arduino.h>

#include "oled.h"
#include "buttons.h"
#include "time_alarm.h"
#include "temp_humidity.h"

struct datetime date_time;
struct datetime offset;

// Function Declarations
void run_mode(int);

void setup()
{
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_ALARM, OUTPUT);
  pinMode(LED_WARNING, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DOWN, INPUT);

  dhtSensor.setup(DHTPIN, DHTesp::DHT22);

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("OLED Display Initialization Failed.");
    for (;;)
      ;
  }

  display.clearDisplay();
  displayLine("MediBox", 10, 20, 2);
  delay(500);
  display.clearDisplay();

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    display.clearDisplay();
    displayLine("Connecting to Wi-Fi...", 0, 0, 1);
  }
  display.clearDisplay();
  displayLine("Connected to Wi-Fi", 0, 0, 1);
  delay(500);

  // Initialize the `alarm_triggered` array to `true` in the beginning,
  // so that alarms will not trigger at unwanted times.
  for (int i = 0; i < n_alarms; i++)
  {
    alarm_triggered[i] = true;
  }

  // Initially, the user is required to update their time zone.
  run_mode(0);
}

void loop()
{
  update_time_with_check_alarm(&date_time);

  if (digitalRead(PB_OK) == LOW)
  {
    delay(200);
    bool mode_changed = go_to_menu();
    if (mode_changed)
    {
      run_mode(current_mode);
    }
  }

  check_temp_humidity();
}

void run_mode(int mode)
{
  if (mode == 0)
  {
    offset = set_utc_offset();
    UTC_OFFSET = 3600 * offset.hours + 60 * offset.minutes;
    configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  }

  else if (mode == 1 || mode == 2 || mode == 3)
  {
    set_alarm(mode - 1);
  }

  else if (mode == 4)
  {
    alarm_enabled = false;
    display.clearDisplay();
    displayLine("All alarms are disabled.", 0, 0, 2);
    delay(500);
  }
}