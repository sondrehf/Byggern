#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "adc_node2.h"


void adc_config(){
  //ENABLE ADC
  ADCSRA |= (1<<ADEN);
  //Choose input channel selection
  ADCSRB &= ~(1<<MUX5);
  ADMUX &= ~(0x1F);
  //Setting AVCC as ref voltage.
  ADMUX |= (1<<REFS0);
}

int adc_conversion(){
  //Start conversion
  ADCSRA |= (1<<ADSC);
  while (ADCSRA & (1<<ADSC));
  uint8_t lower_bits = ADCL;
  uint8_t higher_bits = ADCH;
  //With or without higher bits, unsure.
  int value = lower_bits + (higher_bits<<8);
  return value;
}

uint8_t score_counter(uint8_t score){
  int value = adc_conversion();
  printf("%d\n\r",value );
  if(value < 10){
    score++;
    printf("%s: %d\n\r","Current score", score);
    printf("%s\n\r", "Restarting in: 3" );
    _delay_ms(1000);
    printf("%s\n\r", "2" );
    _delay_ms(1000);
    printf("%s\n", "1" );
    _delay_ms(1000);

  }
  return score;
}
