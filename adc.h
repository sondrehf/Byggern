#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

void adc_config(uint8_t channel);
uint8_t get_channel_value(uint8_t channel);
