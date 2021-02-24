#include "safelight.h"

// create an array for led state and a safelightBrightness variable
CRGB leds[LED_COUNT];
int safelightBrightness;

void setupSafelight() {
    safelightBrightness = 0;
    // add leds
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    // clear and update the leds
    FastLED.clear();
    FastLED.show();
}

void updateLEDs() {
    // for each led in the strip, incrementing by our skip value
    for (int l = 0; l < LED_COUNT; l += LED_SKIP) {
        // set the led to the specified red safelightBrightness
        leds[l] = CRGB(safelightBrightness, 0, 0);
    }
    // update the leds
    FastLED.show();
};

void safelightStartupAnimation() {
    safelightBrightness = 0;
    // calculate the duration of a single safelightBrightness step for the startup animation
    int stepDuration = SAFELIGHT_STARTUP_DURATION / SAFELIGHT_MAX_BRIGHTNESS;
    // increment by our calculated step length until we hit the specified duration
    for (int i = 0; i < SAFELIGHT_STARTUP_DURATION; i += stepDuration) {
        // increment safelightBrightness by 1 if we're still below the max safelightBrightness
        if (safelightBrightness < SAFELIGHT_MAX_BRIGHTNESS) safelightBrightness++;
        // update the leds to the new safelightBrightness
        updateLEDs();
        // delay for the step duration
        delay(stepDuration);
    }
}

void safelightOff() {
    FastLED.clear();
    FastLED.show();
};

void safelightOn() {
    updateLEDs();
};

void increaseSafelightBrightness() {
    if (safelightBrightness < SAFELIGHT_MAX_BRIGHTNESS) safelightBrightness++;
    updateLEDs();
};

void decreaseSafelightBrightness() {
    if (safelightBrightness > SAFELIGHT_MIN_BRIGHTNESS) safelightBrightness--;
    updateLEDs();
};