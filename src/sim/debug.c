#include "sim/debug.h"
#include <stdbool.h>

#ifdef HAL_SIM
    #include <stdio.h>
    #include <unistd.h>
    
    #ifdef _WIN32
        #include <conio.h>
    #endif
#endif


 // Private helpers

static bool _are_keys_in_buffer(void) {
    #ifdef _WIN32
        return _kbhit();
    #endif
    return false;
}

static void _clear_key_buffer(void) {
    #ifdef _WIN32
        _getch();
    #endif
}

static bool _layer_is_enabled(debug_layer_t layer) {
    #ifdef HAL_SIM
    switch (layer) {
    case DEBUG_LAYER_HAL:
        #ifdef ENABLE_DEBUG_LAYER_HAL
            printf("\t\t\t\t\t\t\t\t\t");
            return true;
        #endif
        return false;
        break;
    case DEBUG_LAYER_DRIVERS:
        #ifdef ENABLE_DEBUG_LAYER_DRIVERS
            printf("\t\t\t\t\t\t");
            return true;
        #endif
        return false;
        break;
    case DEBUG_LAYER_SERVICES:
        #ifdef ENABLE_DEBUG_LAYER_SERVICES
            printf("\t\t\t");
            return true;
        #endif
        return false;
        break;
    case DEBUG_LAYER_APP:
        #ifdef ENABLE_DEBUG_LAYER_APP
            return true;
        #endif
        return false;
        break;
    case DEBUG_LAYER_MAIN:
        #ifdef ENABLE_DEBUG_LAYER_MAIN
            return true;
        #endif
        return false;
        break;
    case DEBUG_LAYER_SIM:
        #ifdef ENABLE_DEBUG_LAYER_SIM
            return true;
        #endif
        return false;
        break;
    default:
        return false;
    }
    #endif
    return false;
}


// PUBLIC API

void debug_init(void) {
    #ifdef HAL_SIM
        setbuf(stdout, NULL);
    #endif
}

void debug_delay_ms(uint32_t time_ms) {
    #ifdef HAL_SIM
        usleep(((uint64_t)time_ms) * 1000);
    #endif
}

void debug_print(char* message, debug_layer_t layer) {
    #ifdef HAL_SIM
        if (_layer_is_enabled(layer)) {
            printf(message);
            debug_delay_ms(DEBUG_PRINT_DELAY_MS);
        }
    #endif
}

void debug_println(char* message, debug_layer_t layer) {
    #ifdef HAL_SIM
        if (_layer_is_enabled(layer)) {
            printf(message);
            printf("\n");
            debug_delay_ms(DEBUG_PRINT_DELAY_MS);
        }
    #endif
}

void debug_println_bin(char* message, uint8_t bin, debug_layer_t layer) {
    #ifdef HAL_SIM
        if (_layer_is_enabled(layer)) {
            printf(message);
            printf(": ");
            for (int i = 7; i >= 0; i--) {
                if (bin & (1 << i)) {
                    printf("1");
                } else {
                    printf("0");
                }
                if (i == 4) {
                    printf("-");
                }
            }
            printf("\n");
            debug_delay_ms(DEBUG_PRINT_DELAY_MS);
        }
    #endif
}

void debug_println_dec(char* message, uint8_t dec, debug_layer_t layer) {
    #ifdef HAL_SIM
        if (_layer_is_enabled(layer)) {
            printf(message);
            printf(": %d\n", dec);
            debug_delay_ms(DEBUG_PRINT_DELAY_MS);
        }
    #endif
}

void debug_println_hex(char* message, uint8_t hex, debug_layer_t layer) {
    #ifdef HAL_SIM 
        if (_layer_is_enabled(layer)) {
            printf(message);
            printf(": x%02X\n", hex);
            debug_delay_ms(DEBUG_PRINT_DELAY_MS);
        }
    #endif
}

void debug_println_title(char* message, uint8_t width, debug_layer_t layer) {
    #ifdef HAL_SIM
        if (_layer_is_enabled(layer)) {
            printf("\n\n[");
            //printf(left);
            for (int i = 0; i < width; i++) {
                printf("=");
            }
            printf(" ");
            printf(message);
            printf(" ");
            
            for (int i = 0; i < width; i++) {
                printf("=");
            }
            printf("]\n");
            debug_delay_ms(DEBUG_PRINT_DELAY_MS);
        }
    #endif
}

void debug_pause(char* message, debug_layer_t layer) {
    #ifdef HAL_SIM
        if (_layer_is_enabled(layer)) {
            printf(message);
            // Read and discard all characters until the newline character is found.
            char c;
            while (1) {
                c = getchar();
                if (c == '\n' || c == '\r') {
                    break;
                }
            }
            while (_are_keys_in_buffer()) {
                _clear_key_buffer();
            }
        }
    #endif
}

void debug_yield_loop(void) {
    #ifdef HAL_SIM
        debug_delay_ms(DEBUG_YIELD_LOOP_DELAY_MS);
    #endif
}
