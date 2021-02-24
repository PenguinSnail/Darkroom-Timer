#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

#include "pins.h"

#define DISPLAY_LENGTH 4
#define DISPLAY_ADDRESS 0x70

// define brightness limits
#define DISPLAY_MAX_BRIGHTNESS 15
#define DISPLAY_MIN_BRIGHTNESS 1

// duration of the startup animation
#define DISPLAY_STARTUP_DURATION 1500

void setupDisplay();
void displayStartupAnimation();

void increaseDisplayBrightness();
void decreaseDisplayBrightness();