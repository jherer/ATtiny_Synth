#pragma once
#include <stdint.h>
#define DEBUG_PRINT_DELAY_MS 25
#define DEBUG_YIELD_LOOP_DELAY_MS 25

typedef enum {
    DEBUG_LAYER_HAL,
    DEBUG_LAYER_DRIVERS,
    DEBUG_LAYER_SERVICES,
    DEBUG_LAYER_APP,
    DEBUG_LAYER_MAIN,
    DEBUG_LAYER_SIM,

    NUM_DEBUG_LAYERS,
} debug_layer_t;


#define ENABLE_DEBUG_LAYER_HAL
#define ENABLE_DEBUG_LAYER_DRIVERS
#define ENABLE_DEBUG_LAYER_SERVICES
#define ENABLE_DEBUG_LAYER_APP
#define ENABLE_DEBUG_LAYER_MAIN
#define ENABLE_DEBUG_LAYER_SIM



void debug_init(void);
void debug_print(char *message, debug_layer_t layer);
void debug_println(char *message, debug_layer_t layer);
void debug_println_bin(char *message, uint8_t bin, debug_layer_t layer);
void debug_println_dec(char *message, uint8_t dec, debug_layer_t layer);
void debug_println_hex(char *message, uint8_t hex, debug_layer_t layer);
void debug_println_title(char *message, uint8_t width, debug_layer_t layer);
void debug_pause(char* message, debug_layer_t layer);
char debug_query_char(char* message, debug_layer_t layer);
void debug_delay_ms(uint32_t ms);
void debug_yield_loop(void);