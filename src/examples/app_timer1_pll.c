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
#include <drivers/timer1_driver.h>
#include <drivers/gpio_driver.h>
#include <platform/debug.h>

typedef struct {
    gpio_t led0;
    gpio_t led1;
    uint16_t i;
} app_state_t;

static app_state_t state = {0};

void callback0(void) {
    gpio_toggle(&state.led0);
}

error_code_t app_init(void) {
    DEBUG_PRINTLN("Timer1 PLL mode example", DEBUG_LAYER_APP);
    // Configure drivers and services here 
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(timer1_init(TIMER1_MODE_NORMAL));
    ASSERT_OK(timer1_set_callback(TIMER1_EVENT_OVERFLOW, callback0));
    ASSERT_OK(timer1_enable_callback(TIMER1_EVENT_OVERFLOW, true));
    ASSERT_OK(timer1_start_clock(TIMER1_CLOCK_PLL_64));
    
    interrupt_enable();
    return ERROR_OK;
}
error_code_t app_run(void) {
    // Run the main loop of the program here
    return ERROR_OK;
}