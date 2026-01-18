/**
 * @file gpio.h
 * @brief Header for GPIO driver
 * 
 * This module is used to create and modify instances of the gpio_t struct that represents a single GPIO pin.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

 #ifndef GPIO_H
#define GPIO_H
#include "hal/gpio_ids.h"
#include <stdbool.h>
#include "drivers/error.h"

// Enum for the modes (data direction) settings of a pin
typedef enum {
    GPIO_MODE_UNINITIALIZED = 0,
    GPIO_MODE_INPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_OUTPUT,
} gpio_mode_t;

// Struct that holds the location of a certain pin
typedef struct {
    gpio_id_t gpio_id;
    gpio_mode_t mode;
} gpio_t;

// Create new gpio struct
error_t gpio_create(gpio_t *new_gpio, gpio_id_t gpio_id, gpio_mode_t mode);
// Set the mode (data direction) of a pin
error_t gpio_set_mode(gpio_t *gpio, gpio_mode_t mode);
// Read the digital state of an input pin
bool gpio_read(gpio_t *gpio);
// Write a digital state to an output pin
error_t gpio_write(gpio_t *gpio, bool state);
// Toggle an output pin
error_t gpio_toggle(gpio_t *gpio);

#endif