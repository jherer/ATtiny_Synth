#pragma once
#include <stdint.h>
#include <core/error_code.h>

#define ERROR_SOFT_TIMER_SVC_UNINIT         (ERROR_MODULE_SOFT_TIMER | 0x2)
#define ERROR_SOFT_TIMER_BAD_ID         (ERROR_MODULE_SOFT_TIMER | 0x3)
#define ERROR_SOFT_TIMER_ID_UNUSED      (ERROR_MODULE_SOFT_TIMER | 0x4)
#define ERROR_SOFT_TIMER_ID_CONFLICT    (ERROR_MODULE_SOFT_TIMER | 0x5)
#define ERROR_SOFT_TIMER_BAD_ENUM       (ERROR_MODULE_SOFT_TIMER | 0x6)

typedef void (*soft_timer_callback_t)(void);

#define SOFT_TIMER_ID_MAX 8
typedef uint8_t soft_timer_id_t;
typedef uint16_t soft_timer_count_t;

error_code_t soft_timer_service_init(void);
error_code_t soft_timer_claim(soft_timer_id_t id, uint16_t period_ms);
error_code_t soft_timer_set_period(soft_timer_id_t id, uint16_t period_ms);
error_code_t soft_timer_set_callback(soft_timer_id_t id, soft_timer_callback_t callback);
error_code_t soft_timer_unclaim(soft_timer_id_t id);