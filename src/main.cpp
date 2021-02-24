#include <Arduino.h>

#include "pins.h"
#include "display/display.h"
#include "safelight/safelight.h"

void setup() {
    // setup the safelight and run the startup animation
    setupSafelight();
    startupAnimation();
}

void loop() {
    
}