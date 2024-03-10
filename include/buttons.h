#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 33
#define PB_DOWN 35

extern int current_mode;

int wait_for_button_press();
bool go_to_menu();

#endif