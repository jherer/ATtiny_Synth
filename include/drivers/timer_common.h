#pragma once

typedef void (*timer_callback_t)(void);

typedef enum {
    TIMER_EVENT_COMPA,
    TIMER_EVENT_COMPB,
    TIMER_EVENT_OVERFLOW,
    
    NUM_TIMER_EVENTS,
} timer_event_t;
