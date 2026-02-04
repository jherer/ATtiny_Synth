#include <drivers/timer0_driver.h>

static inline bool _timer0_is_top_variable(timer0_mode_t mode) {
    return (mode == TIMER0_MODE_CTC) || 
            (mode == TIMER0_MODE_PWM_VARIABLE_TOP);
}

static inline bool _timer0_is_mode_pwm(timer0_mode_t mode) {
    return (mode == TIMER0_MODE_PWM_FIXED_TOP) ||
                (mode == TIMER0_MODE_PWM_VARIABLE_TOP);
}

static inline bool _timer0_is_pwm_channel_free(timer0_pwm_channel_t pwm_channel, timer0_mode_t mode) {
    bool taken_by_top = (mode == TIMER0_MODE_PWM_VARIABLE_TOP) &&
            (pwm_channel == TIMER0_PWM_CHANNEL_A);
    return !taken_by_top;
}

static inline bool _timer0_is_event_mode_compatible(timer0_mode_t mode, timer0_event_t event) {
    bool invalid = (_timer0_is_top_variable(mode)) &&
                    (event == TIMER0_EVENT_OVERFLOW);
    return !invalid;
}

static inline bool _timer0_is_mode_valid(timer0_mode_t mode) {
    return (unsigned)mode < TIMER0_NUM_MODES;
}

static inline bool _timer0_is_clock_valid(timer0_clock_t clock) {
    return (unsigned)clock < TIMER0_NUM_CLOCKS;
}

static inline bool _timer0_is_pwm_channel_valid(timer0_pwm_channel_t pwm_channel) {
    return (unsigned)pwm_channel < TIMER0_PWM_NUM_CHANNELS;
}

static inline bool _timer0_is_event_valid(timer0_event_t event) {
    return (unsigned)event < TIMER0_NUM_EVENTS;
}
