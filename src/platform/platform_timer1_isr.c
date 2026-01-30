#ifdef HW
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers/timer1_driver.h"

ISR(TIMER1_COMPA_vect) {
    timer1_fire_isr_compa();
}

ISR(TIMER1_COMPB_vect) {
    timer1_fire_isr_compb();
}

ISR(TIMER1_OVF_vect) {
    timer1_fire_isr_ovf();
}

#endif