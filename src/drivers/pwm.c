#include "drivers/pwm.h"
#include <avr/io.h>
#include "hal_timer0.h"
#include "hal_timer1.h"
#include "_timer_internal.h"

// TIMER0 PWM

static error_t _pwm_timer0_init(pwm_mode_t mode) {
    uint8_t wgm_bits = 0;
    switch(mode) {
    case PWM_MODE_FAST_FIXED_TOP_T0:
        wgm_bits = 0b011;
        break;
    case PWM_MODE_FAST_VARIABLE_TOP:
        wgm_bits = 0b111;
        break;
    default:
        return ERROR_PWM_MODE_UNSUPPORTED;
    }
    hal_timer0_set_wave_gen_mode(wgm_bits);
    hal_timer0_enable_interrupt_compA(false);
    hal_timer0_enable_interrupt_compB(false);
    hal_timer0_enable_interrupt_ovf(false);
    return ERROR_OK;
}

static error_t _pwm_timer0_attach(pwm_channel_t channel) {
    switch (channel) {
    case PWM_CHANNEL_A:
        hal_timer0_set_compare_output_mode_A(0b10);
        break;
    case PWM_CHANNEL_B:
        hal_timer0_set_compare_output_mode_B(0b10);
        break;
    default:
        return ERROR_PWM_CHANNEL_UNSUPPORTED;
    }
    return ERROR_OK;
}

static error_t _pwm_timer0_detach(pwm_channel_t channel) {
    switch (channel) {
    case PWM_CHANNEL_A:
        hal_timer0_set_compare_output_mode_A(0b00);
        break;
    case PWM_CHANNEL_B:
        hal_timer0_set_compare_output_mode_B(0b00);
        break;
    default:
        return ERROR_PWM_CHANNEL_UNSUPPORTED;
    }
    return ERROR_OK;
}

static error_t _pwm_timer0_set_duty(pwm_channel_t channel, uint8_t value) {
    switch (channel) {
    case PWM_CHANNEL_A:
        OCR0A = value;
        break;
    case PWM_CHANNEL_B:
        OCR0B = value;
    default:
        return ERROR_PWM_CHANNEL_UNSUPPORTED;
    }
    return ERROR_OK;
}

void _pwm_timer0_set_top(uint8_t top) {
    OCR0A = top;
}


// TIMER1 PWM

static error_t _pwm_timer1_init(pwm_mode_t mode) {
    switch (mode) {
    case PWM_MODE_FAST_FIXED_TOP_T0:
        hal_timer1_set_ctc(false);
        return ERROR_PWM_MODE_INVALID_FOR_T1;
    case PWM_MODE_FAST_VARIABLE_TOP:
        hal_timer1_set_ctc(false);
        break;
    default:
        return ERROR_PWM_MODE_UNSUPPORTED;
    }
    hal_timer1_enable_interrupt_compA(false);
    hal_timer1_enable_interrupt_compB(false);
    hal_timer1_enable_interrupt_ovf(false);
    return ERROR_OK;
}

static error_t _pwm_timer1_attach(pwm_channel_t channel) {
    switch (channel) {
    case PWM_CHANNEL_A:
        hal_timer1_set_pwm_1A(true);
        hal_timer1_set_compare_output_mode_A(0b10);
        break;
    case PWM_CHANNEL_B:
        hal_timer1_set_pwm_1B(true);
        hal_timer1_set_compare_output_mode_B(0b10);
        break;
    default:
        return ERROR_PWM_CHANNEL_UNSUPPORTED;
    }
    return ERROR_OK;
}

static error_t _pwm_timer1_detach(pwm_channel_t channel) {
    switch (channel) {
    case PWM_CHANNEL_A:
        hal_timer1_set_pwm_1A(false);
        hal_timer1_set_compare_output_mode_A(0b00);
        break;
    case PWM_CHANNEL_B:
        hal_timer1_set_pwm_1B(false);
        hal_timer1_set_compare_output_mode_B(0b00);
        break;
    default:
        return ERROR_PWM_CHANNEL_UNSUPPORTED;
    }
    return ERROR_OK;
}

static error_t _pwm_timer1_set_duty(pwm_channel_t channel, uint8_t value) {
    switch (channel) {
    case PWM_CHANNEL_A:
        OCR1A = value;
        break;
    case PWM_CHANNEL_B:
        OCR1B = value;
    default:
        return ERROR_PWM_CHANNEL_UNSUPPORTED;
    }
    return ERROR_OK;
}

static void _pwm_timer1_set_top(uint8_t top) {
    OCR1C = top;
}


// SHARED PWM RESOURCES

typedef struct {
    bool initialized;
    pwm_mode_t mode;
    bool compare_A_enabled;
    bool compare_B_enabled;
    uint8_t top;
} pwm_state_t;

pwm_state_t pwm_states[2] = {0};


error_t _set_compare_enabled(timer_id_t id, pwm_channel_t channel, bool enabled) {
    switch (channel) {
    case PWM_CHANNEL_A:
        pwm_states[id].compare_A_enabled = enabled;
        return ERROR_OK;
    case PWM_CHANNEL_B:
        pwm_states[id].compare_B_enabled = enabled;
        return ERROR_OK;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
}

// SHARED PWM PUBLIC API

error_t pwm_init(timer_id_t id, pwm_mode_t mode) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    error_t err = ERROR_OK;
    switch (id) {
    case TIMER_ID_0:
        err = _pwm_timer0_init(mode);
        break;
    case TIMER_ID_1:
        err = _pwm_timer1_init(mode);
        break;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (err == ERROR_OK) {
        pwm_states[id].initialized = true;
        pwm_states[id].mode = mode;
    }
    return err;
}

error_t pwm_start(timer_id_t id, timer_clock_t clock) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (!pwm_states[id].initialized) {
        return ERROR_PWM_UNINITIALIZED;
    }
    switch(id) {
    case TIMER_ID_0:
        return _timer_internal_t0_set_clock(clock);
    case TIMER_ID_1:
        return _timer_internal_t1_set_clock(clock);
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
}

error_t pwm_attach(timer_id_t id, pwm_channel_t channel) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (!pwm_states[id].initialized) {
        return ERROR_PWM_UNINITIALIZED;
    }
    error_t err = ERROR_OK;
    switch (id) {
    case TIMER_ID_0:
        err = _pwm_timer0_attach(channel);
        break;
    case TIMER_ID_1:
        if (pwm_states[TIMER_ID_0].compare_B_enabled &&
            pwm_states[TIMER_ID_1].compare_A_enabled) {
            return ERROR_PWM_CHANNEL_CONFLICT;
        }
        err = _pwm_timer1_attach(channel);
        break;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (err == ERROR_OK) {
        err = _set_compare_enabled(id, channel, true);
    }
    return err;
}

error_t pwm_detach(timer_id_t id, pwm_channel_t channel) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (!pwm_states[id].initialized) {
        return ERROR_PWM_UNINITIALIZED;
    }
    error_t err = ERROR_OK;
    switch (id) {
    case TIMER_ID_0:
        err = _pwm_timer0_detach(channel);
        break;
    case TIMER_ID_1:
        err = _pwm_timer1_detach(channel);
        break;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (err == ERROR_OK) {
        err = _set_compare_enabled(id, channel, false);
    }
    return err;
}

error_t pwm_set_duty(timer_id_t id, pwm_channel_t channel, uint8_t value) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (pwm_states[id].mode == PWM_MODE_FAST_VARIABLE_TOP &&
        value > pwm_states[id].top) {
        return ERROR_PWM_COMPARE_ABOVE_TOP;
    }
    
    switch (id) {
    case TIMER_ID_0:
        if (pwm_states[id].mode == PWM_MODE_FAST_VARIABLE_TOP &&
            channel == PWM_CHANNEL_A) {
            return ERROR_PWM_CHANNEL_A_IS_TOP;
        }
        return _pwm_timer0_set_duty(channel, value);
    case TIMER_ID_1:
        return _pwm_timer1_set_duty(channel, value);
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
}


error_t pwm_set_top(timer_id_t id, uint8_t top) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (!pwm_states[id].initialized) {
        return ERROR_PWM_UNINITIALIZED;
    }

    pwm_mode_t mode = pwm_states[id].mode;
    switch(mode) {
    case PWM_MODE_FAST_FIXED_TOP_T0:
        return ERROR_PWM_MODE_TOP_INVALID;
    case PWM_MODE_FAST_VARIABLE_TOP:
        break;
    default:
        return ERROR_PWM_MODE_UNSUPPORTED;
    }

    switch (id) {
    case TIMER_ID_0:
        _pwm_timer0_set_top(top);
        return ERROR_OK;
    case TIMER_ID_1:
        _pwm_timer1_set_top(top);
        return ERROR_OK;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
}


