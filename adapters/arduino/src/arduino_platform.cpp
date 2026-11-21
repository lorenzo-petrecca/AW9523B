
#include "aw9523b/arduino_platform.hpp"

#include <Arduino.h>

namespace aw9523b::arduino {

bool ArduinoPlatform::configure_output(
    uint32_t pin,
    uint8_t level
) {
    if (pin > UINT8_MAX) {
        return false;
    }

    const uint8_t gpio = static_cast<uint8_t>(pin);

    digitalWrite(gpio, level ? HIGH : LOW);
    pinMode(gpio, OUTPUT);

    return true;
}


bool ArduinoPlatform::set_pin_level(
    uint32_t pin,
    uint8_t level
) {
    if (pin > UINT8_MAX) {
        return false;
    }

    digitalWrite(
        static_cast<uint8_t>(pin),
        level ? HIGH : LOW
    );

    return true;
}


void ArduinoPlatform::delay_ms(uint32_t ms) {
    delay(ms);
}

} // namespace aw9523b::arduino
