#include "app/app.h"
#include "drivers/interrupts.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"

const uint8_t max_count = 20;

typedef struct {
    gpio_t led0;
    gpio_t led1;
    uint16_t count0;
    uint16_t count1;
    uint8_t top0;
    uint8_t top1;
    volatile bool topped0;
    volatile bool topped1;
} app_state_t;

app_state_t state = {0};

void callback0(void) {
    state.topped0 = true;
}

void callback1(void) {
    state.topped1 = true;
}

error_t app_init(void) {
    state.top0 = 56;
    state.top1 = 183;

    state.count0 = 0;
    state.count1 = 0;
    VALIDATE_ERROR(gpio_create(&state.led0, GPIO_ID_B0, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(gpio_create(&state.led1, GPIO_ID_B1, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(timer_init(TIMER_ID_0, TIMER_MODE_CTC));
    VALIDATE_ERROR(timer_init(TIMER_ID_1, TIMER_MODE_CTC));
    VALIDATE_ERROR(timer_set_callback(TIMER_ID_0, TIMER_EVENT_COMPARE_A, callback0));
    VALIDATE_ERROR(timer_set_callback(TIMER_ID_1, TIMER_EVENT_COMPARE_A, callback1));
    VALIDATE_ERROR(timer_enable_callback(TIMER_ID_0, TIMER_EVENT_COMPARE_A, true));
    VALIDATE_ERROR(timer_enable_callback(TIMER_ID_1, TIMER_EVENT_COMPARE_A, true));
    VALIDATE_ERROR(timer_set_top(TIMER_ID_0, state.top0));
    VALIDATE_ERROR(timer_set_top(TIMER_ID_1, state.top1));
    VALIDATE_ERROR(timer_start(TIMER_ID_0, TIMER_CLOCK_1024));
    VALIDATE_ERROR(timer_start(TIMER_ID_1, TIMER_CLOCK_1024));
    interrupts_enable();
    return ERROR_OK;
}

error_t app_run(void) {
        if (state.topped0) {
            state.count0++;
            state.topped0 = false;
        }
        if (state.count0 >= max_count) {
            VALIDATE_ERROR(gpio_toggle(&state.led0));
            state.top0 += 1;
            state.top0 *= 1.1;
            VALIDATE_ERROR(timer_set_top(TIMER_ID_0, state.top0));
            state.count0 = 0;
        }

        if (state.topped1) {
            state.count1++;
            state.topped1 = false;
        }
        if (state.count1 >= max_count) {
            VALIDATE_ERROR(gpio_toggle(&state.led1));
            state.top1 += 1;
            state.top1 *= 1.1;
            VALIDATE_ERROR(timer_set_top(TIMER_ID_1, state.top1));
            state.count1 = 0;
        }
        
    return ERROR_OK;
}