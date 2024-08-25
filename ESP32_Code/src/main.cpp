#include <Arduino.h>

#include "oled.h"
#include "buttons.h"
#include "time_alarm.h"
#include "temp_humidity.h"
#include "ldr.h"
#include "servomotor.h"
#include "connectivity.h"

struct datetime date_time;
struct datetime offset = {};

int angleOffset = 30;
float gammaFactor = 0.75;

// Function Declarations
void run_mode(int);
int calculateServoAngle(int, int, int, float);
void slideWindow(int, float);

void setup()
{
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_ALARM, OUTPUT);
  pinMode(LED_WARNING, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DOWN, INPUT);
  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);
  pinMode(SERVO_PIN, OUTPUT);

  dhtSensor.setup(DHTPIN, DHTesp::DHT22);
  servoInitialize();

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("OLED Display Initialization Failed.");
    for (;;)
      ;
  }

  display.clearDisplay();
  displayLine("MediBox", 20, 20, 2);
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

  // Setting up MQTT Connection
  setupMQTT();

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

  if (subscribedAngleOffsetChanged) {
    angleOffset = subscribedAngleOffset;
    subscribedAngleOffsetChanged = false;
  }
  if (subscribedGammaFactorChanged) {
    gammaFactor = subscribedGammaFactor;
    subscribedGammaFactorChanged = false;
  }
  slideWindow(angleOffset, gammaFactor);

  // Communication with the NodeRED Dashboard

  if (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker...");
    connectToBroker();
  }

  mqttClient.loop();

  mqtt_publish("2853_MEDIBOX_LDR_LEFT", getIntensity(LDR_LEFT));
  mqtt_publish("2853_MEDIBOX_LDR_RIGHT", getIntensity(LDR_RIGHT));
}

void run_mode(int mode)
{
  if (mode == 0)
  {
    set_utc_offset(&offset);
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

int calculateServoAngle(int leftLDR, int rightLDR, int offset, float gamma) {
  float D;
  if (leftLDR >= rightLDR) D = 1.5;
  else D = 0.5;

  float maxValue = 4063.0;
  float I = max(leftLDR, rightLDR) / maxValue;

  int angle = min(offset * D + (180 - offset) * I * gamma, (float) 180);

  return angle;
}

void slideWindow(int offset, float gamma) {
  int leftValue = getIntensity(LDR_LEFT);
  int rightValue = getIntensity(LDR_RIGHT);

  int angle = calculateServoAngle(leftValue, rightValue, offset, gamma);

  turnServo(angle);

  // The following part shows jitter in the simulation.

  // // Turn servo motor slowly.
  // int currentAngle = max(min(getCurrentAngle(), 180), 0);
  // Serial.print("Current Angle: ");
  // Serial.println(currentAngle);

  // if (angle > currentAngle) {
  //   for (int i = currentAngle; i <= angle; i++) {
  //     turnServo(i);
  //     delay(10);
  //   }
  // } else if (angle < currentAngle) {
  //   for (int i = currentAngle; i >= angle; i--) {
  //     turnServo(i);
  //     delay(10);
  //   }
  // }
}