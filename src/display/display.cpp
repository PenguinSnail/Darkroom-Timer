#include "display.h"

// display instance
Adafruit_AlphaNum4 display;

char displayString[DISPLAY_LENGTH];
int dotPosition;

int displayBrightness;

void updateDisplay() {
    for (int c = 0; (unsigned)c < strlen(displayString); c++) {
        display.writeDigitAscii(c, displayString[c], dotPosition == c);
    }
    display.writeDisplay();
}

void setupDisplay() {
    displayBrightness = DISPLAY_MAX_BRIGHTNESS;

    display = Adafruit_AlphaNum4();
    display.begin(DISPLAY_ADDRESS);
    display.clear();

    // clear out our display string and dot position
    for (int i = 0; i < DISPLAY_LENGTH; i++) {
        displayString[i] = ' ';
    }
    dotPosition = -1;
    // set the display brightness and update the display
    display.setBrightness(displayBrightness);
    updateDisplay();
}

void displayStartupAnimation() {
    int stepDuration = DISPLAY_STARTUP_DURATION / ((DISPLAY_LENGTH * 2) - 1);

    for (int i = 0; i < ((DISPLAY_LENGTH * 2) - 1); i++) {
        if (i > DISPLAY_LENGTH - 1) {
            // dotPosition = (DISPLAY_LENGTH - 1) - (i - (DISPLAY_LENGTH - 1));
            // simplified:
            dotPosition = 2 * DISPLAY_LENGTH - 2 - i;
        } else {
            dotPosition = i;
        }

        updateDisplay();
        delay(stepDuration);
    }

    dotPosition = -1;
    updateDisplay();
};

void increaseDisplayBrightness() {
    if (displayBrightness < DISPLAY_MAX_BRIGHTNESS) displayBrightness++;
    display.setBrightness(displayBrightness);
};

void decreaseDisplayBrightness() {
    if (displayBrightness > DISPLAY_MIN_BRIGHTNESS) displayBrightness--;
    display.setBrightness(displayBrightness);
};