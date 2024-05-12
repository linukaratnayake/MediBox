#include <Arduino.h>

#include "servomotor.h"

Servo servo;

void servoInitialize() {
    servo.attach(SERVO_PIN);
}

void turnServo(int angle) {
    servo.write(angle);
}