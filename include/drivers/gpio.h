#ifndef GPIO_H
#define GPIO_H
#include "hal_gpio.h"
#include <avr/io.h>
#include <stdbool.h>

// Enum for the modes (data direction) settings of a pin
typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_OUTPUT,
} gpio_mode_t;

// Struct that holds the location of a certain pin
typedef struct {
    hal_pin_t hal_pin;
    gpio_mode_t mode;
} gpio_pin_t;


// Create new pin struct
gpio_pin_t gpio_pin_new(hal_pin_t hal_pin);

// Set the mode (data direction) of a pin
void gpio_pin_mode(gpio_pin_t *pin, gpio_mode_t mode);
// Read the digital state of an input pin
bool gpio_pin_read(gpio_pin_t *pin);
// Toggle an output pin
void gpio_pin_toggle(gpio_pin_t *pin);


#endif