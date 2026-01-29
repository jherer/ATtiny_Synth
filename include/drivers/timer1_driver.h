#pragma once
#include "hal/timer1_enums.h"
#include <stdint.h>
#include <stdbool.h>
#include "core/error_code.h"


#define ERROR_TIMER1_UNINIT             (ERROR_MODULE_TIMER1 | 0x02)
#define ERROR_TIMER1_ENUM_UNSUPPORTED   (ERROR_MODULE_TIMER1 | 0x03)
#define ERROR_TIMER1_TOP_BAD_MODE       (ERROR_MODULE_TIMER1 | 0x04)
#define ERROR_TIMER1_PWM_BAD_MODE       (ERROR_MODULE_TIMER1 | 0x05)
#define ERROR_TIMER1_PWM_PIN_CONFLICT   (ERROR_MODULE_TIMER1 | 0x07)
#define ERROR_TIMER1_EVENT_BAD_MODE     (ERROR_MODULE_TIMER1 | 0x08)

typedef enum {
    TIMER1_PWM_CHANNEL_A,
    TIMER1_PWM_CHANNEL_B,
    
    NUM_TIMER1_PWM_CHANNELS,
} timer1_pwm_channel_t;

error_code_t timer1_init(timer1_mode_t mode);
error_code_t timer1_set_top(uint8_t top);
error_code_t timer1_pwm_attach(timer1_pwm_channel_t pwm_channel);
error_code_t timer1_pwm_detach(timer1_pwm_channel_t pwm_channel);
error_code_t timer1_pwm_set_duty(timer1_pwm_channel_t pwm_channel, uint8_t value);
error_code_t timer1_set_callback(timer1_event_t event, timer1_callback_t callback);
error_code_t timer1_enable_callback(timer1_event_t event, bool enable);
error_code_t timer1_start_clock(timer1_clock_t clock);
error_code_t timer1_set_mode(timer1_mode_t mode);
error_code_t timer1_cleanup(void);