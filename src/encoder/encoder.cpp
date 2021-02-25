#include "encoder.h"

Encoder enc(ENCODER_PIN_B, ENCODER_PIN_A);

/**
 * Get the number of clicks the encoder has rotated
 */
int getEncoderPosition() {
    return enc.read() / 4;
}

/**
 * Reset the encoder position to zero
 */
void resetEncoderPosition() {
    enc.write(0);
}