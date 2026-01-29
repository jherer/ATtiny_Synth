#include "sim_timer0.h"
#include "hal/timer0_enums.h"
#include <stdlib.h>
#include <stdint.h>
#include "sim/debug.h"
#include "hal/masks.h"

static timer0_mode_t _get_mode(uint8_t tccr0a, uint8_t tccr0b) {
    uint8_t wgm0_bits = ((tccr0a) & (MASK_WGM0_TCCR0A)) >> WGM00;
    wgm0_bits |= ((tccr0b) & (MASK_WGM0_TCCR0B)) >> (WGM02 - 2);
    switch (wgm0_bits) {
    case 0b000: return TIMER0_MODE_NORMAL;
    case 0b010: return TIMER0_MODE_CTC;
    case 0b011: return TIMER0_MODE_PWM_FIXED_TOP;
    case 0b111: return TIMER0_MODE_PWM_VARIABLE_TOP;
    default: return TIMER0_MODE_INVALID;
    }
}

static timer0_clock_t _get_clock(uint8_t tccr0b) {
    uint8_t cs0_bits = ((tccr0b) & (MASK_CS0)) >> CS00;
    switch (cs0_bits) {
    case 0b000: return TIMER0_CLOCK_OFF;
    case 0b001: return TIMER0_CLOCK_1;
    case 0b010: return TIMER0_CLOCK_8;
    case 0b011: return TIMER0_CLOCK_64;
    case 0b100: return TIMER0_CLOCK_256;
    case 0b101: return TIMER0_CLOCK_1024;
    default: return TIMER0_CLOCK_INVALID;
    }
}

static uint32_t _get_clock_divider(timer0_clock_t clock) {
    switch (clock) {
    case TIMER0_CLOCK_OFF: return 0;
    case TIMER0_CLOCK_1: return 1;
    case TIMER0_CLOCK_8: return 8;
    case TIMER0_CLOCK_64: return 64;
    case TIMER0_CLOCK_256: return 256;
    case TIMER0_CLOCK_1024: return 1024;
    case TIMER0_NUM_CLOCKS:
    case TIMER0_CLOCK_INVALID:
    default:
        return 0;
    }
}


typedef struct {
    timer0_mode_t mode;
    timer0_clock_t clock;
    uint16_t clock_divider;
    uint32_t cycles_since_count;

    uint8_t top;
    uint8_t counter;
} sim_timer0_state;


bool _tick(sim_timer0_state *s) {
    timer0_mode_t mode =_get_mode(TCCR0A, TCCR0B);
    timer0_clock_t clock = _get_clock(TCCR0B);

    if (!timer0_is_mode_valid(mode)) {
        debug_println("sim: t0 mode bits undefined", DEBUG_LAYER_SIM);
        return false;
    }
    if (!timer0_is_clock_valid(clock)) {
        debug_println("sim: t0 clock bits undefined", DEBUG_LAYER_SIM);
        return false;
    }
    s->mode = mode;
    s->clock = clock;
    s->clock_divider = _get_clock_divider(s->clock);
    s->cycles_since_count++;

    if (s->cycles_since_count >= s->clock_divider) {
        s->counter++;
        s->cycles_since_count = 0;
    }
    if (s->counter > s->top) {
        s->counter = 0;
        return true;
    }
    return false;
}

static sim_timer0_state state = {0};

bool sim_timer0_tick(void) {
    return _tick_check(&state);
}