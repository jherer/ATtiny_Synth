
#include "sim_timer0.h"

void sim_hw_init(void) {

}

void sim_hw_tick(void) {
    bool run_t0_callback = sim_timer0_tick_check_callback();
}