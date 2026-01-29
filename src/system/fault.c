#include "system/fault.h"
#include "hal_gpio.h"
#include "hal_delay.h"

#define DO_ERROR_BLINK
static uint8_t const error_led_id = GPIO_B4;

#define LONG_MS 1150
#define SHORT_MS 500
#define SPACE_MS 150

void fault_panic(error_code_t error_code) {
    hal_gpio_write_ddr(GPIO_B4, 1);
    hal_gpio_write_port(GPIO_B4, 0);
    
    uint8_t module_count = (error_code & 0x10) >> 4; // Which module has error
    uint8_t submodule_count = (error_code & 0x01); // Which error within module

    while (1) {
        for (int i = 0; i < module_count; i++) {
            hal_gpio_write_port(GPIO_B4, 1);
            hal_delay_smart_ms(SPACE_MS);
            hal_gpio_write_port(GPIO_B4, 0);
            hal_delay_smart_ms(SPACE_MS);
        }
        hal_delay_smart_ms(SHORT_MS);

        for (int i = 0; i < submodule_count; i++) {
            hal_gpio_write_port(GPIO_B4, 1);
            hal_delay_smart_ms(SPACE_MS);
            hal_gpio_write_port(GPIO_B4, 0);
            hal_delay_smart_ms(SPACE_MS);
        }
        hal_delay_smart_ms(LONG_MS);
    }
}