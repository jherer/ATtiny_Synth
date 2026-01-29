#include "hal_gpio.h"
#include "bitwise.h"
#include "platform.h"
#include "sim/debug.h"

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
    debug_println_hex("gpio write ddr", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(DDR_REG(port_num), pin_num, state);
    debug_println_bin("    DDRB", DDRB, DEBUG_LAYER_HAL);
}

void hal_gpio_write_port(gpio_id_t gpio_id, bool state) {
    debug_println_hex("gpio write port", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(PORT_REG(port_num), pin_num, state);
    debug_println_bin("    PORTB", PORTB, DEBUG_LAYER_HAL);
}

void hal_gpio_toggle_port(gpio_id_t gpio_id) {
    debug_println_hex("gpio toggle port", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    bitwise_write_bit(PIN_REG(port_num), pin_num, 1);
    debug_println_bin("    PORTB", PORTB, DEBUG_LAYER_HAL);
}

bool hal_gpio_read_pin(gpio_id_t gpio_id) {
    debug_println_hex("gpio read pin", gpio_id, DEBUG_LAYER_HAL);
    uint8_t pin_num = GET_PIN(gpio_id);
    uint8_t port_num = GET_PORT(gpio_id);
    return bitwise_read_bit(*(PIN_REG(port_num)), pin_num);
    debug_println_bin("    PINB", PINB, DEBUG_LAYER_HAL);
}

