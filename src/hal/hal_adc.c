#include <hal/hal_adc.h>
#include <platform/io.h>
#include "bitwise.h"

void hal_adc_enable(bool enabled) {
    bitwise_write_bit(&ADCSRA, ADEN, enabled);
}

static uint8_t _get_admux_byte(hal_adc_channel_t channel) {
    switch (channel) {
    case ADC_CHANNEL_0: return 0;
    case ADC_CHANNEL_1: return (1 << MUX0);
    case ADC_CHANNEL_2: return (1 << MUX1);
    case ADC_CHANNEL_3: return ((1 << MUX1) | (1 << MUX0));
    default: return 0;
    }
}

static uint8_t _get_adcsra_byte(hal_adc_prescaler_t prescaler) {
    switch (prescaler) {
    case HAL_ADC_PRESCALER_2: return 0;
    case HAL_ADC_PRESCALER_4: return (1 << ADPS1);
    case HAL_ADC_PRESCALER_8: return (1 << ADPS1) | (1 << ADPS0);
    case HAL_ADC_PRESCALER_16: return (1 << ADPS2);
    case HAL_ADC_PRESCALER_32: return (1 << ADPS2) | (1 << ADPS0);
    case HAL_ADC_PRESCALER_64: return (1 << ADPS2) | (1 << ADPS1);
    case HAL_ADC_PRESCALER_128: return (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    default: return 0;
    }
}

void hal_adc_set_mux(hal_adc_channel_t channel) {
    ADMUX = _get_admux_byte(channel);
}

void hal_adc_set_prescaler(hal_adc_prescaler_t prescaler) {
    ADCSRA = _get_adcsra_byte(prescaler);
}
void hal_adc_start_conversion(void) {
    bitwise_write_bit(&ADCSRA, ADSC, 1);
}
uint8_t hal_adc_read(void) {
    return ADCH;
}