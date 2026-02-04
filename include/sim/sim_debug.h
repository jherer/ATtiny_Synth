#pragma once
#ifdef SIM
    #include <core/debug_layer.h>
    #include <sim/sim_io.h>
    #include <sim/sim_config.h>
    #include <stdint.h>

    void _debug_init(void);
    void _debug_print(char *message, debug_layer_t layer);
    void _debug_println(char *message, debug_layer_t layer);
    void _debug_println_bin(char *message, uint8_t bin, debug_layer_t layer);
    void _debug_println_dec(char *message, uint32_t dec, debug_layer_t layer);
    void _debug_println_hex(char *message, uint8_t hex, debug_layer_t layer);
    void _debug_println_title(char *message, uint8_t width, debug_layer_t layer);
    void _debug_pause(char* message, debug_layer_t layer);
    char _debug_query_char(char* message, debug_layer_t layer);
    uint32_t _debug_query_int(char* message, debug_layer_t layer);
    void _debug_delay_ms(uint32_t ms);
    void _debug_yield_loop(void);
    void _debug_dump_registers(sim_io_state_t *s);
#endif