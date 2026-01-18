/**
 * @file timer.h
 * @brief Header for timer driver
 * 
 * This module provides functions to configure hardware timers. No matter which timer you use,
 * they are configured with the same functions, just with a different ID parameter. However,
 * you must still keep in mind the hardware differences, because attempting to use features
 * unavailable to a certain timer will return an error.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#ifndef TIMER_H
#define TIMER_H
#include <stdbool.h>
#include "drivers/timer_clock.h"
#include "drivers/error.h"

typedef void (*timer_callback_t)(void);

typedef enum {
    TIMER_EVENT_COMPARE_A,
    TIMER_EVENT_COMPARE_B,
    TIMER_EVENT_OVERFLOW,
} timer_event_t;

typedef enum {
    TIMER_MODE_NORMAL,
    TIMER_MODE_CTC,
} timer_mode_t;

error_t timer_init(timer_id_t id, timer_mode_t mode);
error_t timer_set_top(timer_id_t id, uint8_t top);
error_t timer_set_count(timer_id_t id, uint8_t count);
error_t timer_set_callback(timer_id_t id, timer_event_t event, timer_callback_t callback);
error_t timer_enable_callback(timer_id_t id, timer_event_t event, bool enable);
error_t timer_start(timer_id_t id, timer_clock_t clock);
error_t timer_stop(timer_id_t id);
#endif