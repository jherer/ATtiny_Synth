#ifndef BUTTON_H
#define BUTTON_H
#include "drivers/gpio.h"

typedef struct {
    gpio_t *gpio;
    bool initialized;
    bool stable;
    bool last_state;
    uint8_t counter;
} button_t;


#endif