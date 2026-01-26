#include "drivers/timer0.h"
#include "hal_timer0.h"
#include <stdlib.h>
#include "drivers/error_code.h"
#include "sim/debug.h"
#include "ownership.h"

typedef struct {
    bool initialized;
    timer0_mode_t mode;
    timer0_clock_t clock;
    bool pwm_a;
    bool pwm_b;
    timer_callback_t compa_callback;
    timer_callback_t compb_callback;
    timer_callback_t ovf_callback;
} timer0_state_t;

// INTERNAL HELPERS

static void _apply_control_regs(timer0_state_t *s) {
    hal_timer0_set_control_registers(s->mode, s->clock, s->pwm_a, s->pwm_b);
}

static void _apply_control_regs_clock_off(timer0_state_t *s) {
    hal_timer0_set_control_registers(s->mode, TIMER0_CLOCK_OFF, s->pwm_a, s->pwm_b);
}

static error_code_t _set_mode(timer0_state_t *s, timer0_mode_t mode) {
    _apply_control_regs_clock_off(s);
    s->mode = mode;
    _apply_control_regs(s);
    return ERROR_OK;
}

static error_code_t _set_clock(timer0_state_t *s, timer0_clock_t clock) {
    s->clock = clock;
    _apply_control_regs(s);
    return ERROR_OK;
}

static error_code_t _enable_pwm_channel(timer0_state_t *s, timer0_pwm_channel_t pwm_channel, bool enabled) {
    _apply_control_regs_clock_off(s);
    switch (pwm_channel) {
    case TIMER0_PWM_CHANNEL_A:
        s->pwm_a = enabled;
        if (!ownership_claim(OWNERSHIP_PIN_B0, OWNERSHIP_TIMER0_PWM)) {
            return ERROR_TIMER0_PWM_PIN_CONFLICT;
        } 
        break;
    case TIMER0_PWM_CHANNEL_B:
        s->pwm_b = enabled;
        if (!ownership_claim(OWNERSHIP_PIN_B1, OWNERSHIP_TIMER0_PWM)) {
            return ERROR_TIMER0_PWM_PIN_CONFLICT;
        }
        break;
    case NUM_TIMER0_PWM_CHANNELS:
    default:
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    _apply_control_regs(s);
    return ERROR_OK;
}

static void _cleanup_pwm(timer0_state_t *s) {
    _apply_control_regs_clock_off(s);
    s->pwm_a = false;
    ownership_free(OWNERSHIP_PIN_B0, OWNERSHIP_TIMER0_PWM);
    s->pwm_b = false;
    ownership_free(OWNERSHIP_PIN_B1, OWNERSHIP_TIMER0_PWM);
    _apply_control_regs(s);
}

static error_code_t _set_pwm_duty(timer0_state_t *s, timer0_pwm_channel_t pwm_channel, uint8_t value) {
    switch (pwm_channel) {
    case TIMER0_PWM_CHANNEL_A:
        hal_timer0_set_output_compare_register_a(value);
        break;
    case TIMER0_PWM_CHANNEL_B:
        hal_timer0_set_output_compare_register_b(value);
        break;
    case NUM_TIMER0_PWM_CHANNELS:
    default:
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    _apply_control_regs(s);
    return ERROR_OK;
}

static error_code_t _set_callback(timer0_state_t *s, timer_event_t event, timer_callback_t callback) {
    switch (event) {
    case TIMER_EVENT_COMPA:
        s->compa_callback = callback;
        break;
    case TIMER_EVENT_COMPB:
        s->compb_callback = callback;
        break;
    case TIMER_EVENT_OVERFLOW:
        s->ovf_callback = callback;
        break;
    case NUM_TIMER_EVENTS:
    default:
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    return ERROR_OK;
}

static error_code_t _enable_callback(timer0_state_t *s, timer_event_t event, bool enable) {
    switch (event) {
    case TIMER_EVENT_COMPA:
        hal_timer0_enable_interrupt_compa(enable);
        break;
    case TIMER_EVENT_COMPB:
        hal_timer0_enable_interrupt_compb(enable);
        break;
    case TIMER_EVENT_OVERFLOW:
        hal_timer0_enable_interrupt_ovf(enable);
        break;
    case NUM_TIMER_EVENTS:
    default:
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    return ERROR_OK;
}

static bool _is_uninitialized(timer0_state_t *s) {
    return !s->initialized;
}

static bool _is_pwm_channel_invalid(timer0_state_t *s, timer0_pwm_channel_t pwm_channel) {
    return s->mode == TIMER0_MODE_PWM_VARIABLE_TOP &&
            pwm_channel == TIMER0_PWM_CHANNEL_A;
}

static bool _is_pwm_invalid(timer0_state_t *s) {
    bool valid = s->mode == TIMER0_MODE_PWM_FIXED_TOP ||
                s->mode == TIMER0_MODE_PWM_VARIABLE_TOP;
    return !valid;
}

static bool _is_set_top_invalid(timer0_state_t *s) {
    bool valid = (s->mode == TIMER0_MODE_CTC) || 
                (s->mode == TIMER0_MODE_PWM_VARIABLE_TOP);
    return !valid;
}

static bool _is_event_invalid(timer0_state_t *s, timer_event_t event) {
    bool variable_top = s->mode == TIMER0_MODE_CTC ||
                    s->mode == TIMER0_MODE_PWM_VARIABLE_TOP;

    return variable_top && event == TIMER_EVENT_OVERFLOW;
}


static timer0_state_t state = {0};


// PUBLIC API

error_code_t timer0_init(timer0_mode_t mode) {
    if ((unsigned)mode >= NUM_TIMER0_MODES) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state) && ((state.pwm_a) || (state.pwm_b))) {
        _cleanup_pwm(&state);
    }

    error_code_t mode_err = _set_mode(&state, mode);
    if (mode_err != ERROR_OK) {
        return mode_err;
    }
    state.initialized = true;
    return ERROR_OK;
}

error_code_t timer0_set_top(uint8_t top) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if (_is_set_top_invalid(&state)) {
        return ERROR_TIMER0_TOP_BAD_MODE;
    }
    hal_timer0_set_output_compare_register_a(top);
    return ERROR_OK;
}

error_code_t timer0_pwm_attach(timer0_pwm_channel_t pwm_channel) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)pwm_channel >= NUM_TIMER0_PWM_CHANNELS) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state)) {
        return ERROR_TIMER0_PWM_BAD_MODE;
    }
    if (_is_pwm_channel_invalid(&state, pwm_channel)) {
        return ERROR_TIMER0_PWM_CHANNEL_BAD_MODE;
    }
    return _enable_pwm_channel(&state, pwm_channel, true);
}


error_code_t timer0_pwm_detach(timer0_pwm_channel_t pwm_channel) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)pwm_channel >= NUM_TIMER0_PWM_CHANNELS) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_channel_invalid(&state, pwm_channel)) {
        return ERROR_TIMER0_PWM_CHANNEL_BAD_MODE;
    }
    return _enable_pwm_channel(&state, pwm_channel, false);
}

error_code_t timer0_pwm_set_duty(timer0_pwm_channel_t pwm_channel, uint8_t value) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)pwm_channel >= NUM_TIMER0_PWM_CHANNELS) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state)) {
        return ERROR_TIMER0_PWM_BAD_MODE;
    }
    if (_is_pwm_channel_invalid(&state, pwm_channel)) {
        return ERROR_TIMER0_PWM_CHANNEL_BAD_MODE;
    }

   return _set_pwm_duty(&state, pwm_channel, value);
}

error_code_t timer0_set_callback(timer_event_t event, timer_callback_t callback) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)event >= NUM_TIMER_EVENTS) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_event_invalid(&state, event)) {
        return ERROR_TIMER_EVENT_BAD_MODE;
    }
    return _set_callback(&state, event, callback);
}

error_code_t timer0_enable_callback(timer_event_t event, bool enable) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)event >= NUM_TIMER_EVENTS) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_event_invalid(&state, event)) {
        return ERROR_TIMER_EVENT_BAD_MODE;
    }
    return _enable_callback(&state, event, enable);
}

error_code_t timer0_start_clock(timer0_clock_t clock) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)clock >= NUM_TIMER0_CLOCKS) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    return _set_clock(&state, clock);
}

error_code_t timer0_set_mode(timer0_mode_t mode) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER0_UNINITIALIZED;
    }
    if ((unsigned)mode >= NUM_TIMER0_MODES) {
        return ERROR_TIMER0_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state)) {
        _cleanup_pwm(&state);
    }
    return _set_mode(&state, mode);
}

error_code_t cleanup() {
    if (&state.clock != TIMER0_CLOCK_OFF) {
        _set_clock(&state, TIMER0_CLOCK_OFF);
    }
    if (&state.pwm_a || &state.pwm_b) {
        _cleanup_pwm(&state);
    }
    if (&state.mode != TIMER0_MODE_NORMAL) {
        _set_mode(&state, TIMER0_MODE_NORMAL);
    }
    return ERROR_OK;
}