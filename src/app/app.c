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

typedef struct {
} app_state_t;

static app_state_t state;

error_t app_init(void) {

    /* Configure drivers and services here */

    interrupts_enable();
    return ERROR_OK;
}

error_t app_run(void) {

    /* Run the main loop of the program here */

    return ERROR_OK;
}