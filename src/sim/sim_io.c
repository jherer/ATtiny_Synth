#include <sim/sim_io.h>

sim_io_state_t sim_io_state = {
    .OCR0A_SIM = 255,
    .OCR0B_SIM = 255,
    .OCR1A_SIM = 255,
    .OCR1B_SIM = 255,
    .OCR1C_SIM = 255,
};

void sei(void) {
    sim_io_state.SREG_SIM |= (1 << 7);
}

void cli(void) {
    sim_io_state.SREG_SIM &= ~(1 << 7);
}