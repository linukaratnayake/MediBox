#include "Arduino.h"

#include "ldr.h"

int getIntensity(int ldr) {
    int intensity = 0;

    if (ldr == LDR_LEFT) {
        intensity = analogRead(LDR_LEFT);
    } else if (ldr == LDR_RIGHT) {
        intensity = analogRead(LDR_RIGHT);
    }

    return intensity;
}