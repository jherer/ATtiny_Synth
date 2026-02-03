#pragma once
#include <stdint.h>

typedef struct {
    // GPIO
    uint8_t PORTB_SIM;
    uint8_t PINB_SIM;
    uint8_t DDRB_SIM;

    // TIMER0
    uint8_t TCCR0A_SIM;
    uint8_t TCCR0B_SIM;
    uint8_t TCNT0_SIM;
    uint8_t OCR0A_SIM;
    uint8_t OCR0B_SIM;

    // TIMER1
    uint8_t TCCR1_SIM;
    uint8_t GTCCR_SIM;
    uint8_t PLLCSR_SIM;
    uint8_t TCNT1_SIM;
    uint8_t OCR1A_SIM;
    uint8_t OCR1B_SIM;
    uint8_t OCR1C_SIM;

    // STATUS / INTERRUPT
    uint8_t SREG_SIM;
    uint8_t TIMSK_SIM;

} sim_io_state_t;

extern sim_io_state_t sim_io_state;


#define WGM00 0
#define WGM01 1
#define COM0B0 4
#define COM0B1 5
#define COM0A0 6
#define COM0A1 7
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM02 3

#define CS10 0
#define CS11 1
#define CS12 2
#define CS13 3
#define COM1A0 4
#define COM1A1 5
#define PWM1A 6
#define CTC1 7
#define COM1B0 4
#define COM1B1 5
#define PWM1B 6

#define TOIE0 1
#define TOIE1 2
#define OCIE0B 3
#define OCIE0A 4
#define OCIE1B 5
#define OCIE1A 6

void sei(void);
void cli(void);