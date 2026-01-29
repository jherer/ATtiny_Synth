#pragma once
#include <stdbool.h>

typedef enum {
    TIMER0_MODE_NORMAL = 0,
    TIMER0_MODE_CTC,
    TIMER0_MODE_PWM_FIXED_TOP,
    TIMER0_MODE_PWM_VARIABLE_TOP,
    
    TIMER0_NUM_MODES,
    TIMER0_MODE_INVALID,
} timer0_mode_t;

static inline bool timer0_is_mode_valid(timer0_mode_t mode) {
    return (unsigned)mode < TIMER0_NUM_MODES;
}

static inline bool timer0_is_top_variable(timer0_mode_t mode) {
    return (mode == TIMER0_MODE_CTC) || 
            (mode == TIMER0_MODE_PWM_VARIABLE_TOP);
}

static inline bool timer0_is_mode_pwm(timer0_mode_t mode) {
    return (mode == TIMER0_MODE_PWM_FIXED_TOP) ||
                (mode == TIMER0_MODE_PWM_VARIABLE_TOP);
}

typedef enum {
    TIMER0_CLOCK_OFF = 0,
    TIMER0_CLOCK_1,
    TIMER0_CLOCK_8,
    TIMER0_CLOCK_64,
    TIMER0_CLOCK_256,
    TIMER0_CLOCK_1024,

    TIMER0_NUM_CLOCKS,
    TIMER0_CLOCK_INVALID,
} timer0_clock_t;

static inline bool timer0_is_clock_valid(timer0_clock_t clock) {
    return (unsigned)clock < TIMER0_NUM_CLOCKS;
}


typedef void (*timer0_callback_t)(void);

typedef enum {
    TIMER0_EVENT_COMPA,
    TIMER0_EVENT_COMPB,
    TIMER0_EVENT_OVERFLOW,
    
    TIMER0_NUM_EVENTS,
    TIMER0_EVENT_INVALID,
} timer0_event_t;

static inline bool timer0_is_event_valid(timer0_event_t event) {
    return (unsigned)event < TIMER0_NUM_EVENTS;
}

static bool _is_event_valid(timer0_mode_t mode, timer0_event_t event) {
    bool invalid = timer0_is_top_variable(mode) &&
        event == TIMER0_EVENT_OVERFLOW;
    return !invalid && timer_is_event_valid(event);
}



static bool _is_pwm_channel_available(timer0_mode_t mode, timer0_pwm_channel_t pwm_channel) {
    return mode == TIMER0_MODE_PWM_VARIABLE_TOP &&
            pwm_channel == TIMER0_PWM_CHANNEL_A;
}
