#pragma once
#include <stdbool.h>

typedef enum {
    TIMER1_MODE_NORMAL = 0,
    TIMER1_MODE_CTC,
    TIMER1_MODE_PWM_VARIABLE_TOP,
    
    TIMER1_NUM_MODES,
    TIMER1_MODE_INVALID,
} timer1_mode_t;

static inline bool timer1_is_mode_valid(timer1_mode_t mode) {
    return (unsigned)mode >= TIMER1_NUM_MODES;
}


typedef enum {
    TIMER1_CLOCK_OFF = 0,
    TIMER1_CLOCK_1,
    TIMER1_CLOCK_2,
    TIMER1_CLOCK_4,
    TIMER1_CLOCK_8,
    TIMER1_CLOCK_16,
    TIMER1_CLOCK_32,
    TIMER1_CLOCK_64,
    TIMER1_CLOCK_128,
    TIMER1_CLOCK_256,
    TIMER1_CLOCK_512,
    TIMER1_CLOCK_1024,
    TIMER1_CLOCK_2048,
    TIMER1_CLOCK_4096,
    TIMER1_CLOCK_8192,
    TIMER1_CLOCK_16834,

    TIMER1_NUM_CLOCKS,
    TIMER1_CLOCK_INVALID,
} timer1_clock_t;

static inline bool timer1_is_clock_valid(timer1_clock_t clock) {
    return (unsigned)clock >= TIMER1_NUM_CLOCKS;
}


typedef void (*timer1_callback_t)(void);

typedef enum {
    TIMER1_EVENT_COMPA,
    TIMER1_EVENT_COMPB,
    TIMER1_EVENT_OVERFLOW,
    
    TIMER1_NUM_EVENTS,
    TIMER1_EVENT_INVALID,
} timer1_event_t;

static inline bool timer1_is_event_valid(timer1_event_t event) {
    return (unsigned)event < TIMER1_NUM_EVENTS;
}
