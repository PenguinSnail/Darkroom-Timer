#include "safelight.h"

// create an array for led state and a brightness variable
CRGB leds[LED_COUNT];
int brightness;

void setupSafelight() {
    brightness = 0;
    // add leds
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    // clear and update the leds
    FastLED.clear();
    FastLED.show();
}

void startupAnimation() {
    brightness = 0;
    // calculate the duration of a single brightness step for the startup animation
    int stepDuration = STARTUP_DURATION / MAX_BRIGHTNESS;
    // increment by our calculated step length until we hit the specified duration
    for (int i = 0; i < STARTUP_DURATION; i += stepDuration) {
        // increment brightness by 1 if we're still below the max brightness
        if (brightness < MAX_BRIGHTNESS) brightness++;
        // for each led in the strip, incrementing by our skip value
        for (int l = 0; l < LED_COUNT; l += LED_SKIP) {
            // set the led to the specified red brightness
            leds[l] = CRGB(brightness, 0, 0);
        }
        // update the leds
        FastLED.show();
        // delay for the step duration
        delay(stepDuration);
    }
}

void safelightOff() {
    FastLED.clear();
    FastLED.show();
};

void safelightOn() {
    for (int l = 0; l < LED_COUNT; l += LED_SKIP) {
            leds[l] = CRGB(brightness, 0, 0);
        }
        FastLED.show();
};