#include <system/fault.h>
#include <hal/hal_gpio.h>
#include <hal/hal_delay.h>

#define DO_ERROR_BLINK
static uint8_t const error_led_id = GPIO_B4;

#define LONG_MS 1000
#define SHORT_MS 450
#define SPACE_MS 150


void _display_error(error_code_t error_code, bool forever) {
    hal_gpio_write_ddr(GPIO_B4, 1);
    hal_gpio_write_port(GPIO_B4, 0);
    
    uint8_t module_code = (error_code & 0xF0) >> 4; // Which module
    uint8_t submodule_code = (error_code & 0x0F); // Which error within module

    do {
        // First # of blinks: module code
        for (int i = 0; i < module_code; i++) {
            hal_gpio_write_port(GPIO_B4, 1);
            hal_delay_smart_ms(SPACE_MS);
            hal_gpio_write_port(GPIO_B4, 0);
            hal_delay_smart_ms(SPACE_MS);
        }
        hal_delay_smart_ms(SHORT_MS);

        // Second # of blinks: submodule code
        for (int i = 0; i < submodule_code; i++) {
            hal_gpio_write_port(GPIO_B4, 1);
            hal_delay_smart_ms(SPACE_MS);
            hal_gpio_write_port(GPIO_B4, 0);
            hal_delay_smart_ms(SPACE_MS);
        }
        hal_delay_smart_ms(LONG_MS);
    } while (forever);
}


void fault_fatal(error_code_t error_code) {
    _display_error(error_code, true);
}


void fault_warning(error_code_t error_code) {
    _display_error(error_code, false);
}