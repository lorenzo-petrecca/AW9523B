#pragma once

#include <cstdint>
#include <span>

#include "driver/i2c_master.h"
#include "aw9523b/i2c_transport.hpp"


namespace aw9523b::esp_idf {

/**
 * @brief ESP-IDF implementation of the I2C transport interface.
 *
 * Uses an existing ESP-IDF I2C master device handle.
 * The application owns and manages the I2C bus and device.
 */
class EspIdfI2cTransport final : public I2cTransport {
public:
    /**
     * @brief Creates a transport for an existing I2C device.
     *
     * @param device Valid ESP-IDF I2C master device handle.
     * @param address Configured 7-bit I2C device address.
     * @param timeout_ms Transaction timeout in milliseconds.
     */
    EspIdfI2cTransport(
        i2c_master_dev_handle_t device,
        uint8_t address,
        int timeout_ms = 100
    );

    bool write_reg (
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
    i2c_master_dev_handle_t device_;
    uint8_t address_;
    int timeout_ms_;

};


}
