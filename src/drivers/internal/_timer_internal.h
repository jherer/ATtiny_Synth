/**
 * @file _timer_internal.h
 * @brief Header for timer internal functions
 * 
 * This module provides timer configuration functions which will be used
 * in both the timer driver and PWM driver.
 * 
 * @author Joshua Herer
 * @date January 18, 2026
 * @version 1.0
 */

 #ifndef TIMER_INTERNAL_H
#define TIMER_INTERNAL_H
#include "drivers/error.h"
#include "drivers/timer_clock.h"

error_t _timer_internal_t0_set_clock(timer_clock_t clock);
error_t _timer_internal_t1_set_clock(timer_clock_t clock);

#endif