#include <drivers/timer1_driver.h>

static inline bool _timer1_is_top_variable(timer1_mode_t mode) {
    return (mode == TIMER1_MODE_CTC) || 
            (mode == TIMER1_MODE_PWM_VARIABLE_TOP);
}

static inline bool _timer1_is_mode_pwm(timer1_mode_t mode) {
    return (mode == TIMER1_MODE_PWM_VARIABLE_TOP);
}

static inline bool _timer1_is_event_mode_compatible(timer1_event_t event, timer1_mode_t mode) {
    bool invalid = (_timer1_is_top_variable(mode)) &&
                    (event == TIMER1_EVENT_OVERFLOW);
    return !invalid;
}

static inline bool _timer1_is_mode_valid(timer1_mode_t mode) {
    return (unsigned)mode < TIMER1_NUM_MODES;
}

static inline bool _timer1_is_clock_valid(timer1_clock_t clock) {
    return (unsigned)clock < TIMER1_NUM_CLOCKS;
}

static inline bool _timer1_is_pwm_channel_valid(timer1_pwm_channel_t pwm_channel) {
    return (unsigned)pwm_channel < TIMER1_PWM_NUM_CHANNELS;
}

static inline bool _timer1_is_event_valid(timer1_event_t event) {
    return (unsigned)event < TIMER1_NUM_EVENTS;
}
