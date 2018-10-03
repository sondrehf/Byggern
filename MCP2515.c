#include <avr/io.h>
#include <stdio.h>
#include "SPI.h"
#include "MCP2515.h"

#define SS DDB4

uint8_t mcp2515_init(){
  uint8_t value;

  SPI_init();
  mcp2515_reset();

  // Test: check if MCP is in config mode
  value = mcp2515_read(MCP_CANSTAT);
  if (value != 0x80){ // Check if CANSTAT shows config mode
    printf("%s %d\n\r", "MCP NOT in config mode", value);
    return 1;
  }

  return 0;
}

uint8_t mcp2515_read(uint8_t address){
  PORTB &= ~(1 << SS); // Select MCP

  SPI_send(MCP_WRITE);
  SPI_send(address);

  PORTB |= (1 << SS); // Deselect MCP

  return SPI_read();
}

void mcp2515_write(uint8_t address,uint8_t data){
  PORTB &= ~(1 << SS);

  SPI_send(MCP_READ);
  SPI_send(address);
  SPI_send(data);

  PORTB |= (1 << SS);

}

void mcp2515_request_to_send(uint8_t TXiRTS){
  PORTB &= ~(1 << SS);

  switch (TXiRTS) {
    case 1:
      SPI_send(MCP_RTS_TX0);
      break;
    case 2:
      SPI_send(MCP_RTS_TX1);
      break;
    case 3:
      SPI_send(MCP_RTS_TX2);
      break;
    default:
      SPI_send(MCP_RTS_ALL);
      break;

    PORTB |= (1 << SS);
  }
}
uint8_t mcp2515_read_status(){
  PORTB &= ~(1 << SS);

  SPI_send(MCP_READ_STATUS);

  PORTB |= (1 << SS);


  return SPI_read();
}
/* NOT ALL REGISTERS CAN BE ACCESSED, SEE MAP IN DATASHEET */
void mcp2515_bit_modify(uint8_t address, uint8_t maskByte, uint8_t dataByte){
  PORTB &= ~(1 << SS);

  SPI_send(MCP_BITMOD);
  SPI_send(address);
  SPI_send(maskByte);
  SPI_send(dataByte);

  PORTB |= (1 << SS);

}

void mcp2515_reset(){
  PORTB &= ~(1 << SS);

  SPI_send(MCP_RESET);

  PORTB |= (1 << SS);

}
