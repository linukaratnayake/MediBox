#include "Arduino.h"

#include "ldr.h"

int getIntensity(int ldr) {
    int intensity = 0;
    int maxValue  = 4063;

    // LDR output has an inverse relationship with the light intensity.
    
    if (ldr == LDR_LEFT) {
        intensity = maxValue - analogRead(LDR_LEFT);
    } else if (ldr == LDR_RIGHT) {
        // Serial.println("Reading RIGHT");
        intensity = maxValue - analogRead(LDR_RIGHT);
    }

    return intensity;
}