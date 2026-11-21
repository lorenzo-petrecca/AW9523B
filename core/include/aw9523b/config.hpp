/**
 * @file config.hpp
 * @brief Configuration types for the AW9523B device.
 */

#pragma once

#include <optional>
#include "aw9523b/types.hpp"
#include "platform_service.hpp"
#include "aw9523b/i2c_transport.hpp"

namespace aw9523b {

/**
 * @brief Hardware connections required by an AW9523B instance.
 *
 * The I2C transport must be initialized before communication.
 * Referenced hardware adapters must remain valid while in use.
 */
struct Config {
    I2cTransport& i2c; ///< I2C transport used for register access.
    PlatformService& platform; ///< Platform service
    I2cAddr addr = ADDR_DEFAULT; ///< Device address on the I2C bus.
    std::optional<uint32_t> reset_pin = std::nullopt; ///< Optional hardware reset pin.
};

} // namespace aw9523b
