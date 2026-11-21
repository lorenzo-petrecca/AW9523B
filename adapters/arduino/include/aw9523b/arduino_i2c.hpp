#pragma once

#include <cstdint>
#include <span>

#include <Wire.h>

#include "aw9523b/i2c_transport.hpp"


namespace aw9523b::arduino {

/**
 * @brief Arduino Wire implementation of the I2C transport.
 *
 * Uses an existing TwoWire instance managed by the application.
 */
class ArduinoI2cTransport final : public I2cTransport {
public:
    /**
     * @brief Creates an I2C transport using an existing Wire bus.
     *
     * @param wire Initialized TwoWire instance.
     */
    explicit ArduinoI2cTransport(TwoWire& wire);

    bool write_reg(
        uint8_t address,
        uint8_t reg,
        uint8_t value
    ) override;

    bool read_reg(
        uint8_t address,
        uint8_t reg,
        uint8_t& value
    ) override;

    bool write_regs(
        uint8_t address,
        uint8_t start_reg,
        std::span<const uint8_t> data
    ) override;

private:
    TwoWire& wire_;

};


} // namespace aw9523b::arduino