#include "oled.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayLine(String text, int column, int row, int textSize)
{
    display.setCursor(column, row);
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.println(text);
    display.display();
}