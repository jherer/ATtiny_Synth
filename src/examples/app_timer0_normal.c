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
#include "drivers/timer0.h"
#include "drivers/gpio.h"

typedef struct {
    gpio_t led0;
} app_state_t;

static app_state_t state = {0};

void callback0() {
    gpio_toggle(&state.led0);
}

error_code_t app_init(void) {
    /* Configure drivers and services here */
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(timer0_init(TIMER0_MODE_NORMAL));
    ASSERT_OK(timer0_set_callback(TIMER_EVENT_OVERFLOW, callback0));
    ASSERT_OK(timer0_enable_callback(TIMER_EVENT_OVERFLOW, true));
    ASSERT_OK(timer0_start_clock(TIMER0_CLOCK_1024));
    
    interrupts_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    /* Run the main loop of the program here */

    return ERROR_OK;
}