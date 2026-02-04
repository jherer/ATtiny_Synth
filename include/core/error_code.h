/**
 * @file error_code.h
 * @brief Header for error and system status
 * 
 * This module provides enums for errors that can occur in the driver and service layers
 * Functions that return these can have their values passed up through the app to main,
 * where they will be dealt with.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#pragma once
#include <stdint.h>
typedef uint8_t error_module_t;
typedef uint8_t error_code_t;

#define ERROR_OK (0x00)
#define ERROR_MODULE_GENERAL (0x10)
#define ERROR_MODULE_GPIO (0x20)
#define ERROR_MODULE_TIMER0 (0x30)
#define ERROR_MODULE_TIMER1 (0x40)
#define ERROR_MODULE_ADC (0x50)
#define ERROR_MODULE_SPI (0x60)
#define ERROR_MODULE_TIMER_SERVICE (0x70)
#define ERROR_MODULE_PWM_SERVICE (0x80)

#define ERROR_GENERAL_FUNCTION_UNSUPPORTED (ERROR_MODULE_GENERAL | 0x02)