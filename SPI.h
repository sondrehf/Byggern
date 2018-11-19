#include <avr/io.h>
#include <stdio.h>

void SPI_init();
uint8_t SPI_read();
void SPI_send(uint8_t data);
