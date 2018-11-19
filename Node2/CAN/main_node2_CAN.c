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
