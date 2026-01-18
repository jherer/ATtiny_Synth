/**
 * @file app.h
 * @brief Header for the app layer
 * 
 * This module provides an interface for main.c to access the user-defined application
 * The makefile determines which application to compile.
 * 
 * @author Joshua Herer
 * @date January 17, 2026
 * @version 1.0
 */

#ifndef APP_H
#define APP_H
#include "drivers/error.h"
#include "drivers/interrupts.h"
error_t app_init(void);
error_t app_run(void);
#endif