#include "drivers/timer1_driver.h"
#include "hal_timer1.h"
#include <stdlib.h>
#include "core/error_code.h"
#include "sim/debug.h"

typedef struct {
    bool initialized;
    timer1_mode_t mode;
    timer1_clock_t clock;
    bool pwm_a;
    bool pwm_b;
    timer1_callback_t compa_callback;
    timer1_callback_t compb_callback;
    timer1_callback_t ovf_callback;
} timer1_state_t;

// INTERNAL HELPERS

static void _apply_control_regs(timer1_state_t *s) {
    hal_timer1_set_control_registers(s->mode, s->clock, s->pwm_a, s->pwm_b);
}

static void _apply_control_regs_clock_off(timer1_state_t *s) {
    hal_timer1_set_control_registers(s->mode, TIMER1_CLOCK_OFF, s->pwm_a, s->pwm_b);
}

static void _set_mode(timer1_state_t *s, timer1_mode_t mode) {
    _apply_control_regs_clock_off(s);
    s->mode = mode;
    _apply_control_regs(s);
}

static void _set_clock(timer1_state_t *s, timer1_clock_t clock) {
    s->clock = clock;
    _apply_control_regs(s);
}

static void _set_top(timer1_state_t *s, timer1_clock_t top) {
    hal_timer1_set_output_compare_register_c(top);
}

static void _set_pwm_channel(timer1_state_t *s, timer1_pwm_channel_t pwm_channel, bool enable) {
    _apply_control_regs_clock_off(s);
    switch (pwm_channel) {
    case TIMER1_PWM_CHANNEL_A:
        s->pwm_a = enable;
        break;
    case TIMER1_PWM_CHANNEL_B:
        s->pwm_b = enable;
        break;
    case NUM_TIMER1_PWM_CHANNELS:
    default:
        break;
    }
    _apply_control_regs(s);
}

static void _cleanup_pwm(timer1_state_t *s) {
    _apply_control_regs_clock_off(s);
    s->pwm_a = false;
    s->pwm_b = false;
    _apply_control_regs(s);
}

static void _set_pwm_duty(timer1_state_t *s, timer1_pwm_channel_t pwm_channel, uint8_t value) {
    switch (pwm_channel) {
    case TIMER1_PWM_CHANNEL_A:
        hal_timer1_set_output_compare_register_a(value);
        break;
    case TIMER1_PWM_CHANNEL_B:
        hal_timer1_set_output_compare_register_b(value);
        break;
    case NUM_TIMER1_PWM_CHANNELS:
    default:
        break;
    }
    _apply_control_regs(s);
}

static void _set_callback(timer1_state_t *s, timer1_event_t event, timer1_callback_t callback) {
    switch (event) {
    case TIMER1_EVENT_COMPA:
        s->compa_callback = callback;
        break;
    case TIMER1_EVENT_COMPB:
        s->compb_callback = callback;
        break;
    case TIMER1_EVENT_OVERFLOW:
        s->ovf_callback = callback;
        break;
    default:
        break;
    }
}

static void _enable_callback(timer1_state_t *s, timer1_event_t event, bool enable) {
    switch (event) {
    case TIMER1_EVENT_COMPA:
        hal_timer1_enable_interrupt_compa(enable);
        break;
    case TIMER1_EVENT_COMPB:
        hal_timer1_enable_interrupt_compb(enable);
        break;
    case TIMER1_EVENT_OVERFLOW:
        hal_timer1_enable_interrupt_ovf(enable);
        break;
    default:
        break;
    }
}

static bool _is_initialized(timer1_state_t *s) {
    return s->initialized;
}

static inline bool _is_top_variable(timer1_mode_t mode) {
    return (mode == TIMER1_MODE_CTC) || 
            (mode == TIMER1_MODE_PWM_VARIABLE_TOP);
}

static inline bool _is_mode_pwm(timer1_mode_t mode) {
    return (mode == TIMER1_MODE_PWM_VARIABLE_TOP);
}

static inline bool _is_pwm_channel_free(timer1_pwm_channel_t pwm_channel, timer1_mode_t mode) {
    return true;
}

static inline bool _is_event_mode_compatible(timer1_mode_t mode, timer1_event_t event) {
    bool invalid = (_is_top_variable(mode)) &&
                    (event == TIMER1_EVENT_OVERFLOW);
    return !invalid;
}



static timer1_state_t state = {0};


// PUBLIC API

error_code_t timer1_init(timer1_mode_t mode) {
    debug_println("t1 init", DEBUG_LAYER_DRIVERS);
    if (!timer1_is_mode_valid(mode)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    bool currently_pwm = ((state.pwm_a) || (state.pwm_b));
    if (!_is_mode_pwm(state.mode) && currently_pwm) {
        _cleanup_pwm(&state);
    }
    _set_mode(&state, mode);
    state.initialized = true;
    return ERROR_OK;
}

error_code_t timer1_set_top(uint8_t top) {
    debug_println_dec("t1 set top", top, DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!_is_top_variable(state.mode)) {
        return ERROR_TIMER1_TOP_BAD_MODE;
    }
    _set_top(&state, top);
    return ERROR_OK;
}

error_code_t timer1_pwm_attach(timer1_pwm_channel_t pwm_channel) {
    debug_println("t1 pwm attatch", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_pwm_channel_valid(pwm_channel)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (!_is_mode_pwm(state.mode)) {
        return ERROR_TIMER1_PWM_BAD_MODE;
    }
    if (!_is_pwm_channel_free(pwm_channel, state.mode)) {
        return ERROR_TIMER1_PWM_CHANNEL_BAD_MODE;
    }
    _set_pwm_channel(&state, pwm_channel, true);
    return ERROR_OK;
}

error_code_t timer1_pwm_detach(timer1_pwm_channel_t pwm_channel) {
    debug_println("t1 pwm detach", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_pwm_channel_valid(pwm_channel)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (!_is_pwm_channel_free(pwm_channel, state.mode)) {
        return ERROR_TIMER1_PWM_CHANNEL_BAD_MODE;
    }
    _set_pwm_channel(&state, pwm_channel, false);
    return ERROR_OK;
}

error_code_t timer1_pwm_set_duty(timer1_pwm_channel_t pwm_channel, uint8_t value) {
    debug_println("t1 pwm set duty", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_pwm_channel_valid(pwm_channel)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (!_is_mode_pwm(state.mode)) {
        return ERROR_TIMER1_PWM_BAD_MODE;
    }
    if (!_is_pwm_channel_free(pwm_channel, state.mode)) {
        return ERROR_TIMER1_PWM_CHANNEL_BAD_MODE;
    }

    _set_pwm_duty(&state, pwm_channel, value);
    return ERROR_OK;
}

error_code_t timer1_set_callback(timer1_event_t event, timer1_callback_t callback) {
    debug_println("t1 set callback", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_event_valid(event)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (!_is_event_mode_compatible(event, state.mode)) {
        return ERROR_TIMER1_EVENT_BAD_MODE;
    }
    _set_callback(&state, event, callback);
    return ERROR_OK;
}

error_code_t timer1_enable_callback(timer1_event_t event, bool enable) {
    debug_println("t1 enable callback", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_event_valid(event)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (!timer1_is_event_valid(event)) {
        return ERROR_TIMER1_EVENT_BAD_MODE;
    }
    _enable_callback(&state, event, enable);
    return ERROR_OK;
}

error_code_t timer1_start_clock(timer1_clock_t clock) {
    debug_println("t1 start clock", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_clock_valid(clock)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    _set_clock(&state, clock);
    return ERROR_OK;
}

error_code_t timer1_set_mode(timer1_mode_t mode) {
    debug_println("t1 set mode", DEBUG_LAYER_DRIVERS);
    if (!_is_initialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_mode_valid(mode)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (!_is_mode_pwm(mode)) {
        _cleanup_pwm(&state);
    }
    _set_mode(&state, mode);
    return ERROR_OK;
}

error_code_t timer1_cleanup(void) {
    debug_println("t1 cleanup", DEBUG_LAYER_DRIVERS);
    _set_clock(&state, TIMER1_CLOCK_OFF);
    _cleanup_pwm(&state);
    _enable_callback(&state, TIMER1_EVENT_COMPA, false);
    _enable_callback(&state, TIMER1_EVENT_COMPB, false);
    _enable_callback(&state, TIMER1_EVENT_OVERFLOW, false);
    _set_mode(&state, TIMER1_MODE_NORMAL);
    state.initialized = false;
    return ERROR_OK;
}

void timer1_fire_isr_compa(void) {
    if (state.compa_callback != NULL) {
        state.compa_callback();
    }
}

void timer1_fire_isr_compb(void) {
    if (state.compb_callback != NULL) {
        state.compb_callback();
    } 
}

void timer1_fire_isr_ovf(void) {
    if (state.ovf_callback != NULL) {
        state.ovf_callback();
    }
}