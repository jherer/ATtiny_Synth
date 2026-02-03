#include <sim/sim_timer0.h>
#include <platform/debug.h>
#include <stdbool.h>
#include <core/error_code.h>

static timer0_mode_t _get_mode() {
    return TIMER0_MODE_CTC;
}

static timer0_clock_t _get_clock() {
    return TIMER0_CLOCK_1024;
}

static sim_timer0_events_t _tick(sim_io_state_t *sim) {
    DEBUG_PRINTLN("sim t0 tick", DEBUG_LAYER_SIM_ISR);
    timer0_mode_t mode = _get_mode();
    sim_timer0_events_t events = {0};

    if (_get_clock() == TIMER0_CLOCK_OFF) {
        return events;
    }
    sim->TCNT0_SIM++;

    if (sim->TCNT0_SIM >= sim->OCR0A_SIM) {
        DEBUG_PRINTLN_DEC("    compa @ tcnt0", sim->TCNT0_SIM, DEBUG_LAYER_SIM_ISR);
        if (mode == TIMER0_MODE_CTC || mode == TIMER0_MODE_PWM_VARIABLE_TOP) {
            sim->TCNT0_SIM = 0;
            DEBUG_PRINTLN_DEC("    tcnt0 cleared", sim->TCNT0_SIM, DEBUG_LAYER_SIM_ISR);
        }
        events.compa = true;
    }
    if (sim->TCNT0_SIM >= sim->OCR0B_SIM) {
        DEBUG_PRINTLN_DEC("    compb @ tcnt0", sim->TCNT0_SIM, DEBUG_LAYER_SIM_ISR);
        events.compb = true;
    }
    if (sim->TCNT0_SIM == 0) {
        if (mode == TIMER0_MODE_NORMAL || mode == TIMER0_MODE_PWM_FIXED_TOP) {
            DEBUG_PRINTLN_DEC("    ovf @ tcnt0", sim->TCNT0_SIM, DEBUG_LAYER_SIM_ISR);
            events.ovf = true;
        }
    }
    
    return events;
}

sim_timer0_events_t sim_timer0_tick(sim_io_state_t *sim) {
    return _tick(sim);
}