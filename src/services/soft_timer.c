#include "services/soft_timer.h"
#include "drivers/timer0.h"
#include <stdlib.h>
#include <stdbool.h>

#define TIMER_CLOCK_SOURCE TIMER0_CLOCK_64
#define TIMER_PRESCALER_VALUE 64
#define TIMER_TOP 250
/*
const uint16_t ovf_per_sec = ((F_CPU / TIMER_PRESCALER_VALUE) / TIMER_TOP);

uint32_t num_millis = 0;

typedef struct {
    uint16_t max_count;
    timer_callback_t callback;
    volatile uint16_t counter;
    volatile bool overflow;
} soft_timer_t;

#define MAX_SOFT_TIMERS 8

static soft_timer_t soft_timers[MAX_SOFT_TIMERS] = {
    {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0},
};

static bool _callback_exists(soft_timer_t soft_timer) {
    return soft_timer.callback != NULL;
}

static void _internal_callback(void) {
    num_millis += (1000 / ovf_per_sec);
    for (int i = 0; i < MAX_SOFT_TIMERS; i++) {
        if (_soft_timer_internal_in_use(soft_timers[i])) {
            soft_timers[i].counter++;

            if (soft_timers[i].counter > soft_timers[i].max_count) {
                soft_timers[i].callback();
                soft_timers[i].counter = 0;
                soft_timers[i].overflow = true;
            }
        }
    }
}

error_code_t soft_timer_init() {
    VALIDATE_ERROR(timer0_init(TIMER0_MODE_NORMAL));
    VALIDATE_ERROR(timer0_set_callback(TIMER_EVENT_OVERFLOW, _internal_callback));
    VALIDATE_ERROR(timer0_enable_callback(TIMER_EVENT_OVERFLOW, true));
    VALIDATE_ERROR(timer0_start(TIMER_CLOCK_SOURCE));
    return ERROR_OK;
}

error_code_t soft_timer_add_event(uint8_t i, uint16_t period_ms, timer_callback_t cb) {
    if (i >= MAX_SOFT_TIMERS) {
        return ERROR_SOFT_TIMER_BAD_ID;
    }
    if (_callback_exists(soft_timers[i])) {
        return ERROR_SOFT_TIMER_ID_CONFLICT;
    }
    soft_timers[i].counter = 0;
    soft_timers[i].max_count = period_ms * (ovf_per_sec / 1000);
    soft_timers[i].callback = cb;
    return ERROR_OK;
}

bool soft_timer_check_overflow(uint8_t i) {
    if (i >= MAX_SOFT_TIMERS) {
        return false;
    }
    if (soft_timers[i].overflow) {
        soft_timers[i].overflow = false;
        return true;
    }
    return false;
}

uint32_t soft_timer_get_millis(void) {
    return num_millis;
}

*/