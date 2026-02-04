#include <app/app.h>
#include <drivers/timer0_driver.h>
#include <drivers/gpio_driver.h>
#include <platform/debug.h>

typedef struct {
    gpio_t led0;
    gpio_t btn0;
    uint8_t clock_stage;
    uint8_t top_stage;
    bool last_btn;
} app_state_t;

static app_state_t state = {0};

void callback0(void) {
    gpio_toggle(&state.led0);
}

static timer0_clock_t get_clock(uint8_t clock_stage) {
    switch (clock_stage) {
    case 0: return TIMER0_CLOCK_OFF;
    case 1: return TIMER0_CLOCK_1;
    case 2: return TIMER0_CLOCK_8;
    case 3: return TIMER0_CLOCK_64;
    case 4: return TIMER0_CLOCK_256;
    case 5: return TIMER0_CLOCK_1024;
    default: return TIMER0_CLOCK_INVALID;
    }
}

#define NUM_TOP_STAGES 6
static uint8_t get_top(uint8_t top_stage) {
    switch (top_stage) {
    case 0: return 1;
    case 1: return 7;
    case 2: return 63;
    case 3: return 127;
    case 4: return 191;
    case 5: return 255;
    default: return 32;
    }
}

error_code_t app_init(void) {
    DEBUG_PRINTLN("Timer0 test", DEBUG_LAYER_APP);
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.btn0, GPIO_B1, GPIO_MODE_INPUT_PULLUP));
    ASSERT_OK(timer0_init(TIMER0_MODE_NORMAL));
    interrupt_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    bool curr_btn = gpio_read(&state.btn0);
    if (state.last_btn && !state.last_btn) {

        timer0_clock_t clock = get_clock(state.clock_stage);
        uint8_t top = get_top(state.top_stage);
        ASSERT_OK(timer0_start_clock(clock));
        ASSERT_OK(timer0_set_top(top));

        state.top_stage++;
        if (state.top_stage >= NUM_TOP_STAGES) {
            state.top_stage = 0;
            state.clock_stage++;
        }
        if (state.clock_stage >= TIMER0_NUM_CLOCKS) {
            state.clock_stage = 0;
            state.top_stage = 0;
        }
    }
    state.last_btn = curr_btn;

    return ERROR_OK;
}