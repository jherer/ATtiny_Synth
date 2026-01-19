#include "app/app.h"
#include "drivers/interrupts.h"
#include "drivers/gpio.h"
#include "drivers/pwm.h"

#define NUM_DUTIES 9

typedef struct {
    gpio_t led;
    gpio_t btn;
    bool btn_state;
    bool last_btn_state;
    uint8_t duties[NUM_DUTIES];
    uint8_t index;
} app_state_t;

static app_state_t state = {.duties = {0, 4, 8, 16, 32, 64, 128, 192, 255}};

error_t app_init(void) {
    interrupts_disable();
    VALIDATE_ERROR(gpio_create(&state.led, GPIO_ID_B0, GPIO_MODE_OUTPUT));
    VALIDATE_ERROR(gpio_create(&state.btn, GPIO_ID_B2, GPIO_MODE_INPUT_PULLUP));
    VALIDATE_ERROR(pwm_init(TIMER_ID_0, PWM_MODE_FAST_FIXED_TOP_T0));
    VALIDATE_ERROR(pwm_attach(TIMER_ID_0, PWM_CHANNEL_A));
    VALIDATE_ERROR(pwm_set_duty(TIMER_ID_0, PWM_CHANNEL_A, 0));
    VALIDATE_ERROR(pwm_start(TIMER_ID_0, TIMER_CLOCK_1024));
    //interrupts_enable();
    return ERROR_OK;
}

error_t app_run(void) {
    if (state.btn_state == 0 && state.last_btn_state == 1) {
        state.index++;
        if (state.index >= NUM_DUTIES) {
            state.index = 0;
        }
        pwm_set_duty(TIMER_ID_0, PWM_CHANNEL_A, state.duties[state.index]);
    }
    state.last_btn_state = state.btn_state;
    state.btn_state = gpio_read(&state.btn);
    
    return ERROR_OK;
}