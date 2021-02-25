#include <Arduino.h>

#include "pins.h"
#include "encoder/encoder.h"
#include "display/display.h"
#include "safelight/safelight.h"

#define MAX_BRIGHTNESS 9
#define MIN_BRIGHTNESS 1

// 100 seconds in ms
#define MAX_DURATION 100000

boolean setBrightMode;
boolean modePressed;
long runRelease;

int brightness;
int32_t setTime;

void setBrightness() {
    int steps = MAX_BRIGHTNESS - MIN_BRIGHTNESS;
    float percent = (float)(brightness - MIN_BRIGHTNESS)/(float)steps;

    setSafelightBrightness(percent);
    setDisplayBrightness(percent);
}

void brightModeLoop() {
    int encoderPosition = getEncoderPosition();
    if (encoderPosition > 0 && brightness < MAX_BRIGHTNESS) {
        brightness++;
    } else if (encoderPosition < 0 && brightness > MIN_BRIGHTNESS) {
        brightness--;
    }

    if (encoderPosition != 0) resetEncoderPosition();

    setBrightness();

    setDisplay("BRIGHTNESS");
    setDisplay(DISPLAY_LENGTH - 1, brightness + '0');
    setDisplayDot(-1);

    updateDisplay();
}

void setModeLoop() {
    int encoderPosition = getEncoderPosition();
    if (
        (encoderPosition < 0 && setTime >= 500) ||
        (encoderPosition > 0 && setTime <= MAX_DURATION - 500)
    ) {
        setTime += 500 * encoderPosition;
    }

    if (encoderPosition != 0) resetEncoderPosition();

    setDisplay(setTime / 100);
    if (setTime < 1000) setDisplay(DISPLAY_LENGTH - 2, '0');
    setDisplayDot(DISPLAY_LENGTH - 2);
    updateDisplay();
}

void runTimer() {
    digitalWrite(RELAY_PIN, HIGH);
    safelightOff();

    for (int32_t remainingTime = setTime; remainingTime > 0; remainingTime -= 100) {
        setDisplay(remainingTime / 100);
        if (remainingTime < 1000) setDisplay(DISPLAY_LENGTH - 2, '0');
        setDisplayDot(DISPLAY_LENGTH - 2);
        updateDisplay();

        delay(100);
    }

    digitalWrite(RELAY_PIN, LOW);
    safelightOn();

    setDisplay(0);
    setDisplay(DISPLAY_LENGTH - 2, '0');
    setDisplayDot(DISPLAY_LENGTH - 2);
    updateDisplay();
}

void setup() {
    brightness = MAX_BRIGHTNESS;

    // setup the safelight and run the startup animation
    setupSafelight();
    setupDisplay();

    displayStartupAnimation();
    safelightStartupAnimation();

    pinMode(RELAY_PIN, OUTPUT);
    pinMode(RUN_PIN, INPUT_PULLUP);
    pinMode(MODE_PIN, INPUT_PULLUP);

    digitalWrite(RELAY_PIN, LOW);

    setBrightMode = false;
    modePressed = false;
    runRelease = millis();

    setTime = 0;
}

void loop() {
    // if the mode switch isn't known to be down
    if (!modePressed) {
        // if the mode switch is now pressed
        if (digitalRead(MODE_PIN) == LOW) {
            // switch modes
            setBrightMode = !setBrightMode;
            // set the mode switch state
            modePressed = true;
        }
    } else {
        // once it's released reset its state
        if (digitalRead(MODE_PIN) == HIGH) modePressed = false;
    }
    
    if (setBrightMode) {
        brightModeLoop();
    } else {
        if (digitalRead(RUN_PIN) == LOW && millis() - runRelease > 50) {
            runTimer();
            // freeze until we release the run button
            while (digitalRead(RUN_PIN) == LOW) {}
            // reset the encoder in case it moved while running the timer
            resetEncoderPosition();
            // used for run button debouncing
            runRelease = millis();
        } else {
            setModeLoop();
        }
    }
}