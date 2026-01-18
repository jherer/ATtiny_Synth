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

#ifndef HAL_TIMER0_H
#define HAL_TIMER0_H
#include <stdint.h>
#include <stdbool.h>

// WAVE GENERATION MODE
void hal_timer0_set_wave_gen_mode(uint8_t wgm_bits);

// CLOCK SELECT
void hal_timer0_set_clock_select(uint8_t cs_bits);

// COUNTER
uint8_t hal_timer0_get_count(void);
void hal_timer0_set_count(uint8_t count);

// COMPARE REGISTER
void hal_timer0_set_output_compare_register_A(uint8_t ocr0a_value);
void hal_timer0_set_output_compare_register_B(uint8_t ocr0b_value);

// COMPARE MODE
void hal_timer0_set_compare_output_mode_A(uint8_t com0a_bits);
void hal_timer0_set_compare_output_mode_B(uint8_t com0b_bits);

// INTERRUPTS
void hal_timer0_enable_interrupt_compA(bool enable);
void hal_timer0_enable_interrupt_compB(bool enable);
void hal_timer0_enable_interrupt_ovf(bool enable);
#endif