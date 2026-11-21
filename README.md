# AW9523B

A platform-independent C++20 driver for the Awinic AW9523B 16-channel LED driver and GPIO expander.

## Features

- I²C communication through an abstract transport interface
- Device identification
- Hardware and software reset
- GPIO input/output configuration
- GPIO read and write operations
- LED current scaling and 8-bit dimming
- Individual and batch LED configuration
- GPIO interrupt configuration and input reading
- ESP-IDF and Arduino platform adapters

## Requirements

- C++20-compatible compiler and standard library
- I²C master interface
- AW9523B device

Platform compatibility depends on the selected toolchain. ESP-IDF and Arduino adapters have not yet completed their integration tests.

## Project structure

- `core/` — Platform-independent driver
- `adapters/esp_idf/` — ESP-IDF implementation
- `adapters/arduino/` — Arduino implementation
- `examples/` — Usage examples
- `tests/` — Tests
- `docs/` — Additional documentation

## Pin numbering

The library uses a logical pin numbering convention:

- 0–7: P0_0 through P0_7
- 8–15: P1_0 through P1_7

Array-based dimming methods use this logical order. The driver handles the different physical DIM register ordering internally.

## Basic usage

Initialize the I²C transport and platform service, create an `aw9523b::Config`, and construct an `aw9523b::Aw9523b` instance.

Call `init()` before using the device.

See `examples/` for platform-specific initialization.

## Interrupts

After reset, interrupt mask bits are cleared, enabling interrupts for GPIO inputs by default.

To enable interrupts only for selected inputs, call `disable_all_interrupts()` first, followed by `set_gpio_interrupt()` for each required pin.

The INTN signal must be connected and handled by the host application. The driver does not install interrupt handlers.

## Status

Version 0.1.0 — initial development release.

The platform-independent Core has been compiled. ESP-IDF and Arduino adapters require integration testing before the first public release.

## License

See [LICENSE](/LICENSE) file.