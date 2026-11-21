#pragma once

#include <cstdint>
#include "aw9523b/types.hpp"

namespace aw9523b::detail {

/**
 * @brief AW9523B internal register addresses.
 *
 * Register addresses are defined according to
 * the AW9523B datasheet, revision V2.4.
 */
namespace reg {

inline constexpr uint8_t INPUT_PORT_0        = 0x00;
inline constexpr uint8_t INPUT_PORT_1        = 0x01;
inline constexpr uint8_t OUTPUT_PORT_0       = 0x02;
inline constexpr uint8_t OUTPUT_PORT_1       = 0x03;
inline constexpr uint8_t CONFIG_PORT_0       = 0x04;
inline constexpr uint8_t CONFIG_PORT_1       = 0x05;
inline constexpr uint8_t INT_PORT_0          = 0x06;
inline constexpr uint8_t INT_PORT_1          = 0x07;
inline constexpr uint8_t ID                  = 0x10;
inline constexpr uint8_t GCR                 = 0x11;
inline constexpr uint8_t LED_MODE_PORT_0     = 0x12;
inline constexpr uint8_t LED_MODE_PORT_1     = 0x13;
inline constexpr uint8_t DIM0                = 0x20;
inline constexpr uint8_t DIM1                = 0x21;
inline constexpr uint8_t DIM2                = 0x22;
inline constexpr uint8_t DIM3                = 0x23;
inline constexpr uint8_t DIM4                = 0x24;
inline constexpr uint8_t DIM5                = 0x25;
inline constexpr uint8_t DIM6                = 0x26;
inline constexpr uint8_t DIM7                = 0x27;
inline constexpr uint8_t DIM8                = 0x28;
inline constexpr uint8_t DIM9                = 0x29;
inline constexpr uint8_t DIM10               = 0x2A;
inline constexpr uint8_t DIM11               = 0x2B;
inline constexpr uint8_t DIM12               = 0x2C;
inline constexpr uint8_t DIM13               = 0x2D;
inline constexpr uint8_t DIM14               = 0x2E;
inline constexpr uint8_t DIM15               = 0x2F;
inline constexpr uint8_t SW_RSTN             = 0x7F;

} // namespace reg


inline constexpr uint8_t MODEL_ID = 0x23; ///< Expected value of the AW9523B identification register.
inline constexpr uint8_t MAX_PINS = 16;


namespace helper {

constexpr uint8_t dim_register (Pin pin) {
    const auto index = static_cast<uint8_t>(pin);

    if (index < 8) {
        return static_cast<uint8_t>(0x24 + index);
    }

    if (index < 12) {
        return static_cast<uint8_t>(0x20 + index - 8);
    }

    return static_cast<uint8_t>(0x2C + index - 12);
}

} // namespace helper

} // namespace aw9523b::detail