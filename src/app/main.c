#define F_CPU 16000000UL
#include <avr/io.h>
#include <gpio.h>
#include <util/delay.h>

int main () {
    gpio_pin_t led = gpio_pin_new(HAL_PIN_B2);
    gpio_pin_mode(&led, GPIO_MODE_OUTPUT);
    //hal_gpio_mode_output(HAL_PIN_B0);
    while(1) {
        //hal_gpio_toggle(HAL_PIN_B0);
        gpio_pin_toggle(&led);
        _delay_ms(200);
    }
    return 0;
}