#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../USART/USART.h"
#include "CAN_node2.h"
#include "../Utilities/MCP2515_node2.h"

volatile static can_message msg;


int main(){


  _delay_ms(1000);
  USART_init(MYUBRR);
  mcp2515_init();
  can_init();
  can_set_normal_mode();

  /* INTERRUPT ENABLE */
  // Button input
  DDRB &= ~(1<<PB6);
  // Disable global interrupts
  cli();
  //Enable pin change interrupt on pin : PCINT 0:7
  PCICR |= (1<<PCIE0);
  //Setting PCINT5 interrupt
  PCMSK0 |= (1<<PCINT6);

  //Enable global interrupts
  sei();


  while(1){

    /*_delay_ms(100);
    msg.id = 79;
    msg.length = 4;
    msg.data[0] = (uint8_t)'n';
    msg.data[1] = (uint8_t)'o';
    msg.data[2] = (uint8_t)' ';
    msg.data[3] = (uint8_t)'u';
    can_message_send(&msg);*/

    /*can_message msg;
    msg.id = 1;
    msg.length = 2;
    msg.data[0] = 'U';
    msg.data[1] = 'G';
    can_message_send(&msg);*/
    //Check if RX0IF is high, thus ready to receive new message
    /*if ((mcp2515_read(MCP_CANINTF) & 0b00000001)) {
      can_message msg2;
      msg2 = can_message_receive();
      printf("%d, %x\n\r", msg2.id, msg2.length);
      printf("%d, %d\n", msg2.data[0], msg2.data[1]);
    }*/


    //mcp2515_write(0x36, 0x15);
    //uint8_t value = mcp2515_read(0x36);
    //printf("%d\r\n", value);

    /*printf("%s","CANSTAT: " );
    printf("%x\n\r",mcp2515_read(MCP_CANSTAT));
    printf("%s","CANINTF: " );
    printf("%x\n\r",mcp2515_read(MCP_CANINTF));
    printf("%s","EFLG: " );
    printf("%x\n\r\n\r",mcp2515_read(MCP_EFLG));
*/
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

ISR(PCINT0_vect){
  uint8_t interrupt = mcp2515_read(MCP_CANINTF);
  uint8_t RX0_flag = interrupt & 0b00000001;
  //check to see if received data.
  if (RX0_flag){
    msg = can_message_receive();

  }
}
