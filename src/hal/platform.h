#pragma once
#include <stdint.h>

#ifndef HAL_SIM
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>

    #define 





#endif

#ifdef HAL_SIM
    #include "sim/io.h"
    #include "sim/debug.h"
    static inline void _delay_ms(uint16_t time_ms) {
        debug_delay_ms(time_ms);
    }
#endif

