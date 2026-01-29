/**
 * @file interrupts.h
 * @brief Header for global interrupt control
 * 
 * This module provides a way to turn on and off all interrupts.
 * Doing this makes it so the app doesn't need to import the AVR interrupt header
 * to call sei() and cli(), clearing up the namespace and preventing the app
 * from modifying underlying configurations.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */
#pragma once

void interrupts_enable(void);
void interrupts_disable(void);