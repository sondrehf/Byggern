#include <avr/io.h>
#include <stdio.h>
#include "../Utilities/MCP2515_node2.h"
#include "CAN_node2.h"

#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0DLC 0x35
#define MCP_TXB0D0 0x36
#define MCP_RXB0DLC 0x65
#define MCP_RXB0D0 0x66
#define MCP_RXB0SIDL 0x62

void can_init(){
  mcp2515_write(MCP_CANCTRL, MODE_LOOPBACK);

  mcp2515_write(MCP_CANINTE, MCP_TX_INT); //Enable all interrupts from transmit buffers
  mcp2515_write(MCP_CANINTE, MCP_RX_INT); //Enable all interrupts from receive buffers

  //Setting the TXBxCTRL to zero, ready to send msg
  mcp2515_write(MCP_TXB0CTRL, 0);
  mcp2515_write(MCP_TXB1CTRL, 0);
  mcp2515_write(MCP_TXB2CTRL, 0);
}

void can_set_normal_mode(){
  mcp2515_write(MCP_CANCTRL, MODE_NORMAL);
}

uint8_t can_message_send(can_message* msg){
  if (mcp2515_read_status() & 0b00001000) {return 1;} //Checking to see if the controller is currently transmitting (TXREQ is high)

  //Splitting id into higher and lower MSBs/LSBs
  unsigned id_high = msg->id & 0b11111111000;
  unsigned id_low = msg->id & 0b00000000111;
  id_low = id_low << 5;
  id_high = id_high >> 3;
  mcp2515_write(MCP_TXB0SIDH, id_high); //writing to TXB0SIDH, setting higher part address
  mcp2515_write(MCP_TXB0SIDL, id_low); //writing to TXB0SIDL, setting lower part address

  //Setting the data length, 4 lowest is 1
  mcp2515_write(MCP_TXB0DLC, msg->length);

  //Sending data
  for(uint8_t i = 0; i < msg->length; i++){
    mcp2515_write(MCP_TXB0D0+i, msg->data[i]);
  }

  //Setting TX0RTS low to initiate transmission
  mcp2515_request_to_send(0);
  while(!(mcp2515_read_status() & 0b00000100)); // Checking the interrupt flag of TF0IF
  return 0;
}

can_message can_message_receive(){
  can_message msg;
  if (mcp2515_read_status() & 0x01){ //Checking to see if CAN is ready for a new message (RX0IF is high)

    //Reading the identity
    msg.id = mcp2515_read(MCP_RXB0SIDH); //reading 8 highest bits
    msg.id = msg.id << 3; //Left shifting 3 times to create 11 bits total
    unsigned int tempValue = mcp2515_read(MCP_RXB0SIDL) >> 5; //Reading the 8 lowest bits, right shifting 5 times to get 3 bits
    msg.id += tempValue; //adding the 3 last bits to the 8 already stored.

    //Reading length of message
    msg.length = mcp2515_read(MCP_RXB0DLC) & 0b00001111; //Want the 4 LSBs

    //Reading the data
    for(uint8_t i = 0; i < msg.length; i++){
      msg.data[i] = mcp2515_read(MCP_RXB0D0+i);
    }

    //Resetting interrupt bit
    uint8_t tempStatus = mcp2515_read_status();
    mcp2515_write(MCP_CANINTF, tempStatus & 0b11111110);
    return msg;
  }
  msg.id = 0;
  msg.length = -1;
  for(uint8_t i = 0; i < 8; i++){
    msg.data[i] = 0;
  }
  return msg;
}

//Interrupt handling, up to ATMEGA to decide what to do with the interrupt
//If different to zero an interrupt has occured
uint8_t can_int_vect(){
  return mcp2515_read(MCP_CANINTF);
}
/*
int can_error(){
  if(can_message_send() == 1 ){
    printf("%s\n\r", "Error, message could not send");
  }
  if(can_message_receive().length == -1 ){
    printf("%s\n\r", "Error, could not read message");
  }
  return can_message_send() == 1 || can_message_receive().length == -1;
}*/
