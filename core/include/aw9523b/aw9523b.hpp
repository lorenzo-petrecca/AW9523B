#pragma once

#include "aw9523b/config.hpp"
#include "aw9523b/result.hpp"
#include <span>
#include <cstdint>
#include "led_info.hpp"


namespace aw9523b {
    
class Aw9523b {
private:
    Config cfg_;

    bool write_register (uint8_t reg, uint8_t value);
    bool write_multiple (uint8_t start_reg, std::span<const uint8_t> data);
    bool read_register (uint8_t reg, uint8_t& value);

public:
    Aw9523b(Config& cfg) : cfg_(cfg) {};


    /**
     * @brief Performs a hardware reset of the AW9523B device.
     *
     * Requires a successful prior call to init() and a configured
     * reset GPIO. The GPIO is not reconfigured by this method.
     *
     * The method asserts the active-low RSTN signal, waits for
     * the reset pulse duration, then releases RSTN and waits
     * before returning.
     *
     * Hardware reset restores the device registers to their
     * reset defaults. Any previous LED or GPIO configuration
     * must be applied again if needed.
     *
     * This operation does not reinitialize the I2C controller
     * or verify the device identification register.
     *
     * @return Result of the hardware reset operation.
     * @retval Code::OK Reset sequence completed without reported errors.
     * @retval Code::HW_RESET_UNAVAILABLE No reset GPIO was configured.
     * @retval Code::RESET_ERROR A reset GPIO operation failed.
     */
    err::result hardware_reset ();


    /**
     * @brief Initializes or reinitializes the AW9523B device.
     *
     * If a reset GPIO is configured, this method prepares the
     * active-low RSTN signal, releases the device from reset,
     * and waits before starting I2C communication.
     *
     * RSTN must remain LOW during power-up and must only be
     * released after the required power-on timing. The caller
     * must ensure that the device supply is stable.
     *
     * If no reset GPIO is configured, the caller must ensure
     * that the device is powered, released from reset and
     * ready for I2C communication.
     *
     * The method reads the device ID register and verifies
     * that its value matches the expected AW9523B identifier.
     *
     * Repeated calls are allowed. When hardware reset is
     * available, reinitialization restores the device's
     * registers to their reset defaults.
     *
     * The I2C transport must already be initialized.
     * This method does not configure LED or GPIO functions.
     *
     * @return result of the initialization operation.
     */
    err::result init ();

    
    /**
     * @brief Check the device's model ID
     * 
     * Use this method to simultaneously verify that I2C communication 
     * is established correctly and that the device is correctly 
     * reporting the expected unique model ID.
     */
    err::result check_device ();


    /**
     * @brief Sets the global LED current scale.
     *
     * Updates the ISEL field of the GCR register while preserving
     * the current P0 output mode.
     *
     * This setting applies to all LED channels.
     *
     * @param scale Desired LED current scale.
     * @return Result of the operation.
     */
    err::result set_current_scale (CurrentScale scale);

    /**
     * @brief Sets the GPIO output driver mode for port P0.
     *
     * Updates the GPOMD bit of the GCR register while preserving
     * the current LED current scale.
     *
     * @param mode Open-drain or push-pull output mode.
     * @return Result of the operation.
     */
    err::result set_port0_output_mode (GPOMD mode);


    /**
     * @brief Selects the LED or GPIO function of a device pin.
     *
     * Updates the corresponding bit in the LED mode register
     * while preserving the configuration of the other pins.
     *
     * Selecting GPIO mode does not configure the pin direction.
     * GPIO input/output direction is managed separately.
     *
     * @param pin Physical device pin.
     * @param mode Desired pin function.
     * @return Result of the operation.
     */
    err::result set_pin_mode (Pin pin, PinMode mode);

    /**
     * @brief Sets the LED or GPIO function for all device pins.
     *
     * Writes both LED mode registers in a single sequential
     * I2C transaction.
     *
     * This operation overwrites the LED/GPIO selection of all
     * 16 pins without changing their GPIO direction settings.
     *
     * @param mode Function to apply to all pins.
     * @return Result of the operation.
     */
    err::result set_all_pins_mode (PinMode mode);


    /**
     * @brief Sets the input/output direction of a GPIO pin.
     *
     * Updates the corresponding GPIO configuration register
     * without changing the LED/GPIO function of the pin.
     *
     * @param pin Physical device pin.
     * @param direction Desired GPIO direction.
     * @return Operation result.
     */
    err::result set_gpio_direction (Pin pin, GPIO_dir direction);

    /**
     * @brief Sets the output level of a GPIO pin.
     *
     * Any non-zero level is treated as HIGH, while zero is LOW.
     * Does not modify the pin function or GPIO direction.
     *
     * @param pin Physical device pin.
     * @param level Desired output level (0 = LOW, non-zero = HIGH).
     * @return Operation result.
     */
    err::result set_gpio_level (Pin pin, uint8_t level);


    /**
     * @brief Reads the logic level detected on a device pin.
     *
     * @param pin Physical device pin.
     * @param level Receives 0 (LOW) or 1 (HIGH) on success.
     *               Remains unchanged if the operation fails.
     * @return Operation result.
     */
    err::result read_gpio (Pin pin, uint8_t& level);


    /**
     * @brief Sets the dimming value of a single LED channel.
     *
     * Writes the DIM register associated with the specified pin.
     * Does not change the pin's LED/GPIO function.
     *
     * @param pin Physical device pin.
     * @param dim Dimming value (0-255).
     * @return Operation result.
     */
    err::result set_led_dimming (Pin pin, uint8_t dim);


    /**
     * @brief Sets the same dimming value for all 16 LED channels.
     *
     * Writes all DIM registers in one sequential I2C transaction.
     * Does not change the LED/GPIO function of any pin.
     *
     * @param dim Dimming value applied to every channel (0-255).
     * @return Operation result.
     */
    err::result set_all_leds_dim_level (uint8_t dim);


    /**
     * @brief Sets the dimming values of selected LED channels.
     *
     * Only the specified pins are updated. Other channels remain
     * unchanged. Each entry is written individually.
     *
     * Duplicate pins are allowed; later entries overwrite earlier
     * ones. An I2C error stops the operation, but previously
     * completed writes are not rolled back.
     *
     * An empty list succeeds without performing any I2C transaction.
     *
     * @param leds LED pins and their corresponding dimming values.
     * @return Operation result.
     */
    err::result set_led_dimmings (std::span<const LedInfo> leds);


    /**
     * @brief Sets individual dimming values for all 16 LED channels.
     *
     * Expects exactly 16 values ordered by logical pin index:
     * P0_0 through P0_7, followed by P1_0 through P1_7.
     *
     * The values are internally reordered to match the DIM
     * register layout and written in one sequential I2C transaction.
     *
     * Does not change the LED/GPIO function of any pin.
     *
     * @param dims Dimming values (0-255), one for each pin.
     * @return Operation result.
     */
    err::result set_all_leds_dimmings (std::span<const uint8_t> dims);


    /**
     * @brief Replaces all LED dimming values using a list of channels.
     *
     * Sets the specified LED channels to their provided dimming values
     * and clears all unspecified channels to zero.
     *
     * All 16 DIM registers are written in a single sequential
     * I2C transaction. An empty list clears all channels.
     *
     * Duplicate pins are allowed; the last value takes precedence.
     * Does not change the LED/GPIO function of any pin.
     *
     * @param leds LED pins and their corresponding dimming values.
     * @return Operation result.
     */
    err::result replace_led_dimmings (std::span<const LedInfo> leds);



    /**
     * @brief Performs a software reset of the AW9523B device.
     *
     * After reset verifies its identification.
     *
     * The reset restores the device registers to their default
     * values. Previous LED and GPIO configurations are not restored.
     *
     * Requires an operational I2C connection. The external RSTN
     * pin is not controlled.
     *
     * @return Operation result.
     */
    err::result software_reset ();



    /**
     * @brief Enables or disables interrupts for a GPIO pin.
     *
     * Updates the corresponding interrupt mask bit while preserving
     * the interrupt configuration of other pins.
     *
     * After device reset, all interrupt mask bits are cleared,
     * meaning interrupts are enabled by default for GPIO inputs.
     *
     * To enable interrupts only for selected pins, it is recommended
     * to call disable_all_interrupts() first, then enable each
     * desired pin individually using this method.
     *
     * The pin must be configured separately as a GPIO input.
     * This method does not configure the host MCU interrupt pin.
     *
     * @param pin Physical device pin.
     * @param enabled True to enable interrupts, false to disable them.
     * @return Operation result.
     */
    err::result set_gpio_interrupt(Pin pin, bool enabled);


    /**
     * @brief Reads all GPIO input states and clears pending interrupts.
     *
     * Reads both input port registers in separate I2C transactions
     * as required by the AW9523B interrupt clearing procedure.
     *
     * The returned value contains the current logic levels, not
     * a record of which pins generated an interrupt.
     *
     * Bits 0-7 represent P0_0 through P0_7.
     * Bits 8-15 represent P1_0 through P1_7.
     *
     * New input changes may generate additional interrupts during
     * or after the read operation.
     *
     * @param states Receives the 16 input levels on success.
     *               Remains unchanged if either read fails.
     * @return Operation result.
     */
    err::result read_interrupt_inputs(uint16_t& states);



    /**
     * @brief Disables interrupts for all 16 GPIO pins.
     *
     * Writes 0xFF to both interrupt mask registers in a single
     * sequential I2C transaction.
     *
     * This method does not modify GPIO directions or pin functions.
     * Individual interrupts can subsequently be enabled using
     * set_gpio_interrupt().
     *
     * @return Operation result.
     */
    err::result disable_all_interrupts();
};


} // namespace aw9523b
