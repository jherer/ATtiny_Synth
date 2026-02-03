/**
 * @file interrupts.h
 * @brief Header for global interrupt control
 * 
 * This module provides a way to turn on and off all interrupts.
 * Doing this makes it so the app doesn't need to import the AVR interrupt header
 * in order to call sei() and cli(). This clears up the namespace and preventing
 * the app from directly modifying underlying configurations.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */
#pragma once

void interrupt_enable(void);
void interrupt_disable(void);