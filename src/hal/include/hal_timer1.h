#include "hal/timer1_enums.h"
#include <stdbool.h>
#include <stdint.h>

// CONTROL
void hal_timer1_set_control_registers(timer1_mode_t mode, timer1_clock_t clock, bool pwm1a, bool pwm1b);

// COUNTER 
uint8_t hal_timer1_get_count(void);
void hal_timer1_set_count(uint8_t count);

// COMPARE REGISTER
void hal_timer1_set_output_compare_register_a(uint8_t value);
void hal_timer1_set_output_compare_register_b(uint8_t value);
void hal_timer1_set_output_compare_register_c(uint8_t value);

// INTERRUPTS
void hal_timer1_enable_interrupt_compa(bool enable);
void hal_timer1_enable_interrupt_compb(bool enable);
void hal_timer1_enable_interrupt_ovf(bool enable);
