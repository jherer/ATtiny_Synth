#include "app/app.h"
#include "drivers/interrupts.h"
#include "drivers/gpio.h"
#include "services/soft_timer.h"

typedef struct {
    gpio_t led0;
    gpio_t led1;
    gpio_t led2;
    gpio_t led3;
} app_state_t;

static app_state_t state = {0};

void blink0() {
    gpio_toggle(&state.led0);
}

void blink1() {
    gpio_toggle(&state.led1);
}

void empty() {}

error_t app_init(void) {
    VALIDATE_ERROR(gpio_create(&state.led0, GPIO_ID_B0, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(gpio_create(&state.led1, GPIO_ID_B1, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(gpio_create(&state.led2, GPIO_ID_B2, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(gpio_create(&state.led3, GPIO_ID_B3, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(soft_timer_init());
    VALIDATE_ERROR(soft_timer_add_event(0, 125, blink0));
    VALIDATE_ERROR(soft_timer_add_event(1, 250, blink1));
    VALIDATE_ERROR(soft_timer_add_event(2, 500, empty));
    interrupts_enable();
    return ERROR_OK;
}

uint32_t t = 0;
error_t app_run(void) {
    if (soft_timer_check_overflow(2)) {
        gpio_toggle(&state.led2);
    }
    if (soft_timer_get_millis() - t > 1000) {
        gpio_toggle(&state.led3);
        t = soft_timer_get_millis();
    }
    return ERROR_OK;
}