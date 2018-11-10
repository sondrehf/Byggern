#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "adc_node2.h"


void adc_node2_config(){
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

uint8_t game_over(uint8_t values[], uint8_t* counter, uint8_t length){
  if (*counter == length){
    *counter = 0;
  }
  values[*counter] = adc_conversion();
  printf("%d", adc_conversion());
  (*counter)++;
  uint16_t totalValue=0;
  for(int i = 0; i< length; i++){
    totalValue += values[i];
  }
  printf("value = %d\n\r", totalValue);
  if(totalValue == 0){
    return 1;
  }
  return 0;
}
