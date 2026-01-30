//#define F_CPU 16000000UL

#include "app/app.h"
#include "core/error_code.h"
#include "drivers/gpio_driver.h"
#include "drivers/timer0_driver.h"
#include "drivers/timer1_driver.h"
#include "sim/debug.h"

static void _print_columns(void) {
    debug_println("APP\t\t\tSERVICES\t\tDRIVERS\t\t\tHAL", DEBUG_LAYER_MAIN);
    debug_println(  "---\t\t\t--------\t\t-------\t\t\t---", DEBUG_LAYER_MAIN);
}

int main(void) {
    {   
        debug_init();
        debug_println_title("ATtiny85 Simulator", 55, DEBUG_LAYER_MAIN);
        debug_pause("Press ENTER to init", DEBUG_LAYER_MAIN);
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
    debug_pause("Press ENTER to step", DEBUG_LAYER_MAIN);
    uint32_t i = 0;
    while (1) {
        debug_println_dec("Step", i++, DEBUG_LAYER_MAIN);
        _print_columns();
        error_code_t run_err = app_run();
        
        if (run_err != ERROR_OK) {
            debug_println_hex("RUN ERROR", run_err, DEBUG_LAYER_MAIN);
        }

        char timer_char = debug_query_char("Next step (ENTER)    T0 ISR (0)    T1 ISR (1)", DEBUG_LAYER_MAIN);
        bool use_t0 = timer_char == '0';
        bool use_t1 = timer_char == '1';

        while (use_t0 || use_t1) {
            char isr_char = debug_query_char("COMPA (a)    COMPB (b)    OVF (o)", DEBUG_LAYER_MAIN);
            bool isr_compa = isr_char == 'a';
            bool isr_compb = isr_char == 'b';
            bool isr_ovf = isr_char == 'o';

            if (use_t0) {
                if (isr_compa) {
                    debug_println("Timer0 ISR COMPA Fired", DEBUG_LAYER_MAIN);
                    timer0_fire_isr_compa(); break;
                } else if (isr_compb) {
                    debug_println("Timer0 ISR COMPB Fired", DEBUG_LAYER_MAIN);
                    timer0_fire_isr_compb(); break;
                } else if (isr_ovf) {
                    debug_println("Timer0 ISR OVF Fired", DEBUG_LAYER_MAIN);
                    timer0_fire_isr_ovf(); break;
                } else {
                    debug_println("Invalid entry", DEBUG_LAYER_MAIN); break;
                }
            } else if (use_t1) {
                if (isr_compa) {
                    debug_println("Timer1 ISR COMPA Fired", DEBUG_LAYER_MAIN);
                    timer1_fire_isr_compa(); break;
                } else if (isr_compb) {
                    debug_println("Timer1 ISR COMPB Fired", DEBUG_LAYER_MAIN);
                    timer1_fire_isr_compb(); break;
                } else if (isr_ovf) {
                    debug_println("Timer1 ISR OVF Fired", DEBUG_LAYER_MAIN);
                    timer1_fire_isr_ovf(); break;
                } else {
                    debug_println("Invalid entry", DEBUG_LAYER_MAIN); break;
                }
            }
        }
        debug_println("\n---------------\n", DEBUG_LAYER_MAIN);
    }

    return 0;
}