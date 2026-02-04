#include <services/timer_service.h>
#include <drivers/timer0_driver.h>
#include <stdlib.h>
#include <stdbool.h>
#include <platform/debug.h>

#define CLOCK_SOURCE TIMER0_CLOCK_64
#define PRESCALER_VALUE 64
#define TOP_VALUE 250

static const uint16_t ovf_per_sec = ((F_CPU / PRESCALER_VALUE) / TOP_VALUE);


typedef uint16_t timer_count_t;

typedef struct {
    bool claimed;
    timer_count_t count;
    timer_count_t max_count;
    timer_service_callback_t callback;
} software_timer_t;

typedef struct {
    bool initialized;
    software_timer_t timers[TIMER_SERVICE_ID_MAX];
} timer_service_state_t;

static inline bool _is_timer_id_valid(timer_service_id_t id) {
    return (unsigned)id <= TIMER_SERVICE_ID_MAX;
}

static inline bool _is_timer_id_free(timer_service_state_t *s, timer_service_id_t id) {
    return (s->timers[id]).claimed == false;
}

void _internal_callback(void);

void _init_timers(timer_service_state_t *s) {
    for (int i = 0; i < TIMER_SERVICE_ID_MAX; i++) {
        s->timers[i] = ((software_timer_t) {
            .claimed = false,
            .count = 0,
            .max_count = 1000,
        });
    }
    s->initialized = true;
}

void _claim(timer_service_state_t *s, timer_service_id_t id) {
    s->timers[id].claimed = true;
}

void _set_max_count(timer_service_state_t *s, timer_service_id_t id, uint16_t period_ms) {
    s->timers[id].max_count = period_ms * (ovf_per_sec / 1000U);
}

void _set_callback(timer_service_state_t *s, timer_service_id_t id, timer_service_callback_t callback) {
    s->timers[id].callback = callback;
}

void _unclaim(timer_service_state_t *s, timer_service_id_t id) {
    s->timers->callback = NULL;
    s->timers->claimed = false;
    s->timers->count = 0;
}

static timer_service_state_t state = {0};


void _internal_callback(void) {
    for (int i = 0; i < TIMER_SERVICE_ID_MAX; i++) {
        if (state.timers[i].claimed) {
            state.timers[i].count++;

            if (state.timers[i].count >= state.timers[i].max_count) {
                state.timers[i].count = 0;
                if (state.timers[i].callback != NULL) {
                    state.timers[i].callback();
                }
            }   
        }
    }
}


error_code_t timer_service_init(void) {
    DEBUG_PRINTLN("Soft timer svc init", DEBUG_LAYER_SERVICES);
    error_code_t err = timer0_init(TIMER0_MODE_CTC);
    if (err) return err;
    err = timer0_set_callback(TIMER0_EVENT_COMPA, _internal_callback);
    if (err) return err;
    err = timer0_enable_callback(TIMER0_EVENT_COMPA, true);
    if (err) return err;
    err = timer0_set_top(TOP_VALUE);
    if (err) return err;
    err = timer0_start_clock(CLOCK_SOURCE);
    if (err) return err;

    _init_timers(&state);
    return ERROR_OK;
}

error_code_t timer_service_claim(timer_service_id_t id, uint16_t period_ms) {
    DEBUG_PRINTLN_DEC("Soft timer claim id", id, DEBUG_LAYER_SERVICES);
    if (!state.initialized) {
        return ERROR_TIMER_SERVICE_UNINIT;
    }
    if (!_is_timer_id_valid(id)) {
        return ERROR_TIMER_SERVICE_BAD_ID;
    }
    if (!_is_timer_id_free(&state, id)) {
        DEBUG_PRINTLN("BADDY BAD BAD", DEBUG_LAYER_SERVICES);
        return ERROR_TIMER_SERVICE_ID_CONFLICT;
    }
    _claim(&state, id);
    _set_max_count(&state, id, period_ms);
    DEBUG_PRINTLN_DEC("    period (ms)", period_ms, DEBUG_LAYER_SERVICES);
    return ERROR_OK;
}

error_code_t timer_service_set_period(timer_service_id_t id, uint16_t period_ms) {
    DEBUG_PRINTLN_DEC("Soft timer period id", id, DEBUG_LAYER_SERVICES);
    if (!state.initialized) {
        return ERROR_TIMER_SERVICE_UNINIT;
    }
    if (!_is_timer_id_valid(id)) {
        return ERROR_TIMER_SERVICE_BAD_ID;
    }
    if (_is_timer_id_free(&state, id)) {
        return ERROR_TIMER_SERVICE_ID_UNUSED;
    }
    _set_max_count(&state, id, period_ms);
    DEBUG_PRINTLN_DEC("    period (ms)", period_ms, DEBUG_LAYER_SERVICES);
    return ERROR_OK;
}

error_code_t timer_service_set_callback(timer_service_id_t id, timer_service_callback_t callback) {
    DEBUG_PRINTLN_DEC("Soft timer callback id", id, DEBUG_LAYER_SERVICES);
    if (!state.initialized) {
        return ERROR_TIMER_SERVICE_UNINIT;
    }
    if (!_is_timer_id_valid(id)) {
        return ERROR_TIMER_SERVICE_BAD_ID;
    }
    if (_is_timer_id_free(&state, id)) {
        return ERROR_TIMER_SERVICE_ID_UNUSED;
    }
    _set_callback(&state, id, callback);
    return ERROR_OK;
}

error_code_t timer_service_unclaim(timer_service_id_t id) {
    DEBUG_PRINTLN_DEC("Soft timer unclaim id", id, DEBUG_LAYER_SERVICES);
    if (!state.initialized) {
        return ERROR_TIMER_SERVICE_UNINIT;
    }
    if (!_is_timer_id_valid(id)) {
        return ERROR_TIMER_SERVICE_BAD_ID;
    }
    if (_is_timer_id_free(&state, id)) {
        return ERROR_TIMER_SERVICE_ID_UNUSED;
    }
    _unclaim(&state, id);
    return ERROR_OK;
}
