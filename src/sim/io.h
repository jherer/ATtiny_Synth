#pragma once
#include <stdint.h>

// TIMER COMMON
extern uint8_t TIMSK;
#define OCIE1A 6
#define OCIE1B 5
#define OCIE0A 4
#define OCIE0B 3
#define TOIE1 2
#define TOIE0 1

// TIMER0
extern uint8_t TCCR0A;
#define COM0A1 7
#define COM0A0 6
#define COM0B1 5
#define COM0B0 4
#define WGM01 1
#define WGM00 0
extern uint8_t TCCR0B;
#define WGM02 3
#define CS02 2
#define CS01 1
#define CS00 0
extern uint8_t TCNT0;
extern uint8_t OCR0A;
extern uint8_t OCR0B;

// TIMER1
extern uint8_t TCCR1;
#define CTC1 7
#define PWM1A 6
#define COM1A1 5
#define COM1A0 4
#define CS13 3
#define CS12 2
#define CS11 1
#define CS10 0
extern uint8_t GTCCR;
#define PWM1B 6
#define COM1B1 5
#define COM1B0 4
extern uint8_t TCNT1;
extern uint8_t OCR1A;
extern uint8_t OCR1B;
extern uint8_t OCR1C;

// IO
extern uint8_t PORTB;
extern uint8_t PINB;
extern uint8_t DDRB;

// STATUS / INTERRUPTS

extern uint8_t SREG;

static inline void sei(void) {
    SREG |= (1 << 7);
}

static inline void cli(void) {
    SREG &= ~(1 << 7);
}
