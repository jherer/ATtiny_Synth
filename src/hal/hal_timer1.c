#include "hal_timer1.h"
#include <avr/io.h>
#include "bitwise.h"


// WAVE GENERATION MODE
void hal_timer1_set_ctc(bool ctc_bit) {
    bitwise_write_bit(&TCCR1, CTC1, ctc_bit);
}

void hal_timer1_set_pwm_1A(bool pwm1A_bit) {
    bitwise_write_bit(&TCCR1, PWM1A, pwm1A_bit);
}

void hal_timer1_set_pwm_1B(bool pwm1B_bit) {
    bitwise_write_bit(&GTCCR, PWM1B, pwm1B_bit);
}

// CLOCK SELECT
void hal_timer1_set_clock_select(uint8_t cs_bits) {
    bitwise_write_bit(&TCCR1, CS10, cs_bits & (1 << 0));
    bitwise_write_bit(&TCCR1, CS11, cs_bits & (1 << 1));
    bitwise_write_bit(&TCCR1, CS12, cs_bits & (1 << 2));
    bitwise_write_bit(&TCCR1, CS13, cs_bits & (1 << 3));
}

void hal_timer1_enable_PLL(bool enable) {
    bitwise_write_bit(&PLLCSR, PCKE, enable);
    bitwise_write_bit(&PLLCSR, PLLE, enable);
}


// COUNTER
uint8_t hal_timer1_get_count(void) {
    return TCNT1;
}

void hal_timer1_set_count(uint8_t count) {
    TCNT1 = count;
}


// COMPARE REGISTER

void hal_timer1_set_output_compare_register_A(uint8_t ocr1a_value) {
    OCR1A = ocr1a_value;
}

void hal_timer1_set_output_compare_register_B(uint8_t ocr1b_value) {
    OCR1B = ocr1b_value;
}

void hal_timer1_set_output_compare_register_C(uint8_t ocr1c_value) {
    OCR1C = ocr1c_value;
}

// COMPARE MODE

void hal_timer1_set_compare_output_mode_A(uint8_t com1a_bits) {
    bitwise_write_bit(&TCCR1, COM1A0, com1a_bits & (1 << 0));
    bitwise_write_bit(&TCCR1, COM1A1, com1a_bits & (1 << 1));
}

void hal_timer1_set_compare_output_mode_B(uint8_t com1b_bits) {
    bitwise_write_bit(&GTCCR, COM1B0, com1b_bits & (1 << 0));
    bitwise_write_bit(&GTCCR, COM1B1, com1b_bits & (1 << 1));
}


// INTERRUPTS

void hal_timer1_enable_interrupt_compA(bool enable) {
    bitwise_write_bit(&TIMSK, OCIE1A, enable);
}

void hal_timer1_enable_interrupt_compB(bool enable) {
    bitwise_write_bit(&TIMSK, OCIE1B, enable);
}

void hal_timer1_enable_interrupt_ovf(bool enable) {
    bitwise_write_bit(&TIMSK, TOIE1, enable);
}