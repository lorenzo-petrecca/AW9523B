#include "aw9523b/aw9523b.hpp"
#include "aw9523b/i2c_transport.hpp"
#include "detail.hpp"

#include <array>
#include <algorithm>

namespace aw9523b {

using namespace err;

result Aw9523b::hardware_reset () {
    if (!cfg_.reset_pin) {
        return {Code::HW_RESET_UNAVAILABLE};
    }

    const uint32_t rst = *cfg_.reset_pin;

    if (!cfg_.platform.set_pin_level(rst, 0)) {
        return {Code::RESET_ERROR};
    }

    cfg_.platform.delay_ms(1);
    
    if (!cfg_.platform.set_pin_level(rst, 1)) {
        return {Code::RESET_ERROR};
    }

    cfg_.platform.delay_ms(5);

    return {Code::OK};
}


result Aw9523b::init () {
    const auto address = static_cast<uint8_t>(cfg_.addr);
    if (address < 0x58 || address > 0x5B) {
        return {Code::INVALID_ADDRESS};
    }

    if (cfg_.reset_pin) {
        const uint32_t rst = *cfg_.reset_pin;

        if (!cfg_.platform.configure_output(rst, 0)) {
            return {Code::RESET_ERROR};
        }

        cfg_.platform.delay_ms(1);

        if (!cfg_.platform.set_pin_level(rst, 1)) {
            return {Code::RESET_ERROR};
        }

        cfg_.platform.delay_ms(5);
    }
    

    return check_device();
}



bool Aw9523b::write_register (uint8_t reg, uint8_t value) {
    return cfg_.i2c.write_reg(
        static_cast<uint8_t>(cfg_.addr),
        reg,
        value
    );
}

bool Aw9523b::read_register (uint8_t reg, uint8_t& value) {
    return cfg_.i2c.read_reg(
        static_cast<uint8_t>(cfg_.addr),
        reg,
        value
    );
}

bool Aw9523b::write_multiple (uint8_t start_reg, std::span<const uint8_t> data) {
    return cfg_.i2c.write_regs(
        static_cast<uint8_t>(cfg_.addr),
        start_reg,
        data
    );
}


result Aw9523b::check_device () {
    uint8_t id = 0;
    if (!read_register(detail::reg::ID, id)) {
        return {Code::I2C_ERROR};
    }

    return id == detail::MODEL_ID
        ? result{Code::OK}
        : result{Code::INVALID_DEVICE_ID};
}



result Aw9523b::set_current_scale (CurrentScale scale) {
    if (static_cast<uint8_t>(scale) > 0x03) {
        return {Code::INVALID_ARGUMENT};
    }

    uint8_t gcr = 0;
    if (!read_register(detail::reg::GCR, gcr)) {
        return {Code::I2C_ERROR};
    }

    gcr &= ~uint8_t{0x03};
    gcr |= static_cast<uint8_t>(scale);
    if (!write_register(detail::reg::GCR, gcr)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::set_port0_output_mode (GPOMD mode) {
    if (mode != GPOMD::P0_OPENDRAIN && mode != GPOMD::P0_PUSHPULL) {
        return {Code::INVALID_ARGUMENT};
    }

    uint8_t gcr = 0;
    if (!read_register(detail::reg::GCR, gcr)) {
        return {Code::I2C_ERROR};
    }

    gcr &= ~uint8_t{0x10};
    gcr |= static_cast<uint8_t>(mode);
    if (!write_register(detail::reg::GCR, gcr)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::set_pin_mode (Pin pin, PinMode mode) {
    const uint8_t index = static_cast<uint8_t>(pin);
    if (index >= detail::MAX_PINS) {
        return {Code::INVALID_PIN};
    }

    if (mode != PinMode::LED &&
        mode != PinMode::GPIO) {
        return {Code::INVALID_ARGUMENT};
    }

    const uint8_t port = index >> 3;
    const uint8_t bit = index & 0x07;
    const uint8_t reg = detail::reg::LED_MODE_PORT_0 + port;

    const uint8_t mask = static_cast<uint8_t>(1u << bit);

    uint8_t data = 0;
    if (!read_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    data = (data & ~mask) | (static_cast<uint8_t>(mode) << bit);

    if (!write_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::set_all_pins_mode (PinMode mode) {
    if (mode != PinMode::LED &&
        mode != PinMode::GPIO) {
        return {Code::INVALID_ARGUMENT};
    }

    const uint8_t value = mode == PinMode::GPIO ? 0xFF : 0x00;
    uint8_t data [2] = {value, value};

    if (!write_multiple(detail::reg::LED_MODE_PORT_0, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}



result Aw9523b::set_gpio_direction (Pin pin, GPIO_dir direction) {
    const uint8_t index = static_cast<uint8_t>(pin);
    if (index >= detail::MAX_PINS) {
        return {Code::INVALID_PIN};
    }

    if (direction != GPIO_dir::INPUT &&
        direction != GPIO_dir::OUTPUT) {
        return {Code::INVALID_ARGUMENT};
    }

    const uint8_t port = index >> 3;
    const uint8_t bit = index & 0x07;
    const uint8_t reg = detail::reg::CONFIG_PORT_0 + port;

    const uint8_t mask = static_cast<uint8_t>(1u << bit);

    uint8_t data = 0;
    if (!read_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    data = (data & ~mask) | (static_cast<uint8_t>(direction) << bit);

    if (!write_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}



result Aw9523b::set_gpio_level (Pin pin, const uint8_t level) {
    const uint8_t index = static_cast<uint8_t>(pin);
    if (index >= detail::MAX_PINS) {
        return {Code::INVALID_PIN};
    }

    const uint8_t port = index >> 3;
    const uint8_t bit = index & 0x07;
    const uint8_t reg = detail::reg::OUTPUT_PORT_0 + port;

    const uint8_t mask = static_cast<uint8_t>(1u << bit);

    uint8_t data = 0;
    if (!read_register(reg, data)) {
        return {Code::I2C_ERROR};
    }


    data = (data & ~mask) | ((level != 0) << bit);
    if (!write_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::read_gpio (Pin pin, uint8_t& level) {
    const uint8_t index = static_cast<uint8_t>(pin);
    if (index >= detail::MAX_PINS) {
        return {Code::INVALID_PIN};
    }

    const uint8_t port = index >> 3;
    const uint8_t bit = index & 0x07;
    const uint8_t reg = detail::reg::INPUT_PORT_0 + port;

    uint8_t data = 0;
    if (!read_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    level = (data & (1u << bit)) != 0;

    return {Code::OK};
}



result Aw9523b::set_led_dimming (Pin pin, uint8_t dim) {
    const uint8_t index = static_cast<uint8_t>(pin);
    if (index >= detail::MAX_PINS) {
        return {Code::INVALID_PIN};
    }

    const uint8_t reg = detail::helper::dim_register(pin);
    if (!write_register(reg, dim)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::set_all_leds_dim_level (uint8_t dim) {
    std::array<uint8_t, detail::MAX_PINS> data{};
    data.fill(dim);

    if (!write_multiple(detail::reg::DIM0, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::set_led_dimmings (std::span<const LedInfo> leds) {
    // Validate all pins before performing any writes.
    for (const LedInfo& led : leds) {
        if (static_cast<uint8_t>(led.pin) >= detail::MAX_PINS) {
            return {Code::INVALID_PIN};
        }
    }

    // Write the requested dimming values.
    for (const LedInfo& led : leds) {
        const uint8_t reg = detail::helper::dim_register(led.pin);

        if (!write_register(reg, led.dimming)) {
            return {Code::I2C_ERROR};
        }
    }

    return {Code::OK};
}


result Aw9523b::set_all_leds_dimmings (std::span<const uint8_t> dims) {
    if (dims.size() != detail::MAX_PINS) {
        return {Code::INVALID_ARGUMENT};
    }

    std::array<uint8_t, detail::MAX_PINS> data{};

    for (uint8_t i = 0; i < detail::MAX_PINS; ++i) {
        const Pin pin = static_cast<Pin>(i);
        const uint8_t reg = detail::helper::dim_register(pin);

        data[reg - detail::reg::DIM0] = dims[i];
    }

    if (!write_multiple(detail::reg::DIM0, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}



result Aw9523b::software_reset () {
    if (!write_register(detail::reg::SW_RSTN, 0x00)) {
        return {Code::I2C_ERROR};
    }

    cfg_.platform.delay_ms(3);

    return check_device();
}




result Aw9523b::set_gpio_interrupt(Pin pin, bool enabled) {
    const uint8_t index = static_cast<uint8_t>(pin);
    if (index >= detail::MAX_PINS) {
        return {Code::INVALID_PIN};
    }

    const uint8_t port = index >> 3;
    const uint8_t bit = index & 0x07;
    const uint8_t reg = detail::reg::INT_PORT_0 + port;
    const uint8_t mask = static_cast<uint8_t>(1u << bit);

    uint8_t data = 0;
    if (!read_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    if (enabled) {
        data &= static_cast<uint8_t>(~mask);
    } else {
        data |= mask;
    }

    if (!write_register(reg, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


result Aw9523b::read_interrupt_inputs(uint16_t& states) {
    uint8_t p0 = 0;
    uint8_t p1 = 0;

    if (!read_register(detail::reg::INPUT_PORT_0, p0)) {
        return {Code::I2C_ERROR};
    }

    if (!read_register(detail::reg::INPUT_PORT_1, p1)) {
        return {Code::I2C_ERROR};
    }

    states = static_cast<uint16_t>(p0) |
             (static_cast<uint16_t>(p1) << 8);

    return {Code::OK};
}


result Aw9523b::disable_all_interrupts() {
    const uint8_t data[2] = {0xFF, 0xFF};

    if (!write_multiple(detail::reg::INT_PORT_0, data)) {
        return {Code::I2C_ERROR};
    }

    return {Code::OK};
}


} // namespace aw9523b
