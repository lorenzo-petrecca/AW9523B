#pragma once

#include <cstdint>
#include "aw9523b/types.hpp"

namespace aw9523b {
    
/**
 * @brief Associates a physical pin with an LED dimming value.
 */
struct LedInfo {
    const Pin pin;
    uint8_t dimming;
};

} // namespace aw9523b
