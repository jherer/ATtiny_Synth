#include "gpio.h"
#include "hal_gpio.h"
#include <avr/io.h>
#include <stdbool.h>

gpio_pin_t gpio_pin_new(hal_pin_t hal_pin) {
    gpio_pin_t new_pin = {
        .hal_pin = hal_pin,
        .mode = GPIO_MODE_INPUT,
    };
    return new_pin;
}

// Set the mode (data direction) of a pin
void gpio_pin_mode(gpio_pin_t *pin, gpio_mode_t mode) {
    pin->mode = mode;
    switch (mode) {
        case GPIO_MODE_INPUT:
            hal_gpio_mode_input(pin->hal_pin);
            hal_gpio_clear(pin->hal_pin);
        case GPIO_MODE_INPUT_PULLUP:
            hal_gpio_mode_input(pin->hal_pin);
            hal_gpio_set(pin->hal_pin);
            break;
        case GPIO_MODE_OUTPUT:
            hal_gpio_clear(pin->hal_pin);
            hal_gpio_mode_output(pin->hal_pin);
    }
}


// Read the digital state of an input pin
// If the pin is output, this will return the output state
bool gpio_pin_read(gpio_pin_t *pin) {
    return hal_gpio_read(pin->hal_pin);
}

// Set an output pin (high)
void gpio_pin_write(gpio_pin_t *pin, bool state) {
    switch (pin->mode) {
        case GPIO_MODE_OUTPUT:
            hal_gpio_set(pin->hal_pin);
            break;
        case GPIO_MODE_INPUT:
        case GPIO_MODE_INPUT_PULLUP:
            break;
    }
}

// Toggle an output pin
// If the pin is input this will toggle the pullup resistor
void gpio_pin_toggle(gpio_pin_t *pin) {
    switch (pin->mode) {
        case GPIO_MODE_OUTPUT:
            hal_gpio_toggle(pin->hal_pin);
            break;
        case GPIO_MODE_INPUT:
        case GPIO_MODE_INPUT_PULLUP:
            break;
    }
}