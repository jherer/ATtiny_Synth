#include <stdbool.h>
#include <stdint.h>

typedef enum {
    HAL_ADC_PRESCALER_2,
    HAL_ADC_PRESCALER_4,
    HAL_ADC_PRESCALER_8,
    HAL_ADC_PRESCALER_16,
    HAL_ADC_PRESCALER_32,
    HAL_ADC_PRESCALER_64,
    HAL_ADC_PRESCALER_128,
    HAL_ADC_NUM_PRESCALERS
} hal_adc_prescaler_t;

typedef enum {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_NUM_CHANNELS,
} hal_adc_channel_t;


void hal_adc_enable(bool enabled);
static uint8_t _get_admux_byte(hal_adc_channel_t channel);
static uint8_t _get_adcsra_byte(hal_adc_prescaler_t prescaler);
void hal_adc_set_mux(hal_adc_channel_t channel);
void hal_adc_set_prescaler(hal_adc_prescaler_t prescaler);
void hal_adc_start_conversion(void);
uint8_t hal_adc_read(void);