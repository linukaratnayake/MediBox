#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#define SERVO_PIN 15

#include <ESP32Servo.h>

extern Servo servo;

void servoInitialize();
void turnServo(int);

#endif