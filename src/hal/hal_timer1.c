#include "hal_timer1.h"
#include "platform.h"
#include "bitwise.h"

#define MASK_CTC1 (1 << CTC1)
#define MASK_PWM1A (1 << PWM1A)
#define MASK_COM1A (1 << COM1A1) | (1 << COM1A0)
#define MASK_CS1 (1 << CS13) | (1 << CS12) | (1 << CS11) | (1 << CS10)

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
            ((com1b_bits << COM1B0) & (MASK_COM1B))
    ;
}


// CONTROL
void hal_timer1_set_control_registers(timer1_mode_t mode, timer1_clock_t clock, bool pwm1a, bool pwm1b) {
    uint8_t tccr1_bits = 0;
    uint8_t ctc_bit = 0;
    uint8_t com1a_bits = 0;
    uint8_t pwm1a_bit = 0;
    uint8_t cs1_bits = 0;

    uint8_t gtccr_bits = 0;
    uint8_t com1b_bits = 0;
    uint8_t pwm1b_bit = 0;
    switch (mode) {
    case TIMER1_MODE_NORMAL:
        ctc_bit = 0;
        break;
    case TIMER1_MODE_CTC:
        ctc_bit = 1;
        break;
    case TIMER1_MODE_PWM_VARIABLE_TOP:
        ctc_bit = 0;
        break;
    case NUM_TIMER1_MODES:
    default:
        break;
    }
    
    switch (clock) {
    case TIMER1_CLOCK_OFF:
        cs1_bits = 0b0000;
        break;
    case TIMER1_CLOCK_1:
        cs1_bits = 0b0001;
        break;
    case TIMER1_CLOCK_2:
        cs1_bits = 0b0010;
        break;
    case TIMER1_CLOCK_4:
        cs1_bits = 0b0011;
        break;
    case TIMER1_CLOCK_8:
        cs1_bits = 0b0100;
        break;
    case TIMER1_CLOCK_16:
        cs1_bits = 0b0101;
        break;
    case TIMER1_CLOCK_32:
        cs1_bits = 0b0110;
        break;
    case TIMER1_CLOCK_64:
        cs1_bits = 0b0111;
        break;
    case TIMER1_CLOCK_128:
        cs1_bits = 0b1000;
        break;
    case TIMER1_CLOCK_256:
        cs1_bits = 0b1001;
        break;
    case TIMER1_CLOCK_512:
        cs1_bits = 0b1010;
        break;
    case TIMER1_CLOCK_1024:
        cs1_bits = 0b1011;
        break;
    case TIMER1_CLOCK_2048:
        cs1_bits = 0b1100;
        break;
    case TIMER1_CLOCK_4096:
        cs1_bits = 0b1101;
        break;
    case TIMER1_CLOCK_8192:
        cs1_bits = 0b1110;
        break;
    case TIMER1_CLOCK_16834:
        cs1_bits = 0b1111;
        break;
    case NUM_TIMER1_CLOCKS:
    default:
        break;
    }

    if (pwm1a) {
        pwm1a_bit = 1;
        com1a_bits = 0b10;
    }
    if (pwm1b) {
        pwm1b_bit = 1;
        com1b_bits = 0b10;
    }

    TCCR1 |= _build_control_register_a(ctc_bit, cs1_bits, com1a_bits, pwm1a_bit);
    GTCCR |= _build_general_control_register(com1b_bits, pwm1b_bit);
}


// COUNTER

uint8_t hal_timer1_get_count(void) {
    return TCNT1;
}

void hal_timer1_set_count(uint8_t count) {
    TCNT1 = count;
}


// COMPARE REGISTER

void hal_timer1_set_output_compare_register_a(uint8_t value) {
    OCR1A = value;
}

void hal_timer1_set_output_compare_register_b(uint8_t value) {
    OCR1A = value;
}

void hal_timer1_set_output_compare_register_c(uint8_t value) {
    OCR1C = value;
}


// INTERRUPTS
void hal_timer1_enable_interrupt_compa(bool enable) {
    bitwise_write_bit(&TIMSK, OCIE1A, enable);
}

void hal_timer1_enable_interrupt_compb(bool enable) {
    bitwise_write_bit(&TIMSK, OCIE1B, enable);
}

void hal_timer1_enable_interrupt_ovf(bool enable) {
    bitwise_write_bit(&TIMSK, TOIE1, enable);
}
