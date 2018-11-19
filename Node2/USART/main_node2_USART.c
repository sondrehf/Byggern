#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include "USART.h"
#include "../Motor/motor.h"

int main(){
  USART_init(MYUBRR);
  DDRD = (1<<PD2);
  while(1){
    PORTD |= (1<<PD2);
    printf("%d\n",PORTD );
    _delay_ms(50);
    PORTD &= ~(1<<PD2);
    printf("%d\n", PORTD);
    _delay_ms(50);
  }
  return 0;
}
