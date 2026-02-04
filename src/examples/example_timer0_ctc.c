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
#include <drivers/timer0_driver.h>
#include <drivers/gpio_driver.h>
#include <platform/debug.h>
#include <platform.h>

typedef struct {
    gpio_t led0;
    gpio_t led1;
    gpio_t btn0;
    uint8_t i;
    bool last_btn;
} app_state_t;

static app_state_t state = {0};

void callback0(void) {
    gpio_write(&state.led1, 1);
}

error_code_t app_init(void) {
    DEBUG_PRINTLN("Timer0 CTC mode example", DEBUG_LAYER_APP);
    /* Configure drivers and services here */
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.btn0, GPIO_B2, GPIO_MODE_INPUT_PULLUP));
    ASSERT_OK(gpio_create(&state.led1, GPIO_B3, GPIO_MODE_OUTPUT));
    ASSERT_OK(timer0_init(TIMER0_MODE_CTC));
    ASSERT_OK(timer0_set_callback(TIMER0_EVENT_COMPA, callback0));
    ASSERT_OK(timer0_enable_callback(TIMER0_EVENT_COMPA, true));
    ASSERT_OK(timer0_set_top(250));
    ASSERT_OK(timer0_start_clock(TIMER0_CLOCK_64));

    if ((TCCR0B & 0b111) == 0) {
        return 0x43;
    }
    state.i = 0;
    //ASSERT_OK(0x74);
    interrupt_enable();
    return ERROR_OK;
}
error_code_t app_run(void) {
    /* Run the main loop of the program here */
    //DEBUG_PRINTLN("Running", DEBUG_LAYER_APP);
    //bool btn_state = !gpio_read(&state.btn0);
    ASSERT_OK(gpio_write(&state.led0, !gpio_read(&state.btn0)));
    if (!gpio_read(&state.btn0)) {
        timer0_fire_isr_compa();
    } else {
        gpio_write(&state.led1, 0);
    }
    if ((TIMSK) & (1 << OCIE0A) == 0) {
        return 0x31;
    }
    /*if (TCNT0 > 200) {
        return 0x33;
    }*/
    //TCCR0B = 0b011;
    //DEBUG_PRINTLN_BIN("TCCR0B", TCCR0B, DEBUG_LAYER_APP);
    if ((TCCR0B & 0b111) == 0) {
        return 0x35;
    }
    /*if (btn_state && !state.last_btn) {
        gpio_toggle(&state.led0);
    }
    state.last_btn = btn_state;*/
    return ERROR_OK;
}