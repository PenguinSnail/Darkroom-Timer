#include "bright.h"

int brightness = MAX_BRIGHTNESS;

void updateBrightness() {
    int steps = MAX_BRIGHTNESS - MIN_BRIGHTNESS;
    float percent = (float)(brightness - MIN_BRIGHTNESS)/(float)steps;
    setSafelightBrightness(percent);
    setDisplayBrightness(percent);
}

void updateBrightnessDisplay() {
    displayBlinkOff();

    setDisplay("BRI ");
    setDisplay(3, brightness + '0');

    updateDisplay();
}

void brightModeLoop() {
    if (getEncoderPosition() > 0 && brightness < MAX_BRIGHTNESS) {
        brightness++;
        updateBrightness();
    } else if (getEncoderPosition() < 0 && brightness > MIN_BRIGHTNESS) {
        brightness--;
        updateBrightness();
    }

    updateBrightnessDisplay();
    resetEncoderPosition();
}
