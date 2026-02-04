#include <app/app.h>
#include <drivers/gpio_driver.h>
#include <drivers/timer0_driver.h>
#include <platform/debug.h>

typedef struct {
    gpio_t led0;
    gpio_t led1;
    gpio_t led2;
    gpio_t btn0;
    gpio_t btn1;
} app_state_t;

static app_state_t state = {0};

error_code_t app_init(void) {
    DEBUG_PRINTLN("GPIO example", DEBUG_LAYER_APP);
    // Configure drivers and services here
    ASSERT_OK(gpio_create(&state.led0, GPIO_B0, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.led1, GPIO_B1, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.led2, GPIO_B2, GPIO_MODE_OUTPUT));
    ASSERT_OK(gpio_create(&state.btn0, GPIO_B3, GPIO_MODE_INPUT_PULLUP));
    ASSERT_OK(gpio_create(&state.btn1, GPIO_B4, GPIO_MODE_INPUT_PULLUP));
    interrupt_enable();
    return ERROR_OK;
}

static uint8_t test_func(uint8_t input) {
    switch(input) {
        case 0b00: return 0b000;
        case 0b01: return 0b001;
        case 0b10: return 0b010;
        case 0b11: return 0b100;
        default: return 0b111;
    }
}

error_code_t app_run(void) {
    /* Run the main loop of the program here */
    uint8_t input = (!gpio_read(&state.btn1) << 1) | (!gpio_read(&state.btn0));
    uint8_t output = test_func(input);
    gpio_write(&state.led2, (output & 0b100));
    gpio_write(&state.led1, (output & 0b010));
    gpio_write(&state.led0, (output & 0b001));

    return ERROR_OK;
}