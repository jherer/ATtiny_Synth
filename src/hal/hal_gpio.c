#include "hal_gpio.h"
#include <avr/io.h>
#include "bitwise.h"

// Extract port and pin from encoded value
#define GET_PORT(pin) ((pin) >> 4) // get high nibble
#define GET_PIN(pin)  ((pin) & 0x0F) // get low nibble

// Helper macros to get register addresses
static volatile uint8_t* PORT_REG(uint8_t port) {
    switch(port) {      
#ifdef PORTA
            case 0x0: return &PORTA;
#endif
#ifdef PORTB
            case 0x1: return &PORTB;
#endif
#ifdef PORTC
            case 0x2: return &PORTC;
#endif
#ifdef PORTD
            case 0x3: return &PORTD;
#endif
#ifdef PORTE
            case 0x4: return &PORTE;
#endif
#ifdef PORTF
            case 0x5: return &PORTF;
#endif
        default: return &PORTB;
    }
}
static volatile uint8_t* DDR_REG(uint8_t port) {
    switch(port) {      
#ifdef PORTB
        case 0x1: return &DDRB;
#endif
#ifdef PORTC
        case 0x2: return &DDRC;
#endif
#ifdef PORTD
        case 0x3: return &DDRD;
#endif

        default: return &DDRB;
    }
}
    
static volatile uint8_t* PIN_REG(uint8_t port) {
    switch(port) {      
#ifdef PORTB
        case 0x1: return &PINB;
#endif
#ifdef PORTC
        case 0x2: return &PINC;
#endif
#ifdef PORTD
        case 0x3: return &PIND;
#endif
        default: return &PINB;
    }
}

void hal_gpio_write_ddr(gpio_id_t gpio_id, bool state) {
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(DDR_REG(port_num), pin_num, state);
}

void hal_gpio_write_port(gpio_id_t gpio_id, bool state) {
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(PORT_REG(port_num), pin_num, state);
}

void hal_gpio_toggle_port(gpio_id_t gpio_id) {
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(PIN_REG(port_num), pin_num, 1);
}

bool hal_gpio_read_pin(gpio_id_t gpio_id) {
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    return bitwise_read_bit(*(PIN_REG(port_num)), pin_num);
}