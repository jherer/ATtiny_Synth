#pragma once
#include "hal/platform.h"

#define MASK_WGM0_TCCR0A (1 << WGM01) | (1 << WGM00)
#define MASK_COM0A (1 << COM0A1) | (1 << COM0A0)
#define MASK_COM0B (1 << COM0B1) | (1 << COM0B0)

#define MASK_WGM0_TCCR0B (1 << WGM02)
#define MASK_CS0 ((1 << CS02) | (1 << CS01) | (1 << CS00))


#define MASK_CTC1 (1 << CTC1)
#define MASK_PWM1A (1 << PWM1A)
#define MASK_COM1A (1 << COM1A1) | (1 << COM1A0)
#define MASK_CS1 (1 << CS13) | (1 << CS12) | (1 << CS11) | (1 << CS10)