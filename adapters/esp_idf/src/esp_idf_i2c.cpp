#include "aw9523b/esp_idf_i2c.hpp"
#include "esp_err.h"

namespace aw9523b::esp_idf {

EspIdfI2cTransport::EspIdfI2cTransport(
        i2c_master_dev_handle_t device,
        uint8_t address,
        int timeout_ms
)
    : device_(device), address_(address), timeout_ms_(timeout_ms) {}



bool EspIdfI2cTransport::write_reg (
        uint8_t address,
        uint8_t reg,
        uint8_t value
) {
    if (device_ == nullptr || address != address_) {
        return false;
    }

    const uint8_t data [2] = {reg, value};

    return i2c_master_transmit(
        device_,
        data,
        sizeof(data),
        timeout_ms_
    ) == ESP_OK;
}


bool EspIdfI2cTransport::read_reg(
    uint8_t address,
    uint8_t reg,
    uint8_t& value
) {
    if (device_ == nullptr || address != address_) {
        return false;
    }

    uint8_t data = 0;

    const esp_err_t err = i2c_master_transmit_receive(
        device_,
        &reg,
        1,
        &data,
        1,
        timeout_ms_
    );

    if (err != ESP_OK) {
        return false;
    }

    value = data;
    return true;
}



bool EspIdfI2cTransport::write_regs(
    uint8_t address,
    uint8_t start_reg,
    std::span<const uint8_t> data
) {
    if (device_ == nullptr || address != address_) {
        return false;
    }

    if (data.empty()) return true;

    i2c_master_transmit_multi_buffer_info_t buffers[2] = {
        {
            .write_buffer = &start_reg,
            .buffer_size = 1
        },
        {
            .write_buffer = data.data(),
            .buffer_size = data.size()
        }
    };

    return i2c_master_multi_buffer_transmit(
        device_,
        buffers,
        2,
        timeout_ms_
    ) == ESP_OK;
}


} // namespace aw9523b::esp_idf