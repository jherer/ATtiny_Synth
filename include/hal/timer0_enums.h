#pragma once

typedef enum {
    TIMER0_MODE_NORMAL = 0,
    TIMER0_MODE_CTC,
    TIMER0_MODE_PWM_FIXED_TOP,
    TIMER0_MODE_PWM_VARIABLE_TOP,
    
    NUM_TIMER0_MODES
} timer0_mode_t;

typedef enum {
    TIMER0_CLOCK_OFF = 0,
    TIMER0_CLOCK_1,
    TIMER0_CLOCK_8,
    TIMER0_CLOCK_64,
    TIMER0_CLOCK_256,
    TIMER0_CLOCK_1024,

    NUM_TIMER0_CLOCKS,
} timer0_clock_t;

typedef enum {
    TIMER0_PWM_CHANNEL_A,
    TIMER0_PWM_CHANNEL_B,
    
    NUM_TIMER0_PWM_CHANNELS,
} timer0_pwm_channel_t;
