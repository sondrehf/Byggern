#ifndef F_CPU
#define F_CPU 16000000
#endif

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>

#define MOSI DDB2
#define SCK DDB1
#define SPI DDRB
#define SS DDB0
#define SS2 DDB7

void SPI_init(){
  MCUCR = (1<<SRE);
  /* Set MOSI and SCK outpuMCP2515t and Slave-select, all others input */
  SPI = (1<<MOSI)|(1<<SCK)|(1<<SS)|(1<<SS2);

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
