#include "drivers/gpio_driver.h"
#include <stdlib.h>
#include "hal_gpio.h"
#include <stdlib.h>
#include "sim/debug.h"

error_code_t gpio_create(gpio_t *gpio, gpio_id_t gpio_id, gpio_mode_t mode) {
    *gpio = ((gpio_t) {
        .gpio_id = gpio_id,
        .mode = GPIO_MODE_UNINIT,
        .state = false,
    });
    debug_println_hex("GPIO created at", gpio_id, DEBUG_LAYER_DRIVERS);

    return gpio_set_mode(gpio, mode);
}

// Set the mode (data direction) of a pin
error_code_t gpio_set_mode(gpio_t *gpio, gpio_mode_t mode) {
    if (gpio == NULL) {
        return ERROR_GPIO_NULL_POINTER;
    }
    switch (mode) {
    case GPIO_MODE_INPUT:
        hal_gpio_write_ddr(gpio->gpio_id, 0);
        hal_gpio_write_port(gpio->gpio_id, 0);
        break;
    case GPIO_MODE_INPUT_PULLUP:
        hal_gpio_write_ddr(gpio->gpio_id, 0);
        hal_gpio_write_port(gpio->gpio_id, 1);
        break;
    case GPIO_MODE_OUTPUT:
        hal_gpio_write_port(gpio->gpio_id, 0);
        hal_gpio_write_ddr(gpio->gpio_id, 1);
        break;
    case GPIO_MODE_UNINIT:
        return ERROR_GPIO_UNINIT;
    default:
        return ERROR_GPIO_MODE_UNSUPPORTED;
    }
    gpio->mode = mode;
    debug_println_hex("GPIO mode set to", mode, DEBUG_LAYER_DRIVERS);
    return ERROR_OK;
}

// Read the digital state of an input pin
// If the pin is output, this will return the output state
bool gpio_read(gpio_t *gpio) {
    return hal_gpio_read_pin(gpio->gpio_id);
}

// Set an output pin (high)
error_code_t gpio_write(gpio_t *gpio, bool state) {
    if (gpio == NULL) {
        return ERROR_GPIO_NULL_POINTER;
    }
    switch (gpio->mode) {
    case GPIO_MODE_OUTPUT:
            hal_gpio_write_port(gpio->gpio_id, state);
            gpio->state = state;
        break;
    case GPIO_MODE_INPUT:
    case GPIO_MODE_INPUT_PULLUP:
        return ERROR_GPIO_WRITE_INPUT;
    case GPIO_MODE_UNINIT:
        return ERROR_GPIO_UNINIT;
    default:
        return ERROR_GPIO_MODE_UNSUPPORTED;
    }
    return ERROR_OK;
}

// Toggle an output pin
// If the pin is input this will toggle the pullup resistor
// Checking the state manually can be better than using XOR for toggling
// Because XOR is not a fully atomic operation
error_code_t gpio_toggle(gpio_t *gpio) {
    if (gpio == NULL) {
        return ERROR_GPIO_NULL_POINTER;
    }
    switch (gpio->mode) {
    case GPIO_MODE_OUTPUT:
        if (gpio->state == true) {
            hal_gpio_write_port(gpio->gpio_id, false);
            gpio->state = false;
        } else {
            hal_gpio_write_port(gpio->gpio_id, true);
            gpio->state = true;
        }
        //hal_gpio_toggle_port(gpio->gpio_id);
        break;
    case GPIO_MODE_INPUT:
    case GPIO_MODE_INPUT_PULLUP:
        return ERROR_GPIO_WRITE_INPUT;
    case GPIO_MODE_UNINIT:
        return ERROR_GPIO_UNINIT;
    default:
        return ERROR_GPIO_MODE_UNSUPPORTED;
    }
    return ERROR_OK;
}