#ifndef F_CPU
#define F_CPU 16000000UL // or whatever may be your frequency
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <util/delay.h>                // for _delay_ms()
#include <avr/io.h>
#include "uart.h"



int main(void){
    //DDRA=0x1; //konfigurere pins. Satt DDRA0 til å være output.
    //unsigned int i = 1;
    USART_init(MYUBRR);
    while(1){
     /* PORTA = (1 << PA0); //Setter
      _delay_ms(200);
      PORTA = (0 << PA0);
      _delay_ms(200);
      //_NOP();
*/
    //USART_transmit('B');
    USART_transmit(USART_receive());
    //_delay_ms(200);
    }
    return 0;
}
