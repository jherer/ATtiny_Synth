/**
 * @file hal_timer1.h
 * @brief Header for Timer1 Hardware Abstraction Layer (HAL)
 * 
 * This module provides a direct interface to Timer0 configuration bits
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#ifndef HAL_TIMER1_H
#define HAL_TIMER1_H
#include <stdint.h>
#include <stdbool.h>

// WAVE GENERATION MODE
void hal_timer1_set_ctc(bool enabled);
void hal_timer1_set_pwm_1A(bool enabled);
void hal_timer1_set_pwm_1B(bool enabled);

// CLOCK SELECT
void hal_timer1_set_clock_select(uint8_t cs_bits);
void hal_timer1_enable_PLL(bool enable);

// COUNTER
uint8_t hal_timer1_get_count(void);
void hal_timer1_set_count(uint8_t count);

// COMPARE REGISTER
void hal_timer1_set_output_compare_register_A(uint8_t ocr1a_value);
void hal_timer1_set_output_compare_register_B(uint8_t ocr1b_value);
void hal_timer1_set_output_compare_register_C(uint8_t ocr1c_value);

// COMPARE MODE
void hal_timer1_set_compare_output_mode_A(uint8_t com1a_bits);
void hal_timer1_set_compare_output_mode_B(uint8_t com1b_bits);

// INTERRUPTS
void hal_timer1_enable_interrupt_compA(bool enable);
void hal_timer1_enable_interrupt_compB(bool enable);
void hal_timer1_enable_interrupt_ovf(bool enable);
#endif