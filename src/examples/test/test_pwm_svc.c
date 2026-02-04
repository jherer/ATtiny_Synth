#include <app/app.h>
#include <drivers/gpio_driver.h>
#include <services/pwm_service.h>
#include <platform/debug.h>

typedef struct {
    gpio_t led0;
    gpio_t btn0;
    uint8_t stage;
    bool last_btn;
} app_state_t;

static app_state_t state = {0};

void callback0(void) {
    gpio_toggle(&state.led0);
}

#define APP_PWM_PIN PWM_PIN_PB1
#define APP_NUM_STAGES 7

error_code_t app_init(void) {
    DEBUG_PRINTLN("pwm service test", DEBUG_LAYER_APP);
    ASSERT_OK(pwm_service_init());
    ASSERT_OK(pwm_claim(APP_PWM_PIN, 1000, 127));
    interrupt_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    uint32_t freq = 0;
    uint32_t duty = 127;

    bool curr_btn = gpio_read(&state.btn0);
    if (state.last_btn && !state.last_btn) {

        switch (state.stage) {
        case 0:
            freq = 1000;
            duty = 127;
            break;
        case 1:
            freq = 5000;
            duty = 63;
            break;
        case 2:
            freq = 10000;
            duty = 191;
            break;
        case 3:
            freq = 30000;
            duty = 95;
            break;
        case 4:
            freq = 80000;
            duty = 223;
            break;
        case 5:
            freq = 500;
            duty = 127;
            break;
        case 6:
            freq = 100000;
            duty = 255;
            break;
        case 7:
            freq = 1000;
            duty = 1;
            break;
        }


        if (state.stage >= APP_NUM_STAGES) {
            state.stage = 0;
        }
    }
    state.last_btn = curr_btn;

    ASSERT_OK()

    return ERROR_OK;
}