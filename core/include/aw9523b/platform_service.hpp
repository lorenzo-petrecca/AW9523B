/**
 * @file platform_service.hpp
 * @brief Platform-independent hardware operations.
 */
#pragma once

#include <cstdint>

namespace aw9523b {

/**
 * @brief Abstract interface for platform-specific hardware operations.
 *
 * Implementations provide GPIO control and timing services
 * without exposing platform-specific APIs to the core.
 */
class PlatformService {
public:
    virtual ~PlatformService () = default;

    /**
     * @brief Configures a GPIO as an output.
     *
     * The implementation must set the required level before enabling
     * the output driver, when the platform allows it.
     * 
     * The application must, in any case, ensure a safe state 
     * through the hardware circuit during microcontroller startup.
     *
     * @param pin Platform GPIO identifier.
     * @param level Initial output level (0 = LOW | 1 = HIGH).
     * @return true on success, false otherwise.
     */
    virtual bool configure_output (
        uint32_t pin,
        uint8_t level
    ) = 0;


    /**
     * @brief Sets the output level of a GPIO.
     *
     * @param pin Platform GPIO identifier.
     * @param level 1 for HIGH, 0 for LOW.
     * @return true on success, false otherwise.
     */
    virtual bool set_pin_level (
        uint32_t pin,
        uint8_t level
    ) = 0;


    /**
     * @brief Blocks for at least the requested duration.
     * @param ms Delay duration in milliseconds.
     */
    virtual void delay_ms(uint32_t ms) = 0;

};


} // namespace aw9523b