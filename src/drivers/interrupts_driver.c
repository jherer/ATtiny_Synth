#include "drivers/interrupts_driver.h"
#include "hal_status.h"
#include "sim/debug.h"

void interrupts_enable(void) {
    debug_println("enable interrupts", DEBUG_LAYER_DRIVERS);
    hal_status_interrupts_enable();
}

void interrupts_disable(void) {
    debug_println("disable interrupts", DEBUG_LAYER_DRIVERS);
    hal_status_interrupts_disable();
}