#pragma once

#include <cstdint>


namespace aw9523b {
    

/**
 * @brief Supported 7-bit I2C addresses of the AW9523B.
 *
 * The address is selected by the hardware levels of
 * the AD1 and AD0 pins.
 */
enum class I2cAddr : uint8_t {
    ADDR_0 = 0x58, ///< AD1 = 0; AD0 = 0
    ADDR_1 = 0x59, ///< AD1 = 0; AD0 = 1
    ADDR_2 = 0x5A, ///< AD1 = 1; AD0 = 0
    ADDR_3 = 0x5B, ///< AD1 = 1; AD0 = 1
};




enum class GPOMD : uint8_t {
    P0_OPENDRAIN    = 0x00,
    P0_PUSHPULL     = 0x10,
};



/**
 * @brief Selects the maximum LED current as a percentage
 *        of the nominal full-scale current.
 *
 * The full-scale current is typically 37 mA.
 * The selected scale applies to all LED channels.
 */
enum class CurrentScale : uint8_t {
    I100    = 0x00, // Scale: 1 -> 37 mA per led
    I75     = 0x01, // Scale: 3/4 -> 27.75 mA per led
    I50     = 0x02, // Scale: 1/2 -> 18.5 mA per led
    I25     = 0x03, // Scale: 1/4 -> 9.25 mA per led
};


enum class PinMode : uint8_t {
    LED     = 0,
    GPIO    = 1
};




/**
 * @brief Identifies a physical AW9523B pin.
 *
 * Pins use a zero-based logical numbering convention:
 * - Indices 0-7 correspond to P0_0 through P0_7.
 * - Indices 8-15 correspond to P1_0 through P1_7.
 *
 * The logical pin index is not a DIM register index.
 * APIs accepting arrays of per-pin values use this order
 * unless explicitly documented otherwise.
 */
enum class Pin : uint8_t {
    P0_0 = 0,
    P0_1,
    P0_2,
    P0_3,
    P0_4,
    P0_5,
    P0_6,
    P0_7,

    P1_0,
    P1_1,
    P1_2,
    P1_3,
    P1_4,
    P1_5,
    P1_6,
    P1_7
};


enum class GPIO_dir : uint8_t {
    OUTPUT = 0,
    INPUT = 1
};



inline constexpr I2cAddr ADDR_DEFAULT = I2cAddr::ADDR_0;

} // namespace aw9523b
