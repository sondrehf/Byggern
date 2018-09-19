#ifndef F_CPU
#define F_CPU 4915200
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include "uart.h"
#include "latch.h"
#include "sRAM.h"
#include "adc.h"
#include "usbBoard.h"
#include "oled.h"
#include <stdio.h>
#include "fonts.h"



int main(void){
    USART_init(MYUBRR);
    printf_init();
    extern_mem_init();
    SRAM_test();
    oled_init();

    while(1){
      write_d(0x10);
      _delay_ms(1);
      //write_c(0xA4);





    }
    return 0;
}
