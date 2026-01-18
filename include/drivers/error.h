/**
 * @file error.h
 * @brief Header for error and system status
 * 
 * This module provides enums for errors that can occur in the driver and service layers
 * Functions that return these can have their values passed up through the app to main,
 * where they will be dealt with.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#ifndef ERROR_H
#define ERROR_H
#include "hal/gpio_ids.h"

typedef enum {
    ERROR_OK,

    ERROR_PROGRAM_END,
    ERROR_FUNCTION_UNSUPPORTED,

    ERROR_GPIO_MODE_UNSUPPORTED,
    ERROR_GPIO_WRITE_INPUT,
    ERROR_GPIO_UNINITIALIZED,
    ERROR_GPIO_NULL_POINTER,

    ERROR_TIMER_UNINITIALIZED,
    ERROR_TIMER_MODE_UNSUPPORTED,
    ERROR_TIMER_TOP_NOT_ALLOWED,
    ERROR_TIMER_OVF_NOT_ALLOWED,
    ERROR_TIMER_CLOCK_INVALID,
    ERROR_TIMER_EVENT_UNSUPPORTED,
    ERROR_TIMER_ID_UNSUPPORTED,

    ERROR_PWM_UNINITIALIZED,
    ERROR_PWM_MODE_UNSUPPORTED,
    ERROR_PWM_MODE_INVALID_FOR_T1,
    ERROR_PWM_MODE_TOP_INVALID,
    ERROR_PWM_COMPARE_ABOVE_TOP,
    ERROR_PWM_CHANNEL_UNSUPPORTED,
    ERROR_PWM_CHANNEL_CONFLICT,
    ERROR_PWM_CHANNEL_A_IS_TOP,

    ERROR_SPI_MODE_UNSUPPORTED,
    ERROR_SPI_ROLE_UNSUPPORTED,
    ERROR_SPI_CLOCK_UNSUPPORTED,
    ERROR_SPI_CLOCK_INVALID,
    ERROR_SPI_STROBE_NOT_ALLOWED,
} error_t;

// Wrap any functions in app_init() or app_run() that return error_t
// To return the error to the main
#define VALIDATE_ERROR(x) \
    do { \
        error_t err = x; \
        if (err != ERROR_OK) { \
            return err; \
        } \
    } while (0) \

void error_led_init(gpio_id_t gpio_id);
void error_blink(uint8_t error_code);

#endif