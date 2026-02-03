#ifdef HW
#include <avr/io.h>
#include <avr/interrupt.h>
#include <drivers/timer0_driver.h>

ISR(TIMER0_COMPA_vect) {
    timer0_fire_isr_compa();
}

ISR(TIMER0_COMPB_vect) {
    timer0_fire_isr_compb();
}

ISR(TIMER0_OVF_vect) {
    timer0_fire_isr_ovf();
}

#endif