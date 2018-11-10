#ifndef F_CPU
#define F_CPU 4915200
#endif
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>


/*---------FACTS ABOUT THE JOYSTICK!-----------*/
/*
Right slider, PinD2
Right Button, PinD1
Left slider, PinB5
Left button, PinB6
CUTOFF FREQUENCY: 1/(2*PI*R*C) = 796Hz
SLOPE: 1st ORDER => 1*6dB/octave = 6dB/octave
*/
/*----------CHANNEL CONFIGURATION--------------*/
/*A
          Channel 0 = Y-axis
          Channel 1 = X-axis
          Channel 2 = Left slider
          Channel 3 = Right slider
*/
/*----------BUTTON CONFIGURATION--------------*/
/*        Button right -> Pin PB0 (atmel162)
          Button left -> Pin PB1 (atmel162)
*/

void adc_config(uint8_t channel){
  if(channel>3 || channel < 0){
  }
  else{
    volatile char * ext_adc = (char*) 0x1400;
    ext_adc[0] = 4+channel; //config channel CH(channel) (single ended)
    _delay_us(40);
    uint8_t retrieved_value = ext_adc[0];
    //Setting up the Pin for right button
    DDRB &= ~(0<<PB0);
  }
}
uint8_t get_channel_value(uint8_t channel){
  volatile char * ext_adc = (char*) 0x1400;
  ext_adc[0] = 4+channel; //config channel CH(channel) (single ended)
  _delay_us(40);
  uint8_t retrieved_value = ext_adc[0];
  return retrieved_value;
}

uint8_t get_button_value(){
  return PINB & 1;
}
