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


#include <app/app.h>
#include <drivers/gpio_driver.h>
#include <drivers/timer0_driver.h>
#include <platform/debug.h>

typedef struct {
    uint8_t a;
    gpio_t led0;
    gpio_t led1;
    gpio_t led2;
    gpio_t led3;
} app_state_t;

static app_state_t state = {0};

error_code_t app_init(void) {
    DEBUG_PRINTLN("GPIO example", DEBUG_LAYER_APP);
    /* Configure drivers and services here */
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.led1, GPIO_B1, GPIO_MODE_INPUT));
    ASSERT_OK(gpio_create(&state.led2, GPIO_B2, GPIO_MODE_INPUT_PULLUP));
    ASSERT_OK(gpio_create(&state.led3, GPIO_B3, GPIO_MODE_OUTPUT));
    //ASSERT_OK(gpio_write(&state.led1, 1)); // Causes init error (write input)
    interrupt_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    /* Run the main loop of the program here */
    //while (1) {
    //ASSERT_OK(gpio_toggle(&state.led0));
    ASSERT_OK(gpio_write(&state.led3, gpio_read(&state.led2)));
    //ASSERT_OK(0x27);
    //}
    return ERROR_OK;
}