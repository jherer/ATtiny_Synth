#pragma once
#include <stdbool.h>

typedef enum {
    OWNERSHIP_PIN_B0,
    OWNERSHIP_PIN_B1,
    OWNERSHIP_PIN_B2,
    OWNERSHIP_PIN_B3,
    OWNERSHIP_PIN_B4,
    OWNERSHIP_PIN_B5,
} ownership_pin_t;

typedef enum {
    OWNERSHIP_FREE = 0,
    OWNERSHIP_GPIO,
    OWNERSHIP_TIMER0_PWM,
    OWNERSHIP_TIMER1_PWM,
    OWNERSHIP_PCINT,
    OWNERSHIP_ADC,
    OWNERSHIP_SPI,
} ownership_owner_t;

typedef struct {
    ownership_owner_t b0;
    ownership_owner_t b1;
    ownership_owner_t b2;
    ownership_owner_t b3;
    ownership_owner_t b4;
    ownership_owner_t b5;
} ownership_state_t;

bool ownership_claim(ownership_pin_t pin, ownership_owner_t owner);
bool ownership_free(ownership_pin_t pin, ownership_owner_t owner);