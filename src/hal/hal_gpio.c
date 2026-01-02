#include "hal_gpio.h"
#include <avr/io.h>

// Extract port and pin from encoded value
#define GET_PORT(pin) ((pin) >> 4) // get high nibble
#define GET_PIN(pin)  ((pin) & 0x0F) // get low nibble

// Helper macros to get register addresses
#if defined(__AVR_ATtiny85__)
    #define PORT_REG(port) (&PORTB)
    #define DDR_REG(port)  (&DDRB)
    #define PIN_REG(port)  (&PINB)
#elif defined(__AVR_ATmega328P__)
    static volatile uint8_t* PORT_REG(uint8_t port) {
        switch(port) {
            case HAL_PORT_B: return &PORTB;
            case HAL_PORT_C: return &PORTC;
            case HAL_PORT_D: return &PORTD;
            default: return &PORTB;
        }
    }
    static volatile uint8_t* DDR_REG(uint8_t port) {
        switch(port) {
            case HAL_PORT_B: return &DDRB;
            case HAL_PORT_C: return &DDRC;
            case HAL_PORT_D: return &DDRD;
            default: return &DDRB;
        }
    }
    static volatile uint8_t* PIN_REG(uint8_t port) {
        switch(port) {
            case HAL_PORT_B: return &PINB;
            case HAL_PORT_C: return &PINC;
            case HAL_PORT_D: return &PIND;
            default: return &PINB;
        }
    }
#endif


void hal_gpio_mode_input(hal_pin_t pin) {
    uint8_t pin_num = GET_PIN(pin);
    uint8_t port_num = GET_PORT(pin);
    volatile uint8_t* ddr = DDR_REG(port_num);
    _clear_bit(ddr, pin_num);
}

void hal_gpio_mode_output(hal_pin_t pin) {
    uint8_t pin_num = GET_PIN(pin);
    uint8_t port_num = GET_PORT(pin);
    volatile uint8_t* ddr = DDR_REG(port_num);
    _set_bit(ddr, pin_num);
}

bool hal_gpio_read(hal_pin_t pin) {
    uint8_t pin_num = GET_PIN(pin);
    uint8_t port_num = GET_PORT(pin);
    volatile uint8_t* pin_reg = PIN_REG(port_num);
    return _read_bit(pin_reg, pin_num);
}

void hal_gpio_set(hal_pin_t pin) {
    uint8_t pin_num = GET_PIN(pin);
    uint8_t port_num = GET_PORT(pin);
    volatile uint8_t* port_reg = PORT_REG(port_num);
    _set_bit(port_reg, pin_num);
}

void hal_gpio_clear(hal_pin_t pin) {
    uint8_t pin_num = GET_PIN(pin);
    uint8_t port_num = GET_PORT(pin);
    volatile uint8_t* port_reg = PORT_REG(port_num);
    _clear_bit(port_reg, pin_num);
}

void hal_gpio_toggle(hal_pin_t pin) {
    uint8_t pin_num = GET_PIN(pin);
    uint8_t port_num = GET_PORT(pin);
    volatile uint8_t* port_reg = PORT_REG(port_num);
    _toggle_bit(port_reg, pin_num);
}
