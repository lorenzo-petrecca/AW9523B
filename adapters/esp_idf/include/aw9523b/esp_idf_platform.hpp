#pragma once

#include <cstdint>
#include "aw9523b/platform_service.hpp"

namespace aw9523b::esp_idf {

/**
 * @brief ESP-IDF implementation of the platform interface.
 *
 * Provides GPIO output control and millisecond delays
 * using ESP-IDF APIs.
 */
class EspIdfPlatform final : public PlatformService {
public: 
    /**
     * @brief Configures a GPIO as an output.
     *
     * @param pin Host MCU GPIO number.
     * @param level Initial output logic level.
     * @return True on success, false on failure.
     */
    bool configure_output (
        uint32_t pin,
        uint8_t level
    ) override;

    /**
     * @brief Sets the logic level of a GPIO output.
     *
     * @param pin Host MCU GPIO number.
     * @param level 1 for HIGH, 0 for LOW.
     * @return True on success, false on failure.
     */
    bool set_pin_level(uint32_t pin, uint8_t level) override;

    /**
     * @brief Performs a blocking delay in milliseconds.
     *
     * @param ms Delay duration in milliseconds.
     */
    void delay_ms(uint32_t ms) override;
};

} // namespace aw9523b::esp_idf