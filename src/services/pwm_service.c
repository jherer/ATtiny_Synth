#include <services/pwm_service.h>
#include <drivers/timer1_driver.h>
#include <platform/debug.h>
#include <math.h>

typedef struct {
    timer1_clock_t clock;
    uint8_t top;
} pwm_config_t;


typedef struct {
    bool initialized;
    bool channel_a_pb1_claimed;
    bool channel_b_pb4_claimed;
} pwm_service_state_t;


static error_code_t _init_pwm(pwm_service_state_t *s) {
    error_code_t err = timer1_init(TIMER1_MODE_PWM_VARIABLE_TOP);
    if (err) return err;
    s->initialized = true;
    return ERROR_OK;
}

#define NUM_TIMER_FREQS 17
static pwm_config_t _calculate_pwm_config(uint16_t target_freq_hz) {
    pwm_config_t config = {0};
    uint32_t timer_freqs[NUM_TIMER_FREQS] = {
        64000000UL,
        32000000UL,
        F_CPU, F_CPU/2, F_CPU/4, F_CPU/8,
        F_CPU/16, F_CPU/32, F_CPU/64, F_CPU/128,
        F_CPU/256, F_CPU/512, F_CPU/1024, F_CPU/2048,
        F_CPU/4096, F_CPU/8192, F_CPU/16384,
    };
    uint16_t clocks[NUM_TIMER_FREQS] = {
        TIMER1_CLOCK_PLL_64,
        TIMER1_CLOCK_PLL_32,
        TIMER1_CLOCK_1, TIMER1_CLOCK_2, TIMER1_CLOCK_4, TIMER1_CLOCK_8,
        TIMER1_CLOCK_16, TIMER1_CLOCK_32, TIMER1_CLOCK_64, TIMER1_CLOCK_128,
        TIMER1_CLOCK_256, TIMER1_CLOCK_512, TIMER1_CLOCK_1024, TIMER1_CLOCK_2048,
        TIMER1_CLOCK_4096, TIMER1_CLOCK_8192, TIMER1_CLOCK_16384,
    };
    // Find best prescaler + TOP combination
    for (uint8_t i = 0; i < NUM_TIMER_FREQS; i++) {
        uint32_t top = (timer_freqs[i] / target_freq_hz) - 1;

        if (top <= 255) {  // Fits in 8-bit timer
            config.clock = clocks[i];
            config.top = top;
            return config;
        }
    }
    config.clock = TIMER1_CLOCK_INVALID;
    return config;
}

static error_code_t _set_frequency(pwm_pin_t pwm_pin, uint32_t target_freq_hz) {
    DEBUG_PRINTLN("pwm svc set freq", DEBUG_LAYER_SERVICES);
    pwm_config_t config = _calculate_pwm_config(target_freq_hz);
    error_code_t err = timer1_set_top(config.top);
    if (err) return err;
    err = timer1_start_clock(config.clock);
    if (err) return err;
    DEBUG_PRINTLN_DEC("    top", config.top, DEBUG_LAYER_SERVICES);
    DEBUG_PRINTLN_DEC("    clock", (uint32_t)config.clock, DEBUG_LAYER_SERVICES);
    return ERROR_OK;
}

static bool _is_pin_claimed(pwm_service_state_t *s, pwm_pin_t pwm_pin) {
    switch (pwm_pin) {
    case PWM_PIN_PB1: return s->channel_a_pb1_claimed;
    case PWM_PIN_PB4: return s->channel_b_pb4_claimed;
    default:
        return false;
    }
}

static void _set_pin_claimed(pwm_service_state_t *s, pwm_pin_t pwm_pin, bool claimed) {
    switch (pwm_pin) {
    case PWM_PIN_PB1:
        s->channel_a_pb1_claimed = claimed;
        break;
    case PWM_PIN_PB4:
        s->channel_b_pb4_claimed = claimed;
        break;
    default:
        break;
    }
}

static timer1_pwm_channel_t _get_channel_from_pin(pwm_pin_t pwm_pin) {
    switch (pwm_pin) {
    case PWM_PIN_PB1: return TIMER1_PWM_CHANNEL_A;
    case PWM_PIN_PB4: return TIMER1_PWM_CHANNEL_B;
    default: return TIMER1_PWM_CHANNEL_INVALID;
    }
}

static inline bool _is_pin_valid(pwm_pin_t pwm_pin) {
    return (unsigned)pwm_pin < PWM_NUM_PINS;
}

static inline bool _get_duty_256_from_percent(uint8_t percent) {
    return (256 * percent) / 100;
}


pwm_service_state_t state = {0};


// Public API
error_code_t pwm_service_init(void) {
    return _init_pwm(&state);
}

error_code_t pwm_claim(pwm_pin_t pwm_pin, uint32_t freq_hz, uint8_t duty_percent) {
    if (!state.initialized) {
        return ERROR_PWM_SERVICE_UNINIT;
    }
    if (!_is_pin_valid(pwm_pin)) {
        return ERROR_PWM_SERVICE_BAD_PIN;
    }
    if (_is_pin_claimed(&state, pwm_pin)) {
        return ERROR_PWM_SERVICE_PIN_IN_USE;
    }
    timer1_pwm_channel_t channel = _get_channel_from_pin(pwm_pin);
    error_code_t err = timer1_pwm_attach(channel);
    if (err) return err;
    err = _set_frequency(pwm_pin, freq_hz);
    if (err) return err;
    uint8_t duty_256 = _get_duty_256_from_percent(duty_percent);
    err = timer1_pwm_set_duty(channel, duty_256);
    if (err) return err;
    _set_pin_claimed(&state, pwm_pin, true);
    return ERROR_OK;
}

error_code_t pwm_set_frequency(pwm_pin_t pwm_pin, uint32_t freq_hz) {
    if (!state.initialized) {
        return ERROR_PWM_SERVICE_UNINIT;
    }
    if (!_is_pin_valid(pwm_pin)) {
        return ERROR_PWM_SERVICE_BAD_PIN;
    }
    if (!_is_pin_claimed(&state, pwm_pin)) {
        return ERROR_PWM_SERVICE_PIN_UNUSED;
    }
    return _set_frequency(pwm_pin, freq_hz);
}

error_code_t pwm_set_duty(pwm_pin_t pwm_pin, uint8_t duty_percent) {
        if (!state.initialized) {
        return ERROR_PWM_SERVICE_UNINIT;
    }
    if (!_is_pin_valid(pwm_pin)) {
        return ERROR_PWM_SERVICE_BAD_PIN;
    }
    if (!_is_pin_claimed(&state, pwm_pin)) {
        return ERROR_PWM_SERVICE_PIN_UNUSED;
    }
    uint8_t duty_256 = _get_duty_256_from_percent(duty_percent);
    return timer1_pwm_set_duty(_get_channel_from_pin(pwm_pin), duty_256);
}

error_code_t pwm_unclaim(pwm_pin_t pwm_pin) {
    if (!state.initialized) {
        return ERROR_PWM_SERVICE_UNINIT;
    }
    if (!_is_pin_valid(pwm_pin)) {
        return ERROR_PWM_SERVICE_BAD_PIN;
    }
    if (!_is_pin_claimed(&state, pwm_pin)) {
        return ERROR_PWM_SERVICE_PIN_UNUSED;
    }
    error_code_t err = timer1_pwm_detach(_get_channel_from_pin(pwm_pin));
    if (err) return err;
    _set_pin_claimed(&state, pwm_pin, false);
    return ERROR_OK;
}