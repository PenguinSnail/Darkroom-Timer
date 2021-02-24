#include <Arduino.h>

#include "encoder/encoder.h"
#include "display/display.h"
#include "safelight/safelight.h"

#define MAX_BRIGHTNESS 9
#define MIN_BRIGHTNESS 1

void brightModeLoop();
void testLoop();