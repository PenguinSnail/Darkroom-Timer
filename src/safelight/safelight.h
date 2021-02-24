#include <Arduino.h>
#include <FastLED.h>

#include "pins.h"

// number of leds in the strip
#define LED_COUNT 60

// lit led interval (2 = alternating on/off/on/off)
#define LED_SKIP 2

// define brightness limits and a variable to store brightness
#define MAX_BRIGHTNESS 32
#define MIN_BRIGHTNESS 1

// duration of the startup glow
#define STARTUP_DURATION 1500

void setupSafelight();
void startupAnimation();

void safelightOff();
void safelightOn();