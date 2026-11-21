#pragma once

#include <cstdint>
#include <span>

namespace aw9523b {

class I2cTransport {
public:
    virtual ~I2cTransport () = default;

    virtual bool write_reg (
        uint8_t address,
        uint8_t reg,
        uint8_t value
    ) = 0;

    virtual bool read_reg (
        uint8_t address,
        uint8_t reg,
        uint8_t &value
    ) = 0;
    
    virtual bool write_regs (
        uint8_t address,
        uint8_t start_reg,
        std::span<const uint8_t> data
    ) = 0;
};

} // namespace aw9523b