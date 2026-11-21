/**
 * @file result.hpp
 * @brief Operation result and error codes for the AW9523B library.
 */

#pragma once

#include <cstdint>
#include <iterator>


namespace aw9523b::err {

/**
 * @brief Identifies the outcome of a library operation.
 *
 * Codes describe operation results, not the internal
 * operating state of the AW9523B.
 */
enum class Code : uint8_t {
    OK                      = 0,
    UNKNOWN                 = 1,
    I2C_ERROR               = 2,
    INVALID_ADDRESS         = 3,
    INVALID_PIN             = 4,
    HW_RESET_UNAVAILABLE    = 5,
    RESET_ERROR             = 6,
    INVALID_DEVICE_ID       = 7,
    INVALID_ARGUMENT        = 8,
};


/**
 * @brief Represents the result of a library operation.
 */
struct result {
    Code code = Code::OK;

    /**
     * @brief Returns a human-readable description of the result.
     * @return A pointer to a static, null-terminated message.
     */
    const char* description () const noexcept;

    /**
     * @brief Check if the result has the code “OK”
     * @return bool
     */
    bool is_ok () const noexcept;
};


} // namespace aw9523b::err
