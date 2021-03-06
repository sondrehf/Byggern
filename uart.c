#include "uart.h"

void USART_init(unsigned int ubrr){
  //setting baud rate
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char) ubrr;
  //Enable TRX and RXD
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);
  //UCSRB = 0b00011000 //equivalent with over
  //Setting frame format, 8 data, 2 stop
  UCSR0C = (1<<URSEL0) | (1<<USBS0) | (1<<UCSZ00) | (1<<UCSZ01) ;
}

void USART_transmit(uint8_t message){
  while (!(UCSR0A & (1<<UDRE0))); //Waiting for empty data buffer
    /* Do nothing */
  UDR0 = message;
}
char USART_receive(){
  while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

FILE* printf_init(){
    return fdevopen(USART_transmit, USART_receive);
}
