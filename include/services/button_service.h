#include <stdbool.h>
#include <core/error_code.h>

#define ERROR_BUTTON_UNINIT     (ERROR_MODULE_BUTTON | 0x1)
#define ERROR_BUTTON_ID_CONFLICT (ERROR_MODULE_BUTTON | 0x2)

typedef void (*button_callback_t)(void);

typedef uint8_t button_id_t;
#define BUTTON_ID_MAX 4

typedef enum {
    BTN_EVENT_PRESS,
    BTN_EVENT_RELEASE,
    BTN_NUM_EVENTS,
} button_event_t;

error_code_t button_service_init();
error_code_t button_claim(button_id_t button_id, bool active_low);
error_code_t button_attach_callback(button_id_t button_id, button_event_t event, button_callback_t callback);
error_code_t button_detach_callback(button_id_t button_id, button_event_t event);
error_code_t button_unclaim(button_id_t button_id);
bool button_is_held();