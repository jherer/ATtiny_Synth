#include "ownership.h"
#include <stdlib.h>

static ownership_state_t state = {0};

bool ownership_claim(ownership_pin_t pin, ownership_owner_t owner) {
    ownership_owner_t *pin_owner;
    switch (pin) {
    case OWNERSHIP_PIN_B0:
        pin_owner = &state.b0;
        break;
    case OWNERSHIP_PIN_B1:
        pin_owner = &state.b1;
        break;
    case OWNERSHIP_PIN_B2:
        pin_owner = &state.b2;
        break;
    case OWNERSHIP_PIN_B3:
        pin_owner = &state.b3;
        break;
    case OWNERSHIP_PIN_B4:
        pin_owner = &state.b4;
        break;
    case OWNERSHIP_PIN_B5:
        pin_owner = &state.b5;
        break;
    default:
        return false;
    }

    if (pin_owner == NULL) {
        return false;
    } else if (*pin_owner != OWNERSHIP_FREE) {
        return false;
    }
    *pin_owner = owner;
    return true;
}

bool ownership_free(ownership_pin_t pin, ownership_owner_t owner) {
    ownership_owner_t *pin_owner;
    switch (pin) {
    case OWNERSHIP_PIN_B0:
        pin_owner = &state.b0;
        break;
    case OWNERSHIP_PIN_B1:
        pin_owner = &state.b1;
        break;
    case OWNERSHIP_PIN_B2:
        pin_owner = &state.b2;
        break;
    case OWNERSHIP_PIN_B3:
        pin_owner = &state.b3;
        break;
    case OWNERSHIP_PIN_B4:
        pin_owner = &state.b4;
        break;
    case OWNERSHIP_PIN_B5:
        pin_owner = &state.b5;
        break;
    default:
        return false;
    }

    if (pin_owner == NULL) {
        return false;
    } else if (*pin_owner != owner) {
        return false;
    }
    *pin_owner = OWNERSHIP_FREE;

    return true;
}