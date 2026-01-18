/**
 * @file timer_clock.h
 * @brief Header for timer clock enums
 * 
 * This module provides enums for the timer clock sources. Since the timer driver and pwm driver
 * both require configuring the clock source, the enum is available to both from this header.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */
#ifndef TIMER_CLOCK_H
#define TIMER_CLOCK_H
#include "drivers/error.h"

typedef enum {
    TIMER_ID_0,
    TIMER_ID_1,
    TIMER_ID_COUNT
} timer_id_t;

typedef enum {
    TIMER_CLOCK_OFF,
    TIMER_CLOCK_1,
    TIMER_CLOCK_8,
    TIMER_CLOCK_64,
    TIMER_CLOCK_256,
    TIMER_CLOCK_1024,
    // T1 EXCLUSIVE BELOW
    TIMER_CLOCK_2_T1,
    TIMER_CLOCK_4_T1,
    TIMER_CLOCK_16_T1,
    TIMER_CLOCK_32_T1,
    TIMER_CLOCK_128_T1,
    TIMER_CLOCK_512_T1,
    TIMER_CLOCK_2048_T1,
    TIMER_CLOCK_4096_T1,
    TIMER_CLOCK_8192_T1,
    TIMER_CLOCK_16384_T1,
    TIMER_CLOCK_PLL_64MHZ_T1,
    TIMER_CLOCK_PLL_32MHZ_T1,
    TIMER_CLOCK_PLL_16MHZ_T1,
} timer_clock_t;

#endif