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
#include "drivers/timer1.h"
#include "drivers/gpio.h"
#include "sim/debug.h"

typedef struct {
    gpio_t led0;
    uint8_t i;
} app_state_t;

static app_state_t state = {0};

void callback0(void) {
    gpio_toggle(&state.led0);
}

error_code_t app_init(void) {
    debug_println("Timer1 CTC mode example", DEBUG_LAYER_APP);
    /* Configure drivers and services here */
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(timer1_init(TIMER1_MODE_CTC));
    ASSERT_OK(timer1_set_callback(TIMER_EVENT_COMPA, callback0));
    ASSERT_OK(timer1_enable_callback(TIMER_EVENT_COMPA, true));
    ASSERT_OK(timer1_set_top(100));
    ASSERT_OK(timer1_start_clock(TIMER1_CLOCK_1024));
    state.i = 0;
    
    interrupts_enable();
    return ERROR_OK;
}
error_code_t app_run(void) {
    /* Run the main loop of the program here */
    ASSERT_OK(timer1_set_top(state.i));
    //ASSERT_OK(timer0_enable_callback(TIMER_EVENT_OVERFLOW, true));
    debug_println("Running", DEBUG_LAYER_APP);
    state.i += 8;
    if (state.i > 100) {
        ASSERT_OK(timer1_cleanup()); // De-initialized timer, causing init error next call
    }
    return ERROR_OK;
}