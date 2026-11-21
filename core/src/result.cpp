#include "aw9523b/result.hpp"


namespace aw9523b::err {

namespace {

struct Entry {
    Code code;
    const char* msg;
};

constexpr Entry entries[] = {
    {Code::OK,                      "Operation completed successfully"},
    {Code::UNKNOWN,                 "Unknown error"}, 
    {Code::I2C_ERROR,               "I2C transaction failed"},
    {Code::INVALID_ADDRESS,         "Unsupported I2C device address"},
    {Code::INVALID_PIN,             "Pin index is outside the supported range"},
    {Code::HW_RESET_UNAVAILABLE,    "Hardware reset controller unavailable"},
    {Code::RESET_ERROR,             "Hardware reset operation failed"},
    {Code::INVALID_DEVICE_ID,       "Unexpected device identifier"},
    {Code::INVALID_ARGUMENT,        "Invalid argument"}
};

}


const char* result::description () const noexcept {
    return static_cast<size_t>(code) < std::size(entries)
        ? entries[static_cast<size_t>(code)].msg
        : "Unknown error";
}


bool result::is_ok () const noexcept {
    return code == Code::OK;
}

    
} // namespace aw9523b::err
