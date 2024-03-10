#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definitions of OLED Display parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c

int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

bool alarm_enabled = true;
const int n_alarms = 3;
int alarm_hours[n_alarms] = {};
int alarm_minutes[n_alarms] = {};
bool alarm_triggered[n_alarms] = {};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function Declarations
void displayLine(String, int, int, int);

void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("OLED Display Initialization Failed.");
    for (;;)
      ;
  }
}

void loop()
{
}

void displayLine(String text, int column, int row, int textSize)
{
  display.setCursor(column, row);
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.println(text);
  display.display();
}