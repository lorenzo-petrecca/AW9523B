#include "aw9523b/arduino_i2c.hpp"


namespace aw9523b::arduino {

ArduinoI2cTransport::ArduinoI2cTransport (TwoWire& wire)
    : wire_(wire) {}

bool ArduinoI2cTransport::write_reg(
    uint8_t address,
    uint8_t reg,
    uint8_t value
) {
    wire_.beginTransmission(address);

    if (wire_.write(reg) != 1 ||
        wire_.write(value) != 1) {
        wire_.endTransmission();
        return false;
    }

    return wire_.endTransmission() == 0;
}


bool ArduinoI2cTransport::read_reg(
    uint8_t address,
    uint8_t reg,
    uint8_t& value
) {
    wire_.beginTransmission(address);

    if (wire_.write(reg) != 1) {
        wire_.endTransmission();
        return false;
    }

    if (wire_.endTransmission(false) != 0) {
        return false;
    }

    if (wire_.requestFrom(address, static_cast<uint8_t>(1)) != 1) {
        return false;
    }

    const int data = wire_.read();
    if (data < 0) {
        return false;
    }

    value = static_cast<uint8_t>(data);
    return true;
}



bool ArduinoI2cTransport::write_regs(
    uint8_t address,
    uint8_t start_reg,
    std::span<const uint8_t> data
) {
    if (data.empty()) {
        return true;
    }

    wire_.beginTransmission(address);

    if (wire_.write(start_reg) != 1) {
        wire_.endTransmission();
        return false;
    }

    if (wire_.write(data.data(), data.size()) != data.size()) {
        wire_.endTransmission();
        return false;
    }

    return wire_.endTransmission() == 0;
}



} // namespace aw9523b::arduino