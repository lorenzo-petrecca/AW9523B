#include "aw9523b/esp_idf_platform.hpp"

#include "driver/gpio.h"
#include "esp_rom_sys.h"


namespace aw9523b::esp_idf {

bool EspIdfPlatform::configure_output (uint32_t pin, uint8_t level) {
    if (pin >= 64 || !GPIO_IS_VALID_OUTPUT_GPIO(pin)) {
        return false;
    }

    const gpio_num_t gpio = static_cast<gpio_num_t>(pin);

    // Preload the output level before enabling the output.
    if (gpio_set_level(gpio, level) != ESP_OK) {
        return false;
    }

    gpio_config_t config{};
    config.pin_bit_mask = 1ULL << pin;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;

    return gpio_config(&config) == ESP_OK;
}


bool EspIdfPlatform::set_pin_level(
    uint32_t pin,
    uint8_t level
) {
    if (pin >= 64 || !GPIO_IS_VALID_OUTPUT_GPIO(pin)) {
        return false;
    }

    return gpio_set_level(
        static_cast<gpio_num_t>(pin),
        level
    ) == ESP_OK;
}


void EspIdfPlatform::delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; ++i) {
        esp_rom_delay_us(1000);
    }
}

} // namespace aw9523b::esp_idf