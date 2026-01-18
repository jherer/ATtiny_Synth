#include "hal_timer0.h"
#include <avr/io.h>
#include "bitwise.h"


// WAVE GENERATION MODE

void hal_timer0_set_wave_gen_mode(uint8_t wgm_bits) {
    bitwise_write_bit(&TCCR0A, WGM00, wgm_bits & (1 << 0));
    bitwise_write_bit(&TCCR0A, WGM01, wgm_bits & (1 << 1));
    bitwise_write_bit(&TCCR0B, WGM02, wgm_bits & (1 << 2));
}


// CLOCK SELECT

void hal_timer0_set_clock_select(uint8_t cs_bits) {
    bitwise_write_bit(&TCCR0B, CS00, cs_bits & (1 << 0));
    bitwise_write_bit(&TCCR0B, CS01, cs_bits & (1 << 1));
    bitwise_write_bit(&TCCR0B, CS02, cs_bits & (1 << 2));
}


// COUNTER 

uint8_t hal_timer0_get_count(void) {
    return TCNT0;
}

void hal_timer0_set_count(uint8_t count) {
    TCNT0 = count;
}


// COMPARE REGISTER

void hal_timer0_set_output_compare_register_A(uint8_t ocr0a_value) {
    OCR0A = ocr0a_value;
}

void hal_timer0_set_output_compare_register_B(uint8_t ocr0b_value) {
    OCR0B = ocr0b_value;
}


// COMPARE MODE

void hal_timer0_set_compare_output_mode_A(uint8_t com0a_bits) {
    bitwise_write_bit(&TCCR0A, COM0A0, com0a_bits & (1 << 0));
    bitwise_write_bit(&TCCR0A, COM0A1, com0a_bits & (1 << 1));
}

void hal_timer0_set_compare_output_mode_B(uint8_t com0b_bits) {
    bitwise_write_bit(&TCCR0A, COM0B0, com0b_bits & (1 << 0));
    bitwise_write_bit(&TCCR0A, COM0B1, com0b_bits & (1 << 1));
}


// INTERRUPTS

void hal_timer0_enable_interrupt_compA(bool enable) {
    bitwise_write_bit(&TIMSK, OCIE0A, enable);
}

void hal_timer0_enable_interrupt_compB(bool enable) {
    bitwise_write_bit(&TIMSK, OCIE0B, enable);
}

void hal_timer0_enable_interrupt_ovf(bool enable) {
    bitwise_write_bit(&TIMSK, TOIE0, enable);
}
