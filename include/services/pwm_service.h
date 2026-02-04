#include <stdint.h>
#include <stdbool.h>
#include <core/error_code.h>

typedef enum {
    PWM_PIN_PB1,
    PWM_PIN_PB4,
    PWM_NUM_PINS,
} pwm_pin_t;

#define ERROR_PWM_SERVICE_UNINIT (ERROR_MODULE_PWM_SERVICE | 0x01)
#define ERROR_PWM_SERVICE_BAD_PIN (ERROR_MODULE_PWM_SERVICE | 0x02)
#define ERROR_PWM_SERVICE_PIN_UNUSED (ERROR_MODULE_PWM_SERVICE | 0x03)
#define ERROR_PWM_SERVICE_PIN_IN_USE (ERROR_MODULE_PWM_SERVICE | 0x04)
#define ERROR_PWM_SERVICE_FREQ_LOW (ERROR_MODULE_PWM_SERVICE | 0x05)
#define ERROR_PWM_SERVICE_FREQ_HIGH (ERROR_MODULE_PWM_SERVICE | 0x06)


error_code_t pwm_service_init(void);
error_code_t pwm_claim(pwm_pin_t pwm_pin, uint32_t freq_hz, uint8_t duty_percent);
error_code_t pwm_set_frequency(pwm_pin_t pwm_pin, uint32_t freq_hz);
error_code_t pwm_set_duty(pwm_pin_t pwm_pin, uint8_t duty_percent);
error_code_t pwm_unclaim(pwm_pin_t pwm_pin);