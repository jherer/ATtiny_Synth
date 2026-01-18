#include "app/app.h"
#include "drivers/interrupts.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"

typedef struct {
    gpio_t led0;
    gpio_t led1;
    uint16_t count0;
    uint16_t count1;
    volatile bool ovf0;
    volatile bool ovf1;
} app_state_t;

app_state_t state = {0};

void callback0(void) {
    state.ovf0 = true;
}

void callback1(void) {
    state.ovf1 = true;
}

error_t app_init(void) {
    VALIDATE_ERROR(gpio_create(&state.led0, GPIO_ID_B0, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(gpio_create(&state.led1, GPIO_ID_B1, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(timer_init(TIMER_ID_0, TIMER_MODE_NORMAL));
    VALIDATE_ERROR(timer_init(TIMER_ID_1, TIMER_MODE_NORMAL));
    VALIDATE_ERROR(timer_set_callback(TIMER_ID_0, TIMER_EVENT_OVERFLOW, callback0));
    VALIDATE_ERROR(timer_set_callback(TIMER_ID_1, TIMER_EVENT_OVERFLOW, callback1));
    VALIDATE_ERROR(timer_enable_callback(TIMER_ID_0, TIMER_EVENT_OVERFLOW, true));
    VALIDATE_ERROR(timer_enable_callback(TIMER_ID_1, TIMER_EVENT_OVERFLOW, true));
    VALIDATE_ERROR(timer_start(TIMER_ID_0, TIMER_CLOCK_1024));
    VALIDATE_ERROR(timer_start(TIMER_ID_1, TIMER_CLOCK_1024));
    VALIDATE_ERROR(timer_set_count(TIMER_ID_0, 0));
    VALIDATE_ERROR(timer_set_count(TIMER_ID_1, 127));
    interrupts_enable();
    return ERROR_OK;
}

error_t app_run(void) {
        if (state.ovf0) {
            state.count0++;
            state.ovf0 = false;
        }
        if (state.count0 >= 21) {
            VALIDATE_ERROR(gpio_toggle(&state.led0));
            state.count0 = 0;
        }

        if (state.ovf1) {
            state.count1++;
            state.ovf1 = false;
        }
        if (state.count1 >= 21) {
            VALIDATE_ERROR(gpio_toggle(&state.led1));
            state.count1 = 0;
        }
        
    return ERROR_OK;
}