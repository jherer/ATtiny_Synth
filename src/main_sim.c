//#define F_CPU 16000000UL

#include <app/app.h>
#include <drivers/gpio_driver.h>
#include <drivers/timer0_driver.h>
#include <drivers/timer1_driver.h>
#include <sim/sim_timer0.h>
#include <platform/io.h>
#include <platform/debug.h>

static void _print_columns(void) {
    DEBUG_PRINTLN("SIM\t\t\tAPP\t\t\tSERVICES\t\tDRIVERS\t\t\tHAL", DEBUG_LAYER_MAIN);
    DEBUG_PRINTLN(  "---\t\t\t---\t\t\t--------\t\t-------\t\t\t---", DEBUG_LAYER_MAIN);
}


#define MICROS_PER_LOOP 100000

void process_input(char c);

int main(void) {
uint64_t microseconds = 0;
    {   
        _debug_init();
        _debug_println_title("ATtiny85 Simulator", 55, DEBUG_LAYER_MAIN);
        _debug_pause("Press ENTER to init", DEBUG_LAYER_MAIN);
        _debug_println_title("INIT", 45, DEBUG_LAYER_MAIN);
        _print_columns();

        error_code_t init_err = app_init();

        if (init_err == ERROR_OK) {
            DEBUG_PRINTLN("Init success!", DEBUG_LAYER_MAIN);
        } else {
            DEBUG_PRINTLN_HEX("INIT ERROR", init_err, DEBUG_LAYER_MAIN);
        }
    }

    _debug_pause("Press ENTER to run", DEBUG_LAYER_MAIN);
    _debug_println_title("RUN", 45, DEBUG_LAYER_MAIN);
    uint32_t i = 0;
    while (1) {
        DEBUG_PRINTLN_DEC("Step", i++, DEBUG_LAYER_MAIN);
        _print_columns();
        error_code_t run_err = app_run();
        
        if (run_err != ERROR_OK) {
            DEBUG_PRINTLN_HEX("RUN ERROR", run_err, DEBUG_LAYER_MAIN);
        }
        DEBUG_PRINTLN("", DEBUG_LAYER_MAIN);
        char response = _debug_query_char("Next step (ENTER)    T0 tick (a)    T1 tick (s)    Dump (d)", DEBUG_LAYER_MAIN);
        process_input(response);
        DEBUG_PRINTLN("\n---------------\n", DEBUG_LAYER_MAIN);
        microseconds += MICROS_PER_LOOP;
    }

    return 0;
}


void process_timer0_events(sim_timer0_events_t event, uint16_t ticks) {
    if (event.compa) {
        //DEBUG_PRINTLN_DEC("T0 compa isr ran at tick", ticks, DEBUG_LAYER_MAIN);
        timer0_fire_isr_compa();
    }
    if (event.compb) {
        //DEBUG_PRINTLN_DEC("T0 compb isr ran attick", ticks, DEBUG_LAYER_MAIN);
        timer0_fire_isr_compb();
    }
    if (event.ovf) {
        //DEBUG_PRINTLN_DEC("T0 ovf isr ran at tick", ticks, DEBUG_LAYER_MAIN);
        timer0_fire_isr_ovf();
    }
}

int char_digit_to_int(char c) {
    switch (c) {
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case '0': return 0;
    default: return 10;
    }
}

void process_input(char c) {
    if (c == 'd') {
        _debug_dump_registers(&sim_io_state);
    }
    bool tcnt0 = c == 'a';
    bool tcnt1 = c == 's';
    int ticks = 0;
    if (tcnt0 || tcnt1) {
        ticks = _debug_query_int("Enter # ticks to run (0 resets the counter)", DEBUG_LAYER_MAIN);
    }
    if (tcnt0) {
        if (ticks == 0) {
            sim_io_state.TCNT0_SIM = 0;
            DEBUG_PRINTLN_DEC("T0 cleared", ticks, DEBUG_LAYER_MAIN);
        } else if (ticks <= 10000000) {
            for (int i = 0; i < ticks; i++) {
                process_timer0_events(sim_timer0_tick(&sim_io_state), i);
            }
            DEBUG_PRINTLN_DEC("T0 tick ran", ticks, DEBUG_LAYER_MAIN);
        } else {
            DEBUG_PRINTLN_DEC("Invalid # ticks entered", ticks, DEBUG_LAYER_MAIN);
        }
        DEBUG_PRINTLN_DEC("TCNT0", sim_io_state.TCNT0_SIM, DEBUG_LAYER_MAIN);
    }
    if (tcnt1) {
        DEBUG_PRINTLN_DEC("T1 NOT IMPLEMENTED", TCNT1, DEBUG_LAYER_MAIN);
    }
    return;
}