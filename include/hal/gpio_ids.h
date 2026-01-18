/**
 * @file gpio_ids.h
 * @brief Header for GPIO IDs
 * 
 * This module provides constants that each encode a particular GPIO pin.
 * Only the pins valid for the specified microcontroller are defined.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

 #ifndef GPIO_IDS_H
#define GPIO_IDS_H

#include <stdint.h>


#define USING_ATTINY85
//#define USING_ATMEGA328

/*
    Each GPIO pin is encoded as a single 8-bit string to save space.
    - High nibble is port letter (A=0, B=1, etc)
    - Low nibble is pin number (0, 1, 2, etc)
    ATtiny85 only has PORTB, but additional ports are provided as a starting point for other microcontrollers
*/
typedef uint8_t gpio_id_t;


#ifdef USING_ATTINY85
#define GPIO_ID_B0 0x10
#define GPIO_ID_B1 0x11
#define GPIO_ID_B2 0x12
#define GPIO_ID_B3 0x13
#define GPIO_ID_B4 0x14
#endif

#ifdef USING_ATMEGA328
// PORTB
#define GPIO_ID_B0 0x10
#define GPIO_ID_B1 0x11
#define GPIO_ID_B2 0x12
#define GPIO_ID_B3 0x13
#define GPIO_ID_B4 0x14
#define GPIO_ID_B5 0x15
#define GPIO_ID_B6 0x16
#define GPIO_ID_B7 0x17
// PORTC
#define GPIO_ID_C0 0x20
#define GPIO_ID_C1 0x21
#define GPIO_ID_C2 0x22
#define GPIO_ID_C3 0x23
#define GPIO_ID_C4 0x24
#define GPIO_ID_C5 0x25
#define GPIO_ID_C6 0x26
// PORTD
#define GPIO_ID_D0 0x30
#define GPIO_ID_D1 0x31
#define GPIO_ID_D2 0x32
#define GPIO_ID_D3 0x33
#define GPIO_ID_D4 0x34
#define GPIO_ID_D5 0x35
#define GPIO_ID_D6 0x36
#define GPIO_ID_D7 0x37
#endif

#endif