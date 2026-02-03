#include <hal/timer0_enums.h>
#include <sim/sim_io.h>
#include <drivers/timer0_driver.h>

typedef struct {
    bool compa;
    bool compb;
    bool ovf;
} sim_timer0_events_t;

sim_timer0_events_t sim_timer0_tick(sim_io_state_t *sim);