#include <Arduino.h>

#include "pins.h"
#include "encoder/encoder.h"
#include "display/display.h"
#include "safelight/safelight.h"

#define MAX_BRIGHTNESS 9
#define MIN_BRIGHTNESS 1

// 100 seconds in ms
#define MAX_DURATION 100000

/* Set Mode (false) / Brightness Mode (true) */
boolean setBrightMode;
/* Mode switch state */
boolean modePressed;
/* Run switch release time (ms) */
long runRelease;

/* Brightness level */
int brightness;
/* Set timer duration */
int32_t setTime;

/**
 * Set systemwide brightness to whatever step `brightness`
 */
void setBrightness() {
    // calculate our brightness ranges from the min and max values
    int steps = MAX_BRIGHTNESS - MIN_BRIGHTNESS;
    // calculate a percentage brightness we should be at
    float percent = (float)(brightness - MIN_BRIGHTNESS)/(float)steps;
    // set the safelight and display brightness to the percentage
    setSafelightBrightness(percent);
    setDisplayBrightness(percent);
}

/**
 * Brightness adjustment mode loop
 */
void brightModeLoop() {
    // read the encoder position
    int encoderPosition = getEncoderPosition();
    // if the encoder position is positive and we haven't hit the max brightness yet
    if (encoderPosition > 0 && brightness < MAX_BRIGHTNESS) {
        // increment the brightness
        brightness++;
    // if the encoder position is negative and we haven't hit the min brightness yet
    } else if (encoderPosition < 0 && brightness > MIN_BRIGHTNESS) {
        // decrement the brightness
        brightness--;
    }

    // if the encoder position is not a resting 0, reset it
    if (encoderPosition != 0) resetEncoderPosition();

    // update the brightness
    setBrightness();

    // set our display text
    setDisplay("BRIGHTNESS");
    // show our brightness level in the last digit of the display
    setDisplay(DISPLAY_LENGTH - 1, brightness + '0');
    // disable the display decimal
    setDisplayDot(-1);
    // update the display
    updateDisplay();
}

/**
 * Timer set mode loop
 */
void setModeLoop() {
    // read the encoder position
    int encoderPosition = getEncoderPosition();
    if (
        // negative position and we're above the lower duration limit
        (encoderPosition < 0 && setTime >= 500) ||
        // positive position and we're below the lower duration limit
        (encoderPosition > 0 && setTime <= MAX_DURATION - 500)
    ) {
        // adjust our set duration by 500ms per encoder click
        setTime += 500 * encoderPosition;
    }

    // reset the encoder if needed
    if (encoderPosition != 0) resetEncoderPosition();

    // set the display to the set duration in ms / 100 so we get a #.#s format
    setDisplay(setTime / 100);
    // set a leading zero in the ones place if there's less than 1 second set
    if (setTime < 1000) setDisplay(DISPLAY_LENGTH - 2, '0');
    // set the display decimal
    setDisplayDot(DISPLAY_LENGTH - 2);
    // update the display
    updateDisplay();
}

/**
 * Run the timer
 */
void runTimer() {
    // enable the relay
    digitalWrite(RELAY_PIN, HIGH);
    // turn off the safelight
    safelightOff();

    // display shows tenths of a second so we decrement by 100ms in our loop
    for (int32_t remainingTime = setTime; remainingTime > 0; remainingTime -= 100) {
        // set the display to remaining ms / 100
        setDisplay(remainingTime / 100);
        // fill in the leading 0 as needed
        if (remainingTime < 1000) setDisplay(DISPLAY_LENGTH - 2, '0');
        // set the display decimal
        setDisplayDot(DISPLAY_LENGTH - 2);
        // update the display
        updateDisplay();
        // delay 100ms
        delay(100);
    }

    // turn off the relay
    digitalWrite(RELAY_PIN, LOW);
    // turn on the safelight
    safelightOn();

    // set the display to 0
    setDisplay(0);
    // set a leading 0
    setDisplay(DISPLAY_LENGTH - 2, '0');
    // set the decimal
    setDisplayDot(DISPLAY_LENGTH - 2);
    // update the display
    updateDisplay();
}

void focusMode() {
    // this is a weird one

    // focus mode is active
    boolean active = true;
    // the button has not been released
    boolean released = false;
    // there hasn't been an exit press
    boolean exitPress = false;

    // turn off the safelight
    safelightOff();
    // turn on the relay
    digitalWrite(RELAY_PIN, HIGH);

    // clear the display
    clearDisplay();
    // write "focus" to the display
    setDisplay(DISPLAY_LENGTH < 6 ? "FOC" : "FOCUS");
    setDisplayDot(-1);
    updateDisplay();

    // while focus mode is active
    while (active) {
        // if the initial press has not been released and the button is in a released state
        if (!released && digitalRead(RUN_PIN) == HIGH) {
            // delay 50ms (debounce)
            delay(50);
            // if it's still high (released) set the released variable
            if (digitalRead(RUN_PIN) == HIGH) released = true;
        }
        // if the initial press has been released but the button is now pressed
        if (released && digitalRead(RUN_PIN) == LOW) {
            // delay 50ms (debounce)
            delay(50);
            // if it's still low (pressed) set the exit press variable
            if (digitalRead(RUN_PIN) == LOW) exitPress = true;
        }
        // if we're currently in the exit press and the button state is released
        if (exitPress && digitalRead(RUN_PIN) == HIGH) {
            // deactivate focus mode
            active = false;
        }
    }

    // turn on the safelight
    safelightOn();
    // turn off the relay
    digitalWrite(RELAY_PIN, LOW);
}

void setup() {
    // default to max brightness
    brightness = MAX_BRIGHTNESS;

    // setup the safelight and display
    setupSafelight();
    setupDisplay();

    // run startup animations
    displayStartupAnimation();
    safelightStartupAnimation();

    // setup our pin modes
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(RUN_PIN, INPUT_PULLUP);
    pinMode(MODE_PIN, INPUT_PULLUP);

    // explicitly turn off the relay to start
    digitalWrite(RELAY_PIN, LOW);

    // set to "set" mode
    setBrightMode = false;
    // set the mode switch state
    modePressed = false;
    // set the run switch release time
    runRelease = millis();
    // start at a 0ms duration
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

    // if we're in brightness mode
    if (setBrightMode) {
        // run the brightness mode loop
        brightModeLoop();
    } else {
        // if the run button is pressed
        // and it's been more than 50ms since it was last released (debouncing)
        if (digitalRead(RUN_PIN) == LOW && millis() - runRelease > 50) {
            // if there is a set duration
            if (setTime > 0) {
                // run the timer
                runTimer();
            } else {
                // otherwise enter focus mode
                focusMode();
            }
            // after exiting focus more or timer
            // freeze until we release the run button
            while (digitalRead(RUN_PIN) == LOW) {}
            // reset the encoder in case it moved
            resetEncoderPosition();
            // set the release time for run button debouncing
            runRelease = millis();
        } else {
            // run the set timer mode loop
            setModeLoop();
        }
    }
}