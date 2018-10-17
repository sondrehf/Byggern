#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include "../USART/USART.h"
#include "CAN_node2.h"
#include "../Utilities/MCP2515_node2.h"

int main(){
  USART_init(MYUBRR);
  mcp2515_init();
  can_init();
  can_message msg;
  msg.id = 69;
  msg.length = 8;
  msg.data[0] = (uint8_t)'H';
  msg.data[1] = (uint8_t)'e';
  msg.data[2] = (uint8_t)'r';
  msg.data[3] = (uint8_t)'m';
  msg.data[4] = (uint8_t)'E';
  msg.data[5] = (uint8_t)'G';
  msg.data[6] = (uint8_t)'a';
  msg.data[7] = (uint8_t)'y';
  can_message_send(&msg);
  printf("%s\n", "QUE!?" );
  can_message msg2;
  msg2 = can_message_receive();
  printf("%d, %d\n\r", msg2.id);//, msg2.length);
  for (size_t i = 0; i < 8; i++) {
    printf("%c ", (char)msg2.data[i]);
  }




  while(1){
    _delay_ms(50);
    //SPI_send(0b10011101);
    //SPI_read();

    //mcp2515_write(0x08, 0x09);
    //uint8_t a = mcp2515_read(0x08);
    //printf("%x\n", a);
  }

  return 0;
}
