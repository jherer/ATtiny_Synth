#ifndef HAL_GPIO_H
#define HAL_GPIO_H
#include <avr/io.h>
#include <stdbool.h>

typedef enum {
#ifdef PORTA
    HAL_PORT_A = 0,
#endif
#ifdef PORTB
    HAL_PORT_B = 1,
#endif
#ifdef PORTC
    HAL_PORT_C = 2,
#endif
#ifdef PORTD
    HAL_PORT_D = 3,
#endif
#ifdef PORTE
    HAL_PORT_E = 4,
#endif
#ifdef PORTF
    HAL_PORT_F = 4,
#endif
} hal_port_t;


// High nibble is port (A=0, B=1, etc)
// Low nibble is pin number (0, 1, 2, etc)
typedef enum {
#ifdef PORTA
    HAL_PIN_A0 = 0x00,
    HAL_PIN_A1 = 0x01,
    HAL_PIN_A2 = 0x02,
    HAL_PIN_A3 = 0x03,
    HAL_PIN_A4 = 0x04,
    HAL_PIN_A5 = 0x05,
#endif
#ifdef PORTB
    HAL_PIN_B0 = 0x10,
    HAL_PIN_B1 = 0x11,
    HAL_PIN_B2 = 0x12,
    HAL_PIN_B3 = 0x13,
    HAL_PIN_B4 = 0x14,
    HAL_PIN_B5 = 0x15,
#endif
#ifdef PORTC
    HAL_PIN_C0 = 0x20,
    HAL_PIN_C1 = 0x21,
    HAL_PIN_C2 = 0x22,
    HAL_PIN_C3 = 0x23,
    HAL_PIN_C4 = 0x24,
    HAL_PIN_C5 = 0x25,
#endif
#ifdef PORTD
    HAL_PIN_D0 = 0x30,
    HAL_PIN_D1 = 0x31,
    HAL_PIN_D2 = 0x32,
    HAL_PIN_D3 = 0x33,
    HAL_PIN_D4 = 0x34,
    HAL_PIN_D5 = 0x35,
#endif
#ifdef PORTE
    HAL_PIN_E0 = 0x40,
    HAL_PIN_E1 = 0x41,
    HAL_PIN_E2 = 0x42,
    HAL_PIN_E3 = 0x43,
    HAL_PIN_E4 = 0x44,
    HAL_PIN_E5 = 0x45,
#endif
#ifdef PORTF
    HAL_PIN_F0 = 0x50,
    HAL_PIN_F1 = 0x51,
    HAL_PIN_F2 = 0x52,
    HAL_PIN_F3 = 0x53,
    HAL_PIN_F4 = 0x54,
    HAL_PIN_F5 = 0x55,
#endif
} hal_pin_t;


// Private helpers, inlined for efficiency

static inline bool _read_bit(volatile uint8_t *reg, uint8_t bit) {
    return (*reg & (1 << bit)) != 0;
}

static inline void _set_bit(volatile uint8_t *reg, uint8_t bit) {
    *reg |= 1 << bit;
}

static inline void _clear_bit(volatile uint8_t *reg, uint8_t bit) {
    *reg &= ~(1 << bit);
}

static inline void _toggle_bit(volatile uint8_t *reg, uint8_t bit) {
    *reg ^= (1 << bit);
}

void hal_gpio_mode_input(hal_pin_t pin);
void hal_gpio_mode_output(hal_pin_t pin);

bool hal_gpio_read(hal_pin_t pin);
void hal_gpio_set(hal_pin_t pin);
void hal_gpio_clear(hal_pin_t pin);
void hal_gpio_toggle(hal_pin_t pin);
#endif