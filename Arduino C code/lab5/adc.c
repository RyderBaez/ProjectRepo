#include <avr/io.h>

#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC
    ADMUX |= (1<<6) | (1 << ADLAR);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

uint8_t adc_sample(uint8_t channel)
{
    // Set ADC input mux bits to 'channel' value
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
    ADMUX |= (channel);
    ADCSRA |= (1<<ADSC);
    // Convert an analog input and return the 8-bit result
    while((ADCSRA & ((1<<ADSC)))){//waits for return value
    }
    return ADCH;
}
