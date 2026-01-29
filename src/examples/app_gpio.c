/**
 * @file app.c
 * @brief Template for app source files
 * 
 * This module provides the boilerplate code to implement the functions specified in the app.h header
 * 
 * @author Joshua Herer
 * @date January 18, 2026
 * @version 1.0
 */


#include "app/app.h"
#include "drivers/interrupts.h"
#include "drivers/gpio.h"
#include "drivers/timer0.h"
#include "sim/debug.h"

typedef struct {
    uint8_t a;
    gpio_t led0;
    gpio_t led1;
    gpio_t led2;
    gpio_t led3;
} app_state_t;

static app_state_t state = {0};

error_code_t app_init(void) {
    debug_println("GPIO example", DEBUG_LAYER_APP);
    /* Configure drivers and services here */
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.led1, GPIO_B1, GPIO_MODE_INPUT));
    ASSERT_OK(gpio_create(&state.led2, GPIO_B2, GPIO_MODE_INPUT_PULLUP));
    ASSERT_OK(gpio_create(&state.led3, GPIO_B3, GPIO_MODE_OUTPUT));
    //ASSERT_OK(gpio_write(&state.led1, 1)); // Causes init error (write input)
    interrupts_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    /* Run the main loop of the program here */
    ASSERT_OK(gpio_toggle(&state.led0));
    return ERROR_OK;
}