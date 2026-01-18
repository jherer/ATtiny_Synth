#include "drivers/timer.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "hal_timer0.h"
#include "hal_timer1.h"
#include "_timer_internal.h"

// HELPER FUNCTIONS

static bool _top_valid(timer_mode_t mode) {   
    switch(mode) {
    case TIMER_MODE_CTC:    return true;
    case TIMER_MODE_NORMAL: return false;
    default:                return false;
    }
}

static bool _ovf_valid(timer_mode_t mode) {
    switch(mode) {
    case TIMER_MODE_NORMAL: return true;
    case TIMER_MODE_CTC:    return false;
    default:                return false;
    }
}


// TIMER 0

static error_t _timer0_set_mode(timer_mode_t mode) {
    uint8_t wgm_bits = 0;
    switch (mode) {
    case TIMER_MODE_NORMAL:
        wgm_bits = 0;
        break;
    case TIMER_MODE_CTC:
        wgm_bits = 0b010;
        break;
    default:
        return ERROR_TIMER_MODE_UNSUPPORTED;
    }
    hal_timer0_set_wave_gen_mode(wgm_bits);
    hal_timer0_enable_interrupt_compA(false);
    hal_timer0_enable_interrupt_compB(false);
    hal_timer0_enable_interrupt_ovf(false);
    return ERROR_OK;
}

static error_t _timer0_enable_callback(timer_event_t event, timer_mode_t mode, bool enable) {
    switch (event) {
    case TIMER_EVENT_COMPARE_A:
        hal_timer0_enable_interrupt_compA(enable);
        break;
    case TIMER_EVENT_COMPARE_B:
        hal_timer0_enable_interrupt_compB(enable);
        break;
    case TIMER_EVENT_OVERFLOW:
        if (!_ovf_valid(mode)) {   
            return ERROR_TIMER_OVF_NOT_ALLOWED;
        }
        hal_timer0_enable_interrupt_ovf(enable);
        break;
    default:
        return ERROR_TIMER_EVENT_UNSUPPORTED;
    }
    return ERROR_OK;
}


// TIMER 1

static error_t _timer1_set_mode(timer_mode_t mode) {
    switch (mode) {
    case TIMER_MODE_NORMAL:
        hal_timer1_set_ctc(0);
        break;
    case TIMER_MODE_CTC:
        hal_timer1_set_ctc(1);
        break;
    default:
        return ERROR_TIMER_MODE_UNSUPPORTED;
        break;
    }
    hal_timer1_enable_interrupt_compA(false);
    hal_timer1_enable_interrupt_compB(false);
    hal_timer1_enable_interrupt_ovf(false);
    return ERROR_OK;
}

static error_t _timer1_enable_callback(timer_event_t event, timer_mode_t mode, bool enable) {
    switch (event) {
    case TIMER_EVENT_COMPARE_A:
        hal_timer1_enable_interrupt_compA(enable);
        break;
    case TIMER_EVENT_COMPARE_B:
        hal_timer1_enable_interrupt_compB(enable);
        break;
    case TIMER_EVENT_OVERFLOW:
        if (!_ovf_valid(mode)) {   
            return ERROR_TIMER_OVF_NOT_ALLOWED;
        }
        hal_timer1_enable_interrupt_ovf(enable);
        break;
    default:
        return ERROR_TIMER_EVENT_UNSUPPORTED;
    }
    return ERROR_OK;
}



// SHARED TIMER RESOURCES

typedef struct {
    bool initialized;
    timer_mode_t mode;
    timer_clock_t clock;
    timer_callback_t compareA_cb;
    timer_callback_t compareB_cb;
    timer_callback_t overflow_cb;
} timer_state_t;

timer_state_t timer_states[2] = {
    ((timer_state_t) {
        .compareA_cb = NULL,
        .compareB_cb = NULL,
        .overflow_cb = NULL,
    }),
    ((timer_state_t) {
        .compareA_cb = NULL,
        .compareB_cb = NULL,
        .overflow_cb = NULL,
    }),
};


// SHARED TIMER PUBLIC API

error_t timer_init(timer_id_t id, timer_mode_t mode) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    error_t err = ERROR_OK;
    switch (id) {
    case TIMER_ID_0:
        err =_timer0_set_mode(mode);
        break;
    case TIMER_ID_1:
        err = _timer1_set_mode(mode);
        break;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (err == ERROR_OK) {
        timer_states[id].initialized = true;
        timer_states[id].mode = mode;
    }
    return err;
}

error_t timer_set_top(timer_id_t id, uint8_t top) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    } else if (!timer_states[id].initialized) {
        return ERROR_TIMER_UNINITIALIZED;
    }
    timer_mode_t mode = timer_states[id].mode;
    if (!_top_valid(mode)) {
        return ERROR_TIMER_TOP_NOT_ALLOWED;
    }
    switch(id) {
    case TIMER_ID_0:
        hal_timer0_set_output_compare_register_A(top);
        return ERROR_OK;
    case TIMER_ID_1:
        hal_timer1_set_output_compare_register_C(top);
        hal_timer1_set_output_compare_register_A(top);
        return ERROR_OK;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
}

error_t timer_set_count(timer_id_t id, uint8_t count) {
     if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    } else if (!timer_states[id].initialized) {
        return ERROR_TIMER_UNINITIALIZED;
    }
    switch (id) {
    case TIMER_ID_0:
        hal_timer0_set_count(count);
        break;
    case TIMER_ID_1:
        hal_timer1_set_count(count);
        break;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    return ERROR_OK;
}

error_t timer_set_callback(timer_id_t id, timer_event_t event, timer_callback_t cb) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    } else if (!timer_states[id].initialized) {
        return ERROR_TIMER_UNINITIALIZED;
    }
    switch (event) {
    case TIMER_EVENT_COMPARE_A:
        timer_states[id].compareA_cb = cb;
        break;
    case TIMER_EVENT_COMPARE_B:
        timer_states[id].compareB_cb = cb;
        break;
        
    case TIMER_EVENT_OVERFLOW:
        if (!_ovf_valid(timer_states[id].mode)) {
            return ERROR_TIMER_OVF_NOT_ALLOWED;
        }
        timer_states[id].overflow_cb = cb;
        break;
        
    default:
        return ERROR_TIMER_EVENT_UNSUPPORTED;
    }
    return ERROR_OK;
}

error_t timer_enable_callback(timer_id_t id, timer_event_t event, bool enable) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    } else if (!timer_states[id].initialized) {
        return ERROR_TIMER_UNINITIALIZED;
    }
    switch (id) {
        case TIMER_ID_0:
            return _timer0_enable_callback(event, timer_states[id].mode, enable);
        case TIMER_ID_1:
            return _timer1_enable_callback(event, timer_states[id].mode, enable);
        default:
            return ERROR_TIMER_ID_UNSUPPORTED;
    }
}

error_t timer_start(timer_id_t id, timer_clock_t clock) {
    if (id > TIMER_ID_COUNT) {
        return ERROR_TIMER_ID_UNSUPPORTED;
    } else if (!timer_states[id].initialized) {
        return ERROR_TIMER_UNINITIALIZED;
    }
    error_t err = ERROR_OK;
    
    switch (id) {
    case TIMER_ID_0:
        err = _timer_internal_t0_set_clock(clock);
        break;
    case TIMER_ID_1:
        err = _timer_internal_t1_set_clock(clock);
        break;
    default:
        return ERROR_TIMER_ID_UNSUPPORTED;
    }
    if (err == ERROR_OK) {
        timer_states[id].clock = clock;
    }
    return err;
}


error_t timer_stop(timer_id_t id) {
    return timer_start(id, TIMER_CLOCK_OFF);
}


// INTERRUPT SERVICE ROUTINES

ISR(TIMER0_COMPA_vect) {
    timer_callback_t cb = timer_states[TIMER_ID_0].compareA_cb;
    if (cb != NULL) {
        cb();
    }
}

ISR(TIMER0_COMPB_vect) {
    timer_callback_t cb = timer_states[TIMER_ID_0].compareB_cb;
    if (cb != NULL) {
        cb();
    }
}

ISR(TIMER0_OVF_vect) {
    timer_callback_t cb = timer_states[TIMER_ID_0].overflow_cb;
    if (cb != NULL) {
        cb();
    }
}
ISR(TIMER1_COMPA_vect) {
    timer_callback_t cb = timer_states[TIMER_ID_1].compareA_cb;
    if (cb != NULL) {
        cb();
    }
}

ISR(TIMER1_COMPB_vect) {
    timer_callback_t cb = timer_states[TIMER_ID_1].compareB_cb;
    if (cb != NULL) {
        cb();
    }
}

ISR(TIMER1_OVF_vect) {
    timer_callback_t cb = timer_states[TIMER_ID_1].overflow_cb;
    if (cb != NULL) {
        cb();
    }
}