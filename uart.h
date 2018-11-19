#include <stdio.h>
#include <avr/io.h>

void USART_init(unsigned int ubrr);
void USART_transmit(unsigned char message);
char USART_receive();
FILE* printf_init();
