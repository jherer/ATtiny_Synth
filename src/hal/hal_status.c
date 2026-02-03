#include <hal/hal_status.h>
#include <platform/io.h>
#include <platform/debug.h>

uint8_t hal_status_get_sreg(void) {
    return SREG;
}

void hal_status_set_sreg(uint8_t new_sreg) {
    DEBUG_PRINTLN("set status reg", DEBUG_LAYER_HAL);
    SREG = new_sreg;
    DEBUG_PRINTLN_BIN("SREG", SREG, DEBUG_LAYER_HAL);
}

void hal_status_interrupt_enable(void) {
    DEBUG_PRINTLN("set sreg", DEBUG_LAYER_HAL);
    sei();
    DEBUG_PRINTLN_BIN("    SREG", SREG, DEBUG_LAYER_HAL);
}

void hal_status_interrupt_disable(void) {
    DEBUG_PRINTLN("set sreg", DEBUG_LAYER_HAL);
    cli();
    DEBUG_PRINTLN_BIN("    SREG", SREG, DEBUG_LAYER_HAL);
}