#include "hal_status.h"
#include "platform.h"
#include "sim/debug.h"

uint8_t hal_status_get_sreg(void) {
    return SREG;
}

void hal_status_set_sreg(uint8_t new_sreg) {
    debug_println("set status reg", DEBUG_LAYER_HAL);
    SREG = new_sreg;
    debug_println_bin("SREG", SREG, DEBUG_LAYER_HAL);
}

void hal_status_interrupts_enable(void) {
    debug_println("set sreg", DEBUG_LAYER_HAL);
    sei();
    debug_println_bin("    SREG", SREG, DEBUG_LAYER_HAL);
}

void hal_status_interrupts_disable(void) {
    debug_println("set sreg", DEBUG_LAYER_HAL);
    cli();
    debug_println_bin("    SREG", SREG, DEBUG_LAYER_HAL);
}