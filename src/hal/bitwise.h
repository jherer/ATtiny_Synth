/**
 * @file bitwise.h
 * @brief Header for bitwise functions used in the Hardware Abstraction Layer (HAL)
 * 
 * This module provides inline utility functions for reading and writing individual bits
 * to an 8-bit binary number.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

static inline bool bitwise_read_bit(uint8_t reg, uint8_t bit_num) {
    return (reg & (1 << bit_num)) != 0;
}

static inline void bitwise_write_bit(volatile uint8_t *reg, uint8_t bit_num, bool enable) {
    if (enable) {
        *reg |= (1 << bit_num);
    } else {
        *reg &= ~(1 << bit_num);
    }
}

inline void bitwise_write_mask(volatile uint8_t *reg, uint8_t mask, uint8_t new_bits) {
    *reg = (*reg & ~mask) | new_bits;
}