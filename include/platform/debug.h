#include <sim/sim_debug.h>

#ifdef HW
    #define DEBUG_DELAY(...) do {} while (0)
    #define DEBUG_PRINT(...) do {} while (0)
    #define DEBUG_PRINTLN(...) do {} while (0)
    #define DEBUG_PRINTLN_DEC(...) do {} while (0)
    #define DEBUG_PRINTLN_BIN(...) do {} while (0)
    #define DEBUG_PRINTLN_HEX(...) do {} while (0)
    #define DEBUG_PRINTLN_TITLE(...) do {} while (0)
#endif


#ifdef SIM
    #define DEBUG_DELAY(...) do {_debug_delay_ms(__VA_ARGS__);} while (0)
    #define DEBUG_PRINT(...) do {_debug_print(__VA_ARGS__);} while (0)
    #define DEBUG_PRINTLN(...) do {_debug_println(__VA_ARGS__);} while (0)
    #define DEBUG_PRINTLN_DEC(...) do {_debug_println_dec(__VA_ARGS__);} while (0)
    #define DEBUG_PRINTLN_BIN(...) do {_debug_println_bin(__VA_ARGS__);} while (0)
    #define DEBUG_PRINTLN_HEX(...) do {_debug_println_hex(__VA_ARGS__);} while (0)
    #define DEBUG_PAUSE(...) do {_debug_pause(__VA_ARGS__);} while (0)

#endif