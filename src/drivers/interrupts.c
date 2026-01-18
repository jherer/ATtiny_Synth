#include <avr/interrupt.h>
#include <stdbool.h>

void interrupts_enable(void) {
    sei();
}

void interrupts_disable(void) {
    cli();
}