#include <avr/io.h>
#include <stdio.h>
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define MOSI DDB5
#define SCK DDB7
#define SPI DDRB
#define SS DDB4

void SPI_init(){
  /* Set MOSI and SCK outpuMCP2515t and Slave-select, all others input */
  SPI = (1<<MOSI)|(1<<SCK)|(1<<SS);

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(1<<SPR0);
  //SPCR = (0<<CPOL)|(1<<CPHA)|(0<<DORD);
}


uint8_t SPI_read(){
  uint8_t dummyMessage = 0x00;
  SPDR = dummyMessage;

  while (!(SPSR & (1<<SPIF)));
   // Wait for transmission to complete

  return SPDR;
}

void SPI_send(uint8_t data){

  SPDR = data;
  while (!(SPSR & (1<<SPIF)));
    // Wait for transmission to complete

}
