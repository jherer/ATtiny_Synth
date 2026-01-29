#include "drivers/timer1_driver.h"
#include "hal_timer1.h"
#include "sim/debug.h"

typedef struct {
    bool initialized;
    timer1_mode_t mode;
    timer1_clock_t clock;
    bool pwm_a;
    bool pwm_b;
    timer_callback_t compa_callback;
    timer_callback_t compb_callback;
    timer_callback_t ovf_callback;
} timer1_state_t;


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

static void _set_top(timer1_state_t *s, uint8_t top) {
    hal_timer1_set_output_compare_register_c(top);
}

static void _set_clock(timer1_state_t *s, timer1_clock_t clock) {
    s->clock = clock;
    _apply_control_regs(s);
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

static void _set_callback(timer1_state_t *s, timer_event_t event, timer_callback_t callback) {
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
        break;
    }
}

static void _enable_callback(timer1_state_t *s, timer_event_t event, bool enable) {
    switch (event) {
    case TIMER_EVENT_COMPA:
        hal_timer1_enable_interrupt_compa(enable);
        break;
    case TIMER_EVENT_COMPB:
        hal_timer1_enable_interrupt_compb(enable);
        break;
    case TIMER_EVENT_OVERFLOW:
        hal_timer1_enable_interrupt_ovf(enable);
        break;
    case NUM_TIMER_EVENTS:
    default:
        break;
    }
}

static bool _is_uninitialized(timer1_state_t *s) {
    return !s->initialized;
}

static bool _is_set_top_invalid(timer1_state_t *s) {
    bool valid = (s->mode == TIMER1_MODE_CTC) ||
                    (s->mode == TIMER1_MODE_PWM_VARIABLE_TOP);
    return !valid;
}

static bool _is_pwm_invalid(timer1_state_t *s) {
    bool valid = (s->mode == TIMER1_MODE_PWM_VARIABLE_TOP);
    return !valid;
}

static bool _is_event_invalid(timer1_state_t *s, timer_event_t event) {
    bool variable_top = s->mode == TIMER1_MODE_CTC ||
                    s->mode == TIMER1_MODE_PWM_VARIABLE_TOP;
    return variable_top && (event == TIMER_EVENT_OVERFLOW);
}


static timer1_state_t state = {0};


// PUBLIC API

error_code_t timer1_init(timer1_mode_t mode) {
    if (!timer1_is_mode_valid(mode)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state) && ((state.pwm_a) || (state.pwm_b))) {
        _cleanup_pwm(&state);
    }
    _set_mode(&state, mode);
    state.initialized = true;
    return ERROR_OK;
}

error_code_t timer1_set_top(uint8_t top) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (_is_set_top_invalid(&state)) {
        return ERROR_TIMER1_TOP_BAD_MODE;
    }
    _set_top(&state, top);
    return ERROR_OK;
}

error_code_t timer1_pwm_attach(timer1_pwm_channel_t pwm_channel) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if ((unsigned)pwm_channel >= NUM_TIMER1_PWM_CHANNELS) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state)) {
        return ERROR_TIMER1_PWM_BAD_MODE;
    }
     _set_pwm_channel(&state, pwm_channel, true);
    return ERROR_OK;
}

error_code_t timer1_pwm_detach(timer1_pwm_channel_t pwm_channel) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if ((unsigned)pwm_channel >= NUM_TIMER1_PWM_CHANNELS) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    _set_pwm_channel(&state, pwm_channel, false);
    return ERROR_OK;
}

error_code_t timer1_pwm_set_duty(timer1_pwm_channel_t pwm_channel, uint8_t value) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if ((unsigned)pwm_channel >= NUM_TIMER1_PWM_CHANNELS) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state)) {
        return ERROR_TIMER1_PWM_BAD_MODE;
    }
    _set_pwm_duty(&state, pwm_channel, value);
    return ERROR_OK;
}

error_code_t timer1_set_callback(timer_event_t event, timer_callback_t callback) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if ((unsigned)event >= NUM_TIMER_EVENTS) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (_is_event_invalid(&state, event)) {
        return ERROR_TIMER1_EVENT_BAD_MODE;
    }
    _set_callback(&state, event, callback);
    return ERROR_OK;
}

error_code_t timer1_enable_callback(timer_event_t event, bool enable) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if ((unsigned)event >= NUM_TIMER_EVENTS) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (_is_event_invalid(&state, event)) {
        return ERROR_TIMER1_EVENT_BAD_MODE;
    }
    _enable_callback(&state, event, enable);
    return ERROR_OK;
}


error_code_t timer1_start_clock(timer1_clock_t clock) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_clock_valid(clock)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    _set_clock(&state, clock);
    return ERROR_OK;
}

error_code_t timer1_set_mode(timer1_mode_t mode) {
    if (_is_uninitialized(&state)) {
        return ERROR_TIMER1_UNINIT;
    }
    if (!timer1_is_clock_valid(mode)) {
        return ERROR_TIMER1_ENUM_UNSUPPORTED;
    }
    if (_is_pwm_invalid(&state)) {
        _cleanup_pwm(&state);
    }
    _set_mode(&state, mode);
    return ERROR_OK;
}

error_code_t timer1_cleanup(void) {
    debug_println("t1 cleanup", DEBUG_LAYER_DRIVERS);
    _set_clock(&state, TIMER1_CLOCK_OFF);
    _cleanup_pwm(&state);
    _enable_callback(&state, TIMER_EVENT_COMPA, false);
    _enable_callback(&state, TIMER_EVENT_COMPB, false);
    _enable_callback(&state, TIMER_EVENT_OVERFLOW, false);
    _set_mode(&state, TIMER1_MODE_NORMAL);
    state.initialized = false;
    return ERROR_OK;
}