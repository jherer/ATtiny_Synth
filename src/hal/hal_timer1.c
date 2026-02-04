#include <hal/hal_timer1.h>
#include <platform/io.h>
#include <platform/debug.h>
#include "bitwise.h"
#include "masks.h"


static uint8_t _build_control_register_a(bool ctc_bit, uint8_t cs1_bits, uint8_t com1a_bits, bool pwm1a_bit) {
    return ((ctc_bit << CTC1) & (MASK_CTC1)) |
            ((pwm1a_bit << PWM1A) & (MASK_PWM1A)) |
            ((com1a_bits << COM1A0) & (MASK_COM1A)) | 
            ((cs1_bits << MASK_CS1) & (MASK_CS1));
}

#define MASK_COM1B (1 << COM1B1) | (1 << COM1B0)
#define MASK_PWM1B (1 << PWM1B)

static uint8_t _build_general_control_register(uint8_t com1b_bits, bool pwm1b_bit) {
    return ((pwm1b_bit << pwm1b_bit) & (MASK_PWM1B)) |
            ((com1b_bits << COM1B0) & (MASK_COM1B));
}


static uint8_t _get_ctc_bit(timer1_mode_t mode) {
    switch (mode) {
    case TIMER1_MODE_NORMAL: return 0;
    case TIMER1_MODE_CTC: return 1;
    case TIMER1_MODE_PWM_VARIABLE_TOP: return 0;
    default: return 0;
    }
    return 0;
}

static uint8_t _get_cs1_bits(timer1_clock_t clock) {
    switch (clock) {
    case TIMER1_CLOCK_OFF: return 0b0000;
    case TIMER1_CLOCK_1: return 0b0001;
    case TIMER1_CLOCK_2: return 0b0010;
    case TIMER1_CLOCK_4: return 0b0011;
    case TIMER1_CLOCK_8: return 0b0100;
    case TIMER1_CLOCK_16: return 0b0101;
    case TIMER1_CLOCK_32: return 0b0110;
    case TIMER1_CLOCK_64: return 0b0111;
    case TIMER1_CLOCK_128: return 0b1000;
    case TIMER1_CLOCK_256: return 0b1001;
    case TIMER1_CLOCK_512: return 0b1010;
    case TIMER1_CLOCK_1024: return 0b1011;
    case TIMER1_CLOCK_2048: return 0b1100;
    case TIMER1_CLOCK_4096: return 0b1101;
    case TIMER1_CLOCK_8192: return 0b1110;
    case TIMER1_CLOCK_16384: return 0b1111;
    case TIMER1_CLOCK_PLL_64: return 0b0001;
    case TIMER1_CLOCK_PLL_32: return 0b0001; // do not prescale, just use low speed pll mode
    default: return 0;
    }
}

static uint8_t _get_pll_control_byte(timer1_clock_t clock) {
    switch (clock) {
    case TIMER1_CLOCK_PLL_64: return 0b00000110;
    case TIMER1_CLOCK_PLL_32: return 0b10000110;
    default: return 0;
    }
}


static uint8_t _get_com1a_bits(bool pwm1a) {
    if (pwm1a) {
        return 0b10;
    }
    return 0;
}

static uint8_t _get_com1b_bits(bool pwm1b) {
    if (pwm1b) {
        return 0b10;
    }
    return 0;
}


// CONTROL
void hal_timer1_set_control_registers(timer1_mode_t mode, timer1_clock_t clock, bool pwm1a, bool pwm1b) {
    DEBUG_PRINTLN("t1 ctrl", DEBUG_LAYER_HAL);
    uint8_t ctc_bit = _get_ctc_bit(mode);
    uint8_t com1a_bits = _get_com1a_bits(clock);
    uint8_t pllcsr_byte = _get_pll_control_byte(clock);
    uint8_t pwm1a_bit = pwm1a;
    uint8_t cs1_bits = _get_cs1_bits(clock);
    uint8_t com1b_bits = _get_com1b_bits(pwm1b);
    uint8_t pwm1b_bit = pwm1b;

    TCCR1 = _build_control_register_a(ctc_bit, cs1_bits, com1a_bits, pwm1a_bit);
    GTCCR = _build_general_control_register(com1b_bits, pwm1b_bit);
    PLLCSR = pllcsr_byte;
    DEBUG_PRINTLN_BIN("    TCCR1", TCCR1, DEBUG_LAYER_HAL);
    DEBUG_PRINTLN_BIN("    GTCCR", GTCCR, DEBUG_LAYER_HAL);
    DEBUG_PRINTLN_BIN("    PLLCSR", PLLCSR, DEBUG_LAYER_HAL);
}


// COUNTER

uint8_t hal_timer1_get_count(void) {
    return TCNT1;
}

void hal_timer1_set_count(uint8_t count) {
    DEBUG_PRINTLN("t1 count", DEBUG_LAYER_HAL);
    TCNT1 = count;
    DEBUG_PRINTLN_DEC("    TCNT1", TCNT1, DEBUG_LAYER_HAL);
}


// COMPARE REGISTER

void hal_timer1_set_output_compare_register_a(uint8_t value) {
    DEBUG_PRINTLN("t1 ocr1a", DEBUG_LAYER_HAL);
    OCR1A = value;
    DEBUG_PRINTLN_DEC("    OCR1A", OCR1A, DEBUG_LAYER_HAL);
}

void hal_timer1_set_output_compare_register_b(uint8_t value) {
    DEBUG_PRINTLN("t1 ocr1b", DEBUG_LAYER_HAL);
    OCR1A = value;
    DEBUG_PRINTLN_DEC("    OCR1B", OCR1B, DEBUG_LAYER_HAL);
}

void hal_timer1_set_output_compare_register_c(uint8_t value) {
    DEBUG_PRINTLN("t1 set ocr1c", DEBUG_LAYER_HAL);
    OCR1C = value;
    DEBUG_PRINTLN_DEC("    OCR1C", OCR1C, DEBUG_LAYER_HAL);
}


// INTERRUPTS
void hal_timer1_enable_interrupt_compa(bool enable) {
    DEBUG_PRINTLN("t1 int compa", DEBUG_LAYER_HAL);
    bitwise_write_bit(&TIMSK, OCIE1A, enable);
    DEBUG_PRINTLN_BIN("    TIMSK", TIMSK, DEBUG_LAYER_HAL);
}

void hal_timer1_enable_interrupt_compb(bool enable) {
    DEBUG_PRINTLN("t1 int compa", DEBUG_LAYER_HAL);
    bitwise_write_bit(&TIMSK, OCIE1B, enable);
    DEBUG_PRINTLN_BIN("    TIMSK", TIMSK, DEBUG_LAYER_HAL);
}

void hal_timer1_enable_interrupt_ovf(bool enable) {
    DEBUG_PRINTLN("t1 int compa", DEBUG_LAYER_HAL);
    bitwise_write_bit(&TIMSK, TOIE1, enable);
    DEBUG_PRINTLN_BIN("    TIMSK", TIMSK, DEBUG_LAYER_HAL);
}
