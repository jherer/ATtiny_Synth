/**
 * @file hal_gpio.h
 * @brief Header for the Hardware Abstraction Layer (HAL) for general-purpose input-output registers (GPIO registers)
 * 
 * This module provides a direct interface to GPIO registers.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H
#include <stdbool.h>
#include "hal/gpio_ids.h"

/* Write to a certain bit on the data direction register */
void hal_gpio_write_ddr(gpio_id_t gpio_id, bool state);

/* Write to a certain bit on the port register (control the state of an output pin or the pullup of an input pin) */
void hal_gpio_write_port(gpio_id_t gpio_id, bool state);

/* Toggle a certain bit on the port register (toggle the state of an output pin) */
void hal_gpio_toggle_port(gpio_id_t gpio_id);

/* Read a certain bit from the pin register (read an input pin) */
bool hal_gpio_read_pin(gpio_id_t gpio_id);
#endif