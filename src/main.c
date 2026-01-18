//#define F_CPU 16000000UL
#include "app/app.h"
#include "drivers/error.h"
#include "drivers/gpio.h"

#define DO_ERROR_BLINK

uint8_t get_error_code(error_t err) {
    switch (err) {
    case ERROR_OK:                      return 0;
    case ERROR_PROGRAM_END:             return 1;
    case ERROR_FUNCTION_UNSUPPORTED:    return 2;

    case ERROR_GPIO_MODE_UNSUPPORTED:   return 3;
    case ERROR_GPIO_WRITE_INPUT:        return 4;
    case ERROR_GPIO_UNINITIALIZED:      return 5;
    case ERROR_GPIO_NULL_POINTER:       return 6;

    case ERROR_TIMER_UNINITIALIZED:     return 7;
    case ERROR_TIMER_MODE_UNSUPPORTED:  return 8;
    case ERROR_TIMER_TOP_NOT_ALLOWED:   return 9;
    case ERROR_TIMER_OVF_NOT_ALLOWED:   return 10;
    case ERROR_TIMER_CLOCK_INVALID:     return 11;
    case ERROR_TIMER_EVENT_UNSUPPORTED: return 12;
    case ERROR_TIMER_ID_UNSUPPORTED:    return 13;

    case ERROR_PWM_UNINITIALIZED:       return 14;
    case ERROR_PWM_MODE_UNSUPPORTED:    return 15;
    case ERROR_PWM_MODE_INVALID_FOR_T1: return 16;
    case ERROR_PWM_MODE_TOP_INVALID:    return 17;
    case ERROR_PWM_COMPARE_ABOVE_TOP:   return 18;
    case ERROR_PWM_CHANNEL_UNSUPPORTED: return 19;
    case ERROR_PWM_CHANNEL_CONFLICT:    return 20;
    case ERROR_PWM_CHANNEL_A_IS_TOP:    return 21;

    case ERROR_SPI_MODE_UNSUPPORTED:    return 22;
    case ERROR_SPI_ROLE_UNSUPPORTED:    return 23;
    case ERROR_SPI_CLOCK_UNSUPPORTED:   return 24;
    case ERROR_SPI_CLOCK_INVALID:       return 25;
    case ERROR_SPI_STROBE_NOT_ALLOWED:  return 26;
    default: return 255;
    }
}

int main() {
    error_t init_err = app_init();
#ifdef DO_ERROR_BLINK
    if (init_err != ERROR_OK) {
        error_blink(get_error_code(init_err));
    }
#endif
    while (1) {
        error_t run_err = app_run();
#ifdef DO_ERROR_BLINK
        if (run_err != ERROR_OK) {
            error_blink(get_error_code(run_err));
        }
#endif
    }
    return 0;
}