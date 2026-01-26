
#include "platform.h"
#include "hal_timer0.h"
#include "bitwise.h"
#include "sim/debug.h"


// CONTROL

#define MASK_WGM0_TCCR0A (1 << WGM01) | (1 << WGM00)
#define MASK_COM0A (1 << COM0A1) | (1 << COM0A0)
#define MASK_COM0B (1 << COM0B1) | (1 << COM0B0)


static uint8_t _build_control_register_a(uint8_t wgm0_bits, uint8_t com0a_bits, uint8_t com0b_bits) {
    return ((wgm0_bits << WGM00) & (MASK_WGM0_TCCR0A)) |
            ((com0a_bits << COM0A0) & (MASK_COM0A)) |
            ((com0b_bits << COM0B0) & (MASK_COM0B));
}


#define MASK_WGM0_TCCR0B (1 << WGM02)
#define MASK_CS0 ((1 << CS02) | (1 << CS01) | (1 << CS00))

static uint8_t _build_control_register_b(uint8_t wgm0_bits, uint8_t cs0_bits) {
    uint8_t wgm02 = 0;
    if ((wgm0_bits & 0b100) != 0) {
        wgm02 = 1;
    }
    return ((cs0_bits << CS00) & MASK_CS0) |
            ((wgm02 << WGM02) & MASK_WGM0_TCCR0B);
}


void hal_timer0_set_control_registers(timer0_mode_t mode, timer0_clock_t clock, bool pwm_a, bool pwm_b) {
    debug_print("t0 ctrl");
    uint8_t wgm0_bits = 0;
    uint8_t cs0_bits = 0;
    uint8_t com0a_bits = 0;
    uint8_t com0b_bits = 0;

    switch (mode) {
    case TIMER0_MODE_NORMAL:
        wgm0_bits = 0b000;
        break;
    case TIMER0_MODE_CTC:
        wgm0_bits = 0b010;
        break;
    case TIMER0_MODE_PWM_FIXED_TOP:
        wgm0_bits = 0b011;
        break;
    case TIMER0_MODE_PWM_VARIABLE_TOP:
        wgm0_bits = 0b111;
        break;
    case NUM_TIMER0_MODES:
    default:
        break;
    }

    switch (clock) {
    case TIMER0_CLOCK_OFF:
        cs0_bits = 0b000;
        break;
    case TIMER0_CLOCK_1:
        cs0_bits = 0b001;
        break;
    case TIMER0_CLOCK_8:
        cs0_bits = 0b010;
        break;
    case TIMER0_CLOCK_64:
        cs0_bits = 0b011;
        break;
    case TIMER0_CLOCK_256:
        cs0_bits = 0b100;
        break;
    case TIMER0_CLOCK_1024:
        cs0_bits = 0b101;
        break;
    case NUM_TIMER0_CLOCKS:
    default:
        break;
    }

    if (pwm_a) {
        com0a_bits = 0b10;
    }
    if (pwm_b) {
        com0b_bits = 0b10;
    }

    TCCR0A = _build_control_register_a(wgm0_bits, com0a_bits, com0b_bits);
    debug_print_bin("   TCCR0A", TCCR0A);
    TCCR0B = _build_control_register_b(wgm0_bits, cs0_bits);
    debug_print_bin("   TCCR0B", TCCR0B);
}


// COUNTER 

uint8_t hal_timer0_get_count(void) {
    return TCNT0;
}

void hal_timer0_set_count(uint8_t count) {
    TCNT0 = count;
}


// COMPARE REGISTER

void hal_timer0_set_output_compare_register_a(uint8_t value) {
    debug_print("t0 ocr0a");
    OCR0A = value;
    debug_print_dec("   OCR0A", OCR0A);
}

void hal_timer0_set_output_compare_register_b(uint8_t value) {
    debug_print("t0 ocr0b");
    OCR0B = value;
    debug_print_dec("   OCR0B", OCR0B);
}

// INTERRUPTS

void hal_timer0_enable_interrupt_compa(bool enable) {
    debug_print("t0 int compa");
    bitwise_write_bit(&TIMSK, OCIE0A, enable);
    debug_print_bin("   TIMSK", TIMSK);
}

void hal_timer0_enable_interrupt_compb(bool enable) {
    debug_print("t0 interrupt compb");
    bitwise_write_bit(&TIMSK, OCIE0B, enable);
    debug_print_bin("   TIMSK", TIMSK);
}

void hal_timer0_enable_interrupt_ovf(bool enable) {
    debug_print("t0 interrupt ovf");
    bitwise_write_bit(&TIMSK, TOIE0, enable);
    debug_print_bin("   TIMSK", TIMSK);
}