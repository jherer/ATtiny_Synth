/**
 * @file timer0_driver.h
 * @brief Header for timer0 driver
 * 
 * This module provides policy enforcement for using the timer0 hardware-abstraction layer (HAL).
 * Using this instead of directly calling the HAL prevents invalid configurations and makes sure
 * the app explicitly specifies which configurations it wants to use. The app should not just assume
 * a default configuration.
 * 
 * @author Joshua Herer
 * @date Februrary 2, 2026
 * @version 1.0
 */


#pragma once
#include <hal/timer0_enums.h>
#include <stdint.h>
#include <stdbool.h>
#include <core/error_code.h>

#define ERROR_TIMER0_UNINIT             (ERROR_MODULE_TIMER0 | 0x01)
#define ERROR_TIMER0_ENUM_UNSUPPORTED   (ERROR_MODULE_TIMER0 | 0x02)
#define ERROR_TIMER0_TOP_BAD_MODE       (ERROR_MODULE_TIMER0 | 0x03)
#define ERROR_TIMER0_PWM_BAD_MODE       (ERROR_MODULE_TIMER0 | 0x04)
#define ERROR_TIMER0_PWM_CHANNEL_BAD_MODE (ERROR_MODULE_TIMER0 | 0x05)
#define ERROR_TIMER0_PWM_PIN_CONFLICT   (ERROR_MODULE_TIMER0 | 0x06)
#define ERROR_TIMER0_EVENT_BAD_MODE     (ERROR_MODULE_TIMER0 | 0x07)


typedef void (*timer0_callback_t)(void);

typedef enum {
    TIMER0_PWM_CHANNEL_A,
    TIMER0_PWM_CHANNEL_B,
    
    TIMER0_PWM_NUM_CHANNELS,
} timer0_pwm_channel_t;

typedef enum {
    TIMER0_EVENT_COMPA,
    TIMER0_EVENT_COMPB,
    TIMER0_EVENT_OVERFLOW,
    
    TIMER0_NUM_EVENTS,
    TIMER0_EVENT_INVALID,
} timer0_event_t;


error_code_t timer0_init(timer0_mode_t mode);
error_code_t timer0_set_top(uint8_t top);
error_code_t timer0_pwm_attach(timer0_pwm_channel_t pwm_channel);
error_code_t timer0_pwm_detach(timer0_pwm_channel_t pwm_channel);
error_code_t timer0_pwm_set_duty(timer0_pwm_channel_t pwm_channel, uint8_t value);
error_code_t timer0_set_callback(timer0_event_t event, timer0_callback_t callback);
error_code_t timer0_enable_callback(timer0_event_t event, bool enable);
error_code_t timer0_start_clock(timer0_clock_t clock);
error_code_t timer0_set_mode(timer0_mode_t mode);
error_code_t timer0_cleanup(void);
void timer0_fire_isr_compa(void);
void timer0_fire_isr_compb(void);
void timer0_fire_isr_ovf(void);