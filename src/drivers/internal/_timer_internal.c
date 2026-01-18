#include "_timer_internal.h"
#include "hal_timer0.h"
#include "hal_timer1.h"

error_t _timer_internal_t0_set_clock(timer_clock_t clock) {
    uint8_t cs_bits = 0;
    switch (clock) {
    case TIMER_CLOCK_OFF:
        cs_bits = 0b000;
        break;
    case TIMER_CLOCK_1:
        cs_bits = 0b001;
        break;
    case TIMER_CLOCK_8:
        cs_bits = 0b010;
        break;
    case TIMER_CLOCK_64:
        cs_bits = 0b011;
        break;
    case TIMER_CLOCK_256:
        cs_bits = 0b100;
        break;
    case TIMER_CLOCK_1024:
        cs_bits = 0b101;
        break;
    case TIMER_CLOCK_2_T1:
    case TIMER_CLOCK_4_T1:
    case TIMER_CLOCK_16_T1:
    case TIMER_CLOCK_32_T1:
    case TIMER_CLOCK_128_T1:
    case TIMER_CLOCK_512_T1:
    case TIMER_CLOCK_2048_T1:
    case TIMER_CLOCK_4096_T1:
    case TIMER_CLOCK_8192_T1:
    case TIMER_CLOCK_16384_T1:
    case TIMER_CLOCK_PLL_64MHZ_T1:
    case TIMER_CLOCK_PLL_32MHZ_T1:
    case TIMER_CLOCK_PLL_16MHZ_T1:
        return ERROR_TIMER_CLOCK_INVALID;
    }
    hal_timer0_set_clock_select(cs_bits);
    return ERROR_OK;
}


error_t _timer_internal_t1_set_clock(timer_clock_t clock) {
    uint8_t cs_bits = 0;
    bool enable_pll = false;
    switch (clock) {
    case TIMER_CLOCK_OFF:
        cs_bits = 0b0000;
        break;
    case TIMER_CLOCK_1:
        cs_bits = 0b0001;
        break;
    case TIMER_CLOCK_2_T1:
        cs_bits = 0b0010;
        break;
    case TIMER_CLOCK_4_T1:
        cs_bits = 0b0011;
        break;
    case TIMER_CLOCK_8:
        cs_bits = 0b0100;
        break;
    case TIMER_CLOCK_16_T1:
        cs_bits = 0b0101;
        break;
    case TIMER_CLOCK_32_T1:
        cs_bits = 0b0110;
        break;
    case TIMER_CLOCK_64:
        cs_bits = 0b0111;
        break;
    case TIMER_CLOCK_128_T1:
        cs_bits = 0b1000;
        break;
    case TIMER_CLOCK_256:
        cs_bits = 0b1001;
        break;
    case TIMER_CLOCK_512_T1:
        cs_bits = 0b1010;
        break;
    case TIMER_CLOCK_1024:
        cs_bits = 0b1011;
        break;
    case TIMER_CLOCK_2048_T1:
        cs_bits = 0b1100;
        break;
    case TIMER_CLOCK_4096_T1:
        cs_bits = 0b1101;
        break;
    case TIMER_CLOCK_8192_T1:
        cs_bits = 0b1110;
        break;
    case TIMER_CLOCK_16384_T1:
        cs_bits = 0b1111;
        break;
    case TIMER_CLOCK_PLL_64MHZ_T1:
        enable_pll = true;
        cs_bits = 0b0001;
        break;
    case TIMER_CLOCK_PLL_32MHZ_T1:
        enable_pll = true;
        cs_bits = 0b0010;
        break;
    case TIMER_CLOCK_PLL_16MHZ_T1:
        enable_pll = true;
        cs_bits = 0b0011;
        break;
    default:
        return ERROR_TIMER_CLOCK_INVALID;
    }
    hal_timer1_enable_PLL(enable_pll);
    hal_timer1_set_clock_select(cs_bits);
    return ERROR_OK;
}