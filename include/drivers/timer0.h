#pragma once

#include "drivers/timer0.h"
#include "hal/timer0_enums.h"
#include "drivers/timer_common.h"
#include <stdint.h>
#include <stdbool.h>
#include "drivers/error_code.h"


// PUBLIC API

#define ERROR_TIMER0_UNINITIALIZED      (ERROR_MODULE_TIMER0 | 0x02)
#define ERROR_TIMER0_ENUM_UNSUPPORTED   (ERROR_MODULE_TIMER0 | 0x03)
#define ERROR_TIMER0_TOP_BAD_MODE       (ERROR_MODULE_TIMER0 | 0x04)
#define ERROR_TIMER0_PWM_BAD_MODE       (ERROR_MODULE_TIMER0 | 0x05)
#define ERROR_TIMER0_PWM_CHANNEL_BAD_MODE   (ERROR_MODULE_TIMER0 | 0x06)
#define ERROR_TIMER0_PWM_PIN_CONFLICT   (ERROR_MODULE_TIMER0 | 0x07)
#define ERROR_TIMER_EVENT_BAD_MODE     (ERROR_MODULE_TIMER0 | 0x08)


error_code_t timer0_init(timer0_mode_t mode);
error_code_t timer0_set_top(uint8_t top);
error_code_t timer0_pwm_attach(timer0_pwm_channel_t pwm_channel);
error_code_t timer0_pwm_detach(timer0_pwm_channel_t pwm_channel);
error_code_t timer0_pwm_set_duty(timer0_pwm_channel_t pwm_channel, uint8_t value);
error_code_t timer0_set_callback(timer_event_t event, timer_callback_t callback);
error_code_t timer0_enable_callback(timer_event_t event, bool enable);
error_code_t timer0_start_clock(timer0_clock_t clock);
error_code_t timer0_set_mode(timer0_mode_t mode);
error_code_t cleanup();