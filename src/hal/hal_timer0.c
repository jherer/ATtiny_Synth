#include <hal/hal_timer0.h>
#include <platform/io.h>
#include <platform/debug.h>
#include "bitwise.h"
#include "masks.h"


// CONTROL

static uint8_t _build_control_register_a(uint8_t wgm0_bits, uint8_t com0a_bits, uint8_t com0b_bits) {
    return ((wgm0_bits << WGM00) & (MASK_WGM0_TCCR0A)) |
            ((com0a_bits << COM0A0) & (MASK_COM0A)) |
            ((com0b_bits << COM0B0) & (MASK_COM0B));
}

static uint8_t _build_control_register_b(uint8_t wgm0_bits, uint8_t cs0_bits) {
    uint8_t wgm02 = 0;
    if ((wgm0_bits & 0b100) != 0) {
        wgm02 = 1;
    }
    return ((cs0_bits << CS00) & (MASK_CS0)) |
            ((wgm02 << WGM02) & (MASK_WGM0_TCCR0B));
}

static uint8_t _get_wgm0_bits(timer0_mode_t mode) {
    switch (mode) {
    case TIMER0_MODE_NORMAL: return 0b000;
    case TIMER0_MODE_CTC: return 0b010;
    case TIMER0_MODE_PWM_FIXED_TOP: return 0b011;
    case TIMER0_MODE_PWM_VARIABLE_TOP: return 0b111;
    case TIMER0_NUM_MODES:
    default: return 0b000;
    }
}

static uint8_t _get_cs0_bits(timer0_clock_t clock) {
    switch (clock) {
    case TIMER0_CLOCK_1: return 0b001;
    case TIMER0_CLOCK_8: return 0b010;
    case TIMER0_CLOCK_64: return 0b011;
    case TIMER0_CLOCK_256: return 0b100;
    case TIMER0_CLOCK_1024: return 0b101;
    case TIMER0_CLOCK_OFF: return 0b000;
    case TIMER0_NUM_CLOCKS:
    default: return 0b000;
    }
}

static uint8_t _get_com0a_bits(bool pwm_a) {
    if (pwm_a) {
        return 0b10;
    }
    return 0;
}

uint8_t _get_com0b_bits(bool pwm_b) {
    if (pwm_b) {
        return 0b10;
    }
    return 0;
}

void hal_timer0_set_control_registers(timer0_mode_t mode, timer0_clock_t clock, bool pwm_a, bool pwm_b) {
    DEBUG_PRINTLN("t0 ctrl", DEBUG_LAYER_HAL);
    uint8_t wgm0_bits = _get_wgm0_bits(mode);
    uint8_t cs0_bits = _get_cs0_bits(clock);
    uint8_t com0a_bits = _get_com0a_bits(pwm_a);
    uint8_t com0b_bits = _get_com0b_bits(pwm_b);

    TCCR0A = _build_control_register_a(wgm0_bits, com0a_bits, com0b_bits);
    TCCR0B = _build_control_register_b(wgm0_bits, cs0_bits);
    DEBUG_PRINTLN_BIN("    TCCR0A", TCCR0A, DEBUG_LAYER_HAL);
    DEBUG_PRINTLN_BIN("    TCCR0B", TCCR0B, DEBUG_LAYER_HAL);
}


// COUNTER 

uint8_t hal_timer0_get_count(void) {
    return TCNT0;
}

void hal_timer0_set_count(uint8_t count) {
    DEBUG_PRINTLN("t0 count", DEBUG_LAYER_HAL);
    TCNT0 = count;
    DEBUG_PRINTLN_DEC("    TCNT0", TCNT0, DEBUG_LAYER_HAL);
}


// COMPARE REGISTER

void hal_timer0_set_output_compare_register_a(uint8_t value) {
    DEBUG_PRINTLN("t0 ocr0a", DEBUG_LAYER_HAL);
    OCR0A = value;
    DEBUG_PRINTLN_DEC("    OCR0A", OCR0A, DEBUG_LAYER_HAL);
}

void hal_timer0_set_output_compare_register_b(uint8_t value) {
    DEBUG_PRINTLN("t0 ocr0b", DEBUG_LAYER_HAL);
    OCR0B = value;
    DEBUG_PRINTLN_DEC("    OCR0B", OCR0B, DEBUG_LAYER_HAL);
}

// INTERRUPTS

void hal_timer0_enable_interrupt_compa(bool enable) {
    DEBUG_PRINTLN("t0 int compa", DEBUG_LAYER_HAL);
    bitwise_write_bit(&TIMSK, OCIE0A, enable);
    DEBUG_PRINTLN_DEC("    TIMSK", TIMSK, DEBUG_LAYER_HAL);
}

void hal_timer0_enable_interrupt_compb(bool enable) {
    DEBUG_PRINTLN("t0 interrupt compb", DEBUG_LAYER_HAL);
    bitwise_write_bit(&TIMSK, OCIE0B, enable);
    DEBUG_PRINTLN_DEC("    TIMSK", TIMSK, DEBUG_LAYER_HAL);
}

void hal_timer0_enable_interrupt_ovf(bool enable) {
    DEBUG_PRINTLN("t0 interrupt ovf", DEBUG_LAYER_HAL);
    bitwise_write_bit(&TIMSK, TOIE0, enable);
    DEBUG_PRINTLN_DEC("    TIMSK", TIMSK, DEBUG_LAYER_HAL);
}