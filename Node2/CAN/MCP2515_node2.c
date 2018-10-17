#include <avr/io.h>
#include <stdio.h>
#include "SPI_node2.h"
#include "MCP2515_node2.h"
#include <util/delay.h>

#define SS DDB0
#define SS2 DDB7
#define CONFIG_MODE 0x80

uint8_t mcp2515_init(){
  uint8_t value;

  SPI_init();
  mcp2515_reset();

  mcp2515_write(MCP_CANCTRL, 0b10000000);
  // Test: check if MCP is in config mode
  value = mcp2515_read(MCP_CANSTAT);


  if (value != CONFIG_MODE){ // Check if CANSTAT shows config mode
    printf("%s %x\n\r", "MCP NOT in config mode", value);
    return 1;
  }
  printf("%s %x\r\n", "MCP in config mode", value);

return 0;
}

uint8_t mcp2515_read(uint8_t address){
  PORTB &= ~(1 << SS);
  PORTB &= ~(1 << SS2); // Select MCP (lowering CS_bar pin)
  SPI_send(MCP_READ);
  SPI_send(address);


  uint8_t value = SPI_read();

  PORTB |= (1 << SS); // Deselect MCP (raising CS_bar pin)
  PORTB |= (1 << SS2);
  return value;
}

void mcp2515_write(uint8_t address,uint8_t data){
  PORTB &= ~(1 << SS);
  PORTB &= ~(1 << SS2);

  SPI_send(MCP_WRITE);
  SPI_send(address);
  SPI_send(data);

  PORTB |= (1 << SS);
  PORTB |= (1 << SS2);

}

void mcp2515_request_to_send(uint8_t TXiRTS){
  PORTB &= ~(1 << SS);
  PORTB &= ~(1 << SS2);

  switch (TXiRTS) {
    case 0:
      SPI_send(MCP_RTS_TX0);
      break;
    case 1:
      SPI_send(MCP_RTS_TX1);
      break;
    case 2:
      SPI_send(MCP_RTS_TX2);
      break;
    default:
      SPI_send(MCP_RTS_ALL);
      break;

    PORTB |= (1 << SS);
    PORTB |= (1 << SS2);
  }
}
uint8_t mcp2515_read_status(){
  PORTB &= ~(1 << SS);
  PORTB &= ~(1<<SS2);

  SPI_send(MCP_READ_STATUS);
  uint8_t value = SPI_read();

  PORTB |= (1 << SS);
  PORTB |= (1 << SS2);
  return value;
}

/* NOT ALL REGISTERS CAN BE ACCESSED, SEE MAP IN DATASHEET */
void mcp2515_bit_modify(uint8_t address, uint8_t maskByte, uint8_t dataByte){
  PORTB &= ~(1 << SS);
  PORTB &= ~(1 << SS2);

  SPI_send(MCP_BITMOD);
  SPI_send(address);
  SPI_send(maskByte);
  SPI_send(dataByte);

  PORTB |= (1 << SS);
  PORTB |= (1 << SS2);
}

void mcp2515_reset(){
  PORTB &= ~(1 << SS);
  PORTB &= ~(1 << SS2);

  SPI_send(MCP_RESET);

  PORTB |= (1 << SS);
  PORTB |= (1 << SS2);

}
