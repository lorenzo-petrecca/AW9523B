
#include <Arduino.h>
#include <Wire.h>

#include "aw9523b/aw9523b.hpp"
#include "aw9523b/arduino_i2c.hpp"
#include "aw9523b/arduino_platform.hpp"

using namespace aw9523b;

arduino::ArduinoI2cTransport i2c(Wire);
arduino::ArduinoPlatform platform;

Config config{
    .i2c = i2c,
    .platform = platform,
    .addr = I2cAddr::ADDR_0,
    .reset_pin = std::nullopt
};

Aw9523b device(config);

void setup() {
    Serial.begin(115200);
    Wire.begin();

    const auto result = device.init();

    if (!result.is_ok()) {
        Serial.print("AW9523B initialization failed: ");
        Serial.println(result.description());
        return;
    }

    Serial.println("AW9523B detected!");

    // Configure P0_0 as an LED channel.
    if (!device.set_pin_mode(Pin::P0_0, PinMode::LED).is_ok()) {
        Serial.println("Failed to configure LED mode");
        return;
    }

    if (!device.set_led_dimming(Pin::P0_0, 128).is_ok()) {
        Serial.println("Failed to configure LED dimming");
        return;
    }

    Serial.println("LED configured");
}

void loop() {
}
