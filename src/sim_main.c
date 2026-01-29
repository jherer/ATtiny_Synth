//#define F_CPU 16000000UL

#include "app/app.h"
#include "core/error_code.h"
#include "drivers/gpio.h"
#include "sim/debug.h"

static void _print_columns(void) {
    debug_println("APP\t\t\tSERVICES\t\tDRIVERS\t\t\tHAL", DEBUG_LAYER_MAIN);
    debug_println(  "---\t\t\t--------\t\t-------\t\t\t---", DEBUG_LAYER_MAIN);
}

int main(void) {
    {   
        debug_init();
        debug_println_title("ATtiny85 Simulator", 55, DEBUG_LAYER_MAIN);
        debug_pause("Press enter to init", DEBUG_LAYER_MAIN);
        debug_println_title("INIT", 45, DEBUG_LAYER_MAIN);
        _print_columns();

        error_code_t init_err = app_init();

        if (init_err == ERROR_OK) {
            debug_println("Init success!", DEBUG_LAYER_MAIN);
        } else {
            debug_println_hex("INIT ERROR", init_err, DEBUG_LAYER_MAIN);
        }
    }
    debug_delay_ms(500);

    debug_println_title("RUN", 45, DEBUG_LAYER_MAIN);
    debug_pause("Press enter to step", DEBUG_LAYER_MAIN);
    uint32_t i = 0;
    while (1) {
        debug_println_dec("Step", i++, DEBUG_LAYER_MAIN);
        _print_columns();
        error_code_t run_err = app_run();
        
        if (run_err != ERROR_OK) {
            debug_println_hex("RUN ERROR", run_err, DEBUG_LAYER_MAIN);
        }
        debug_pause("---------------", DEBUG_LAYER_MAIN);
        debug_println("", DEBUG_LAYER_MAIN);
    }

    return 0;
}