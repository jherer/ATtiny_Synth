#include <app/app.h>
#include <drivers/gpio_driver.h>
#include <services/timer_service.h>
#include <platform/debug.h>

typedef struct {
    uint8_t a;
    gpio_t led0;
    gpio_t led1;
    gpio_t led2;
} app_state_t;

static app_state_t state = {0};

void callback0() {
    gpio_toggle(&state.led0);
}

void callback1() {
    gpio_toggle(&state.led1);
}

void callback2() {
    gpio_toggle(&state.led2);
}

error_code_t app_init(void) {
    DEBUG_PRINTLN("Timer service example", DEBUG_LAYER_APP);
    // Configure drivers and services here
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.led1, GPIO_B1, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.led2, GPIO_B2, GPIO_MODE_OUTPUT));
    ASSERT_OK(timer_service_init());
    ASSERT_OK(timer_service_claim(0, 500));
    ASSERT_OK(timer_service_set_callback(0, callback0));
    ASSERT_OK(timer_service_claim(1, 1000));
    ASSERT_OK(timer_service_set_callback(1, callback1));
    ASSERT_OK(timer_service_claim(2, 2000));
    ASSERT_OK(timer_service_set_callback(2, callback2));
    //ASSERT_OK(gpio_write(&state.led1, 1)); // Causes init error (write input)
    interrupt_enable();
    return ERROR_OK;
}

error_code_t app_run(void) {
    // Run the main loop of the program here
    DEBUG_PRINTLN("Timer service app running", DEBUG_LAYER_APP);
    return ERROR_OK;
}