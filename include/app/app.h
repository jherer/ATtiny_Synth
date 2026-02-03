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

#pragma once
#include <core/error_code.h>
#include <drivers/interrupt_driver.h>

// Wrap any functions in app_init() or app_run() that return error_code_t
// To return the error to the main
#define ASSERT_OK(error_code) \
    do { \
        error_code_t err = error_code; \
        if ((error_code_t)err != ERROR_OK) { \
            return err; \
        } \
    } while (0) \

    #define ASSERT_TRUE(statement, error_code) \
    do { \
        error_code_t err = error_code; \
        if (!((bool)statement)) { \
            return err; \
        } \
    } while (0) \

error_code_t app_init(void);
error_code_t app_run(void);