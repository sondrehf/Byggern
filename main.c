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
#include "joystick.h"
#include <stdio.h>

#define x_ax 0b0100
#define y_ax 0b0101
#define intr 5



int main(void){
    //DDRA=0x1; //konfigurere pins. Satt DDRA0 til å være output.
    //unsigned int i = 1;
    USART_init(MYUBRR);
    printf_init();
    extern_mem_init();
    SRAM_test();


/*
    GAL - config (Running someone else's program)
    sram = top
    adc = middle
    oled(kanskje) = down
*/


    while(1){
      //adc_config(0);
      _delay_ms(100);
      struct joystick_angle pos = calculate_angle();
      enum joystick_direction dir = get_joystick_direction(pos);
      switch (dir) {
        case NEUTRAL:
          printf("Neutral\n\r");
          break;
        case UP:
          printf("UP\n\r");
          break;
        case DOWN:
          printf("DOWN\n\r");
          break;
        case RIGHT:
          printf("RIGHT\n\r");
          break;
        case LEFT:
          printf("LEFT\n\r");
          break;
        case ERROR:
          printf("ERROR\n\r");
          break;
        default:
          break;
      }
      //printf("%d\n", a.xAngle);
      //printf("%d\n", a.yAngle);

      //_delay_ms(100);
      /*
        volatile char *ext_ram = (char *) 0x1901;
        *ext_ram = 0;
        _delay_ms(200);
        ext_ram=0x1500;
        *ext_ram = 0;
        _delay_ms(200);
      PORTA = (1 << PA0); //Setter
      _delay_ms(200);
      PORTA = (0 << PA0);
      _delay_ms(200);
      //_NOP();*/

    //test_latch();
    //char a = USART_receive();
    //USART_transmit(a);

    //printf("Hei\r\n");
  //  _delay_ms(200);
    //USART_receive();
    }
    return 0;
}
