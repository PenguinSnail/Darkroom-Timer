#include <Arduino.h>

#include "pins.h"
#include "display/display.h"
#include "safelight/safelight.h"

#include "modes/run.h"
#include "modes/set.h"
#include "modes/bright.h"

void setup() {
    // setup the safelight and run the startup animation
    setupSafelight();
    setupDisplay();
    displayStartupAnimation();
    safelightStartupAnimation();
}

void loop() {
    
}