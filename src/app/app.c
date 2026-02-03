/**
 * @file app.c
 * @brief Template for app source files
 * 
 * This module provides the boilerplate code to implement the functions specified in the app.h header
 * 
 * @author Joshua Herer
 * @date January 19, 2026
 * @version 1.0
 */


#include <app/app.h>
#include <drivers/gpio_driver.h>
#include <platform/debug.h>

// Add state variables in this struct
typedef struct {
    uint8_t a;
} app_state_t;

static app_state_t state = {0};

error_code_t app_init(void) {
    // Configure drivers and services here
    DEBUG_PRINTLN("App Template", DEBUG_LAYER_APP);

    interrupt_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    // Run the main loop of the program here
    return ERROR_OK;
}