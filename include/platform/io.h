/**
 * @file io.h
 * @brief Header for platform-determined IO control
 * 
 * This module determines whether to use actual hardware registers or simulated registers based on the build type.
 * Nothing in the hardware build depends on the sim module, so sim can be excluded from hardware build to save memory
 * (via the makefile).
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */
#pragma once
#include <stdint.h>


#ifdef HW
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
#endif


#ifdef SIM
    #include <sim/sim_io.h>
    static inline void _delay_ms(uint16_t time_ms) {
        /*debug_delay_ms(time_ms);*/
    }

    
    #define PORTB sim_io_state.PORTB_SIM
    #define PINB sim_io_state.PINB_SIM
    #define DDRB sim_io_state.DDRB_SIM
    #define TCCR0A sim_io_state.TCCR0A_SIM
    #define TCCR0B sim_io_state.TCCR0B_SIM
    #define TCNT0 sim_io_state.TCNT0_SIM
    #define OCR0A sim_io_state.OCR0A_SIM
    #define OCR0B sim_io_state.OCR0B_SIM
    
    #define TCCR1 sim_io_state.TCCR1_SIM
    #define GTCCR sim_io_state.GTCCR_SIM
    #define PLLCSR sim_io_state.PLLCSR_SIM
    #define TCNT1 sim_io_state.TCNT1_SIM
    #define OCR1A sim_io_state.OCR1A_SIM
    #define OCR1B sim_io_state.OCR1B_SIM
    #define OCR1C sim_io_state.OCR1C_SIM

    #define SREG sim_io_state.SREG_SIM
    #define TIMSK sim_io_state.TIMSK_SIM



#endif

#if !(defined HW) && !(defined SIM)
#error BUILD MACRO IS NOT DEFINED FOR EITHER HARDWARE (HW) OR SIMULATION (SIM)
#endif


#if defined HW && defined SIM
#error MACRO IS DEFINED FOR BOTH HARDWARE (HW) AND SIMULATION (SIM)
#endif