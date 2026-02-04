#pragma once
#include <stdint.h>
#include <core/error_code.h>

#define ERROR_TIMER_SERVICE_UNINIT      (ERROR_MODULE_TIMER_SERVICE | 0x1)
#define ERROR_TIMER_SERVICE_BAD_ID      (ERROR_MODULE_TIMER_SERVICE | 0x2)
#define ERROR_TIMER_SERVICE_ID_UNUSED   (ERROR_MODULE_TIMER_SERVICE | 0x3)
#define ERROR_TIMER_SERVICE_ID_CONFLICT (ERROR_MODULE_TIMER_SERVICE | 0x4)
#define ERROR_TIMER_SERVICE_BAD_ENUM    (ERROR_MODULE_TIMER_SERVICE | 0x5)

typedef void (*timer_service_callback_t)(void);

#define TIMER_SERVICE_ID_MAX 8
typedef uint8_t timer_service_id_t;

error_code_t timer_service_init(void);
error_code_t timer_service_claim(timer_service_id_t id, uint16_t period_ms);
error_code_t timer_service_set_period(timer_service_id_t id, uint16_t period_ms);
error_code_t timer_service_set_callback(timer_service_id_t id, timer_service_callback_t callback);
error_code_t timer_service_unclaim(timer_service_id_t id);