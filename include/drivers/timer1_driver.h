#pragma once
#include <hal/timer1_enums.h>
#include <stdint.h>
#include <stdbool.h>
#include <core/error_code.h>

#define ERROR_TIMER1_UNINIT             (ERROR_MODULE_TIMER1 | 0x01)
#define ERROR_TIMER1_ENUM_UNSUPPORTED   (ERROR_MODULE_TIMER1 | 0x02)
#define ERROR_TIMER1_TOP_BAD_MODE       (ERROR_MODULE_TIMER1 | 0x03)
#define ERROR_TIMER1_PWM_BAD_MODE       (ERROR_MODULE_TIMER1 | 0x04)
#define ERROR_TIMER1_PWM_PIN_CONFLICT   (ERROR_MODULE_TIMER1 | 0x05)
#define ERROR_TIMER1_EVENT_BAD_MODE     (ERROR_MODULE_TIMER1 | 0x06)


static inline bool timer1_is_mode_valid(timer1_mode_t mode) {
    return (unsigned)mode < TIMER1_NUM_MODES;
}

static inline bool timer1_is_clock_valid(timer1_clock_t clock) {
    return (unsigned)clock < TIMER1_NUM_CLOCKS;
}


typedef enum {
    TIMER1_PWM_CHANNEL_A,
    TIMER1_PWM_CHANNEL_B,
    
    NUM_TIMER1_PWM_CHANNELS,
} timer1_pwm_channel_t;

static inline bool timer1_is_pwm_channel_valid(timer1_pwm_channel_t pwm_channel) {
    return (unsigned)pwm_channel < NUM_TIMER1_PWM_CHANNELS;
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
void timer1_fire_isr_compa(void);
void timer1_fire_isr_compb(void);
void timer1_fire_isr_ovf(void);