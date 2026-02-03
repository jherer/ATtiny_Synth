#include <hal/hal_delay.h>
#include <platform/io.h>

void hal_delay_smart_ms(uint16_t ms) {
    #ifdef SIM
        _delay_ms(ms);
    #endif

    #ifdef HW
        for (int i = 0; i < ms; i++) {
            _delay_ms(1);
        }
    #endif
}
