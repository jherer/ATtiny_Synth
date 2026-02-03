/**
 * @file hal_timer0.h
 * @brief Header for Timer0 Hardware Abstraction Layer (HAL)
 * 
 * This module provides a direct interface to Timer0 configuration bits
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <hal/timer0_enums.h>

// CONTROL
void hal_timer0_set_control_registers(timer0_mode_t mode, timer0_clock_t clock, bool pwm_a, bool pwm_b);

// COUNTER 
uint8_t hal_timer0_get_count(void);
void hal_timer0_set_count(uint8_t count);

// COMPARE REGISTER
void hal_timer0_set_output_compare_register_a(uint8_t value);
void hal_timer0_set_output_compare_register_b(uint8_t value);

// INTERRUPTS
void hal_timer0_enable_interrupt_compa(bool enable);
void hal_timer0_enable_interrupt_compb(bool enable);
void hal_timer0_enable_interrupt_ovf(bool enable);