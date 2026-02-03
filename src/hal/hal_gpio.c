#include <hal/hal_gpio.h>
#include <platform/io.h>
#include <platform/debug.h>
#include "bitwise.h"

// Helper macros to get register addresses
static volatile uint8_t* PORT_REG(uint8_t port) {
    return &PORTB;
}
    
static volatile uint8_t* PIN_REG(uint8_t port) {
    return &PINB;
}

static volatile uint8_t* DDR_REG(uint8_t port) {
    return &DDRB;
}

// Extract port and pin from encoded value
#define GET_PORT(pin) ((pin) >> 4) // get high nibble
#define GET_PIN(pin)  ((pin) & 0x0F) // get low nibble

void hal_gpio_write_ddr(gpio_id_t gpio_id, bool state) {
    DEBUG_PRINTLN_HEX("gpio write ddr", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(DDR_REG(port_num), pin_num, state);
    DEBUG_PRINTLN_BIN("    DDRB", DDRB, DEBUG_LAYER_HAL);
}

void hal_gpio_write_port(gpio_id_t gpio_id, bool state) {
    DEBUG_PRINTLN_HEX("gpio write port", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(PORT_REG(port_num), pin_num, state);
    DEBUG_PRINTLN_BIN("    PORTB", PORTB, DEBUG_LAYER_HAL);
}

void hal_gpio_toggle_port(gpio_id_t gpio_id) {
    DEBUG_PRINTLN_HEX("gpio toggle port", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(PIN_REG(port_num), pin_num, 1);
    DEBUG_PRINTLN_BIN("    PORTB", PORTB, DEBUG_LAYER_HAL);
}

bool hal_gpio_read_pin(gpio_id_t gpio_id) {
    DEBUG_PRINTLN_HEX("gpio read pin", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    return bitwise_read_bit(*(PIN_REG(port_num)), pin_num);
    DEBUG_PRINTLN_BIN("    PINB", PINB, DEBUG_LAYER_HAL);
}

