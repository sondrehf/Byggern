#include "MCP2515.h"

#define SS DDB4
#define CONFIG_MODE 0x80

uint8_t mcp2515_init(){
  uint8_t value;

  SPI_init();
  mcp2515_reset();
  _delay_ms(10);
  mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, CONFIG_MODE);

  //mcp2515_reset();
  _delay_ms(10);

  // Test: check if MCP is in config mode
  value = mcp2515_read(MCP_CANSTAT) & MODE_MASK;
  while(value!=CONFIG_MODE){
    value = mcp2515_read(MCP_CANSTAT);  //Waiting for config mode to be ready
    //mcp2515_reset();
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, CONFIG_MODE);
    _delay_ms(10);
  }

  if (value != CONFIG_MODE){ // Check if CANSTAT shows config mode
    printf("%s %d\n\r", "MCP NOT in config mode", value);
    return 1;
  }
  printf("%s\r\n", "MCP in config mode");

  return 0;
}

uint8_t mcp2515_read(uint8_t address){
  PORTB &= ~(1 << SS); // Select MCP (lowering CS_bar pin)
  SPI_send(MCP_READ);
  SPI_send(address);

  uint8_t value = SPI_read();

  PORTB |= (1 << SS); // Deselect MCP (raising CS_bar pin)

  return value;
}

void mcp2515_write(uint8_t address,uint8_t data){
  PORTB &= ~(1 << SS);

  SPI_send(MCP_WRITE);
  SPI_send(address);
  SPI_send(data);

  PORTB |= (1 << SS);
}

void mcp2515_request_to_send(uint8_t TXiRTS){
  PORTB &= ~(1 << SS);

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
  }
}
uint8_t mcp2515_read_status(){
  PORTB &= ~(1 << SS);

  SPI_send(MCP_READ_STATUS);
  uint8_t value = SPI_read();

  PORTB |= (1 << SS);

  return value;
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
