#include "encoder.h"

Encoder enc(ENCODER_PIN_B, ENCODER_PIN_A);

int getEncoderPosition() {
    return enc.read() / 4;
}

void resetEncoderPosition() {
    enc.write(0);
}