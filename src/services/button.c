#include "services/button.h"

static button_t button = {0};


error_t button_init(button_t *btn, gpio_id_t id) {
    gpio_t gpio;
    error_t err = gpio_create(&gpio, id, GPIO_MODE_INPUT_PULLUP);
    if (err != ERROR_OK) {
        return err;
    }
    *btn = ((button_t) {
        .gpio = &gpio,
        .initialized = true,
        .stable = true,
        .last_state = false,
        .counter = 0,
    });
    return ERROR_OK;
}

error_t button_update(button_t *btn) {
    if (gpio_read(button.gpio) == btn->last_state) {
        if (btn->counter < 255) {
            btn->counter++;
        }
    }
    return ERROR_OK;
}
