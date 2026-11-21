#pragma once

#include <cstdint>

#include "aw9523b/platform_service.hpp"

namespace aw9523b::arduino {

/**
 * @brief Arduino implementation of the platform service.
 *
 * Provides GPIO output control and blocking delays
 * through the standard Arduino API.
 */
class ArduinoPlatform final : public PlatformService {
public:
    bool configure_output(uint32_t pin, uint8_t level) override;

    bool set_pin_level(uint32_t pin, uint8_t level) override;

    void delay_ms(uint32_t ms) override;
};

} // namespace aw9523b::arduino
