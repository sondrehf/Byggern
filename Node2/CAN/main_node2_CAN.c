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
  can_set_normal_mode();


  //kan hende vi frya can transmit
  while(1){
    _delay_ms(150);
    //Check if RX0IF is high, thus ready to receive new message
    if ((mcp2515_read(MCP_CANINTF) & 0b00000001)) {
      can_message msg2;
      msg2 = can_message_receive();
      printf("%d, %x\n\r", msg2.id, msg2.length);
      printf("%d, %d\n", msg2.data[0], msg2.data[1]);
    }
    _delay_ms(500);


    //mcp2515_write(0x36, 0x15);
    //uint8_t value = mcp2515_read(0x36);
    //printf("%d\r\n", value);

    printf("%s","CANSTAT: " );
    printf("%x\n\r",mcp2515_read(MCP_CANSTAT));
    printf("%s","CANINTF: " );
    printf("%x\n\r",mcp2515_read(MCP_CANINTF));
    printf("%s","EFLG: " );
    printf("%x\n\r\n\r",mcp2515_read(MCP_EFLG));
/*
    can_message rcd;
    rcd = can_message_receive();
    for(int i = 0; i<rcd.length; i++){
      printf("%c\n", rcd.data[i] );
    }   */
    //SPI_send(0b10011101);
    //SPI_read();

    //mcp2515_write(0x08, 0x09);
    //uint8_t a = mcp2515_read(0x08);
    //printf("%x\n", a);
  }

  return 0;
}
