#ifndef F_CPU
#define F_CPU 4915200
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "latch.h"
#include "sRAM.h"
#include "adc.h"
#include "usbBoard.h"
#include "oled.h"
#include "menu.h"





int main(void){
    USART_init(MYUBRR);
    printf_init();
    extern_mem_init();
    SRAM_test();
    oled_init();
    oled_reset();

    struct menu_page mainMenu = menu_initialize();
    printf("%s\n", mainMenu.options[5].name);


    struct joystick_angle pos = calculate_angle();
    enum joystick_direction dir=get_joystick_direction(pos);
    int arrowPos = 0;
    update_menu_page(mainMenu, dir, arrowPos);

    struct menu_page varMenu = mainMenu;


    while(1){
        pos = calculate_angle();
        printf("%s",mainMenu.options[6].name);
        if (get_joystick_direction(pos) != dir){
            arrowPos = cursor_counter(varMenu, dir, arrowPos);

            //print_dir(dir);
            dir =  get_joystick_direction(pos);
            update_menu_page(varMenu, dir, arrowPos);
            printf("%s",mainMenu.options[2].name);

            if (dir == RIGHT){
                oled_reset();
                varMenu = varMenu.options[arrowPos];
            }
            if (dir == LEFT){
                oled_reset();
                varMenu = *varMenu.parent;
            }


        }
        _delay_ms(50);

      //write_d(0x00);
      //writed_d(0xFF);
      //write_c(0xA4);

    }
    return 0;
}
