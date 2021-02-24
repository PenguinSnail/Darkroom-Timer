#include <Arduino.h>
#include <FastLED.h>

#include "pins.h"

// number of leds in the strip
#define LED_COUNT 60

// lit led interval (2 = alternating on/off/on/off)
#define LED_SKIP 2

// define brightness limits
#define SAFELIGHT_MAX_BRIGHTNESS 32
#define SAFELIGHT_MIN_BRIGHTNESS 1

// duration of the startup glow
#define SAFELIGHT_STARTUP_DURATION 1500

void setupSafelight();
void safelightStartupAnimation();

void safelightOff();
void safelightOn();

void increaseSafelightBrightness();
void decreaseSafelightBrightness();
void setSafelightBrightness(float percent);