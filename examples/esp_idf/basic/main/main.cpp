#include <cstdint>
#include <cstdio>
#include <optional>

#include "driver/i2c_master.h"
#include "esp_err.h"

#include "aw9523b/aw9523b.hpp"
#include "aw9523b/esp_idf_i2c.hpp"
#include "aw9523b/esp_idf_platform.hpp"

using namespace aw9523b;

extern "C" void app_main() {
    constexpr uint8_t ADDRESS = 0x58;

    // Example GPIO numbers: adapt them to your hardware.
    constexpr int SDA_PIN = 8;
    constexpr int SCL_PIN = 9;

    i2c_master_bus_config_t bus_config{};
    bus_config.i2c_port = I2C_NUM_0;
    bus_config.sda_io_num = static_cast<gpio_num_t>(SDA_PIN);
    bus_config.scl_io_num = static_cast<gpio_num_t>(SCL_PIN);
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = true;

    i2c_master_bus_handle_t bus = nullptr;

    ESP_ERROR_CHECK(
        i2c_new_master_bus(&bus_config, &bus)
    );

    i2c_device_config_t dev_config{};
    dev_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    dev_config.device_address = ADDRESS;
    dev_config.scl_speed_hz = 100000;

    i2c_master_dev_handle_t handle = nullptr;

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(
            bus,
            &dev_config,
            &handle
        )
    );

    {
        esp_idf::EspIdfI2cTransport i2c(handle, ADDRESS);
        esp_idf::EspIdfPlatform platform;

        Config config{
            .i2c = i2c,
            .platform = platform,
            .addr = I2cAddr::ADDR_0,
            .reset_pin = std::nullopt
        };

        Aw9523b device(config);

        const auto result = device.init();

        if (!result.is_ok()) {
            std::printf(
                "AW9523B initialization failed: %s\n",
                result.description()
            );
        } else {
            std::printf("AW9523B detected!\n");
        }
    }

    ESP_ERROR_CHECK(i2c_master_bus_rm_device(handle));
    ESP_ERROR_CHECK(i2c_del_master_bus(bus));
}
