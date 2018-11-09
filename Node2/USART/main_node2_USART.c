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

int main(){
  USART_init(MYUBRR);
  printf("%s\n", "hei");
  while(1){
    USART_transmit('H');
    char a = USART_receive();
    printf(a);
  }
  return 0;
}
