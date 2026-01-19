#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H
#include "drivers/timer.h"
#include "drivers/error.h"

error_t soft_timer_init(void);
error_t soft_timer_add_event(uint8_t soft_timer_num, uint16_t period_ms, timer_callback_t cb);
bool soft_timer_check_overflow(uint8_t soft_timer_num);
uint32_t soft_timer_get_millis(void);
#endif