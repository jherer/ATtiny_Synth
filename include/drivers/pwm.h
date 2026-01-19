#ifndef PWM_H
#define PWM_H
#include "drivers/timer_clock.h"
#include "drivers/error.h"

typedef enum {
    PWM_MODE_FAST_FIXED_TOP_T0, // T0 EXCLUSIVE
    PWM_MODE_FAST_VARIABLE_TOP,
} pwm_mode_t;

typedef enum {
    PWM_CHANNEL_A,
    PWM_CHANNEL_B,
} pwm_channel_t;

error_t pwm_init(timer_id_t id, pwm_mode_t mode);
error_t pwm_attach(timer_id_t id, pwm_channel_t channel);
error_t pwm_detach(timer_id_t id, pwm_channel_t channel);
error_t pwm_set_duty(timer_id_t id, pwm_channel_t channel, uint8_t value);
error_t pwm_set_top(timer_id_t id, uint8_t top);
error_t pwm_start(timer_id_t id, timer_clock_t clock);

#endif