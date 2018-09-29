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

    menu_page mainMenu = menu_initialize();

    //mainMenu.options[1].name = "Heisann";
    //printf("%s\n", (*mainMenu.options[1]).name);


    struct joystick_angle pos = calculate_angle();
    enum joystick_direction dir=get_joystick_direction(pos);
    int arrowPos = 0;
    //update_menu_page(mainMenu, dir, arrowPos, mainMenu.options);

    menu_page varMenu = mainMenu;

    while(1){
        pos = calculate_angle();
        //printf("%s",mainMenu.ouint8_tptions[6].name);            oled_write_letter_sram('A', 8, 0, 0);
        oled_init_sram();
        //oled_write_letter_sram('A', 8, 0, 0);
        //oled_write_letter_sram('A', 8, 0, 8);

        //oled_read_page_sram(0);
        oled_print_sram("YIA YIA",8,0,0);
        oled_print_sram(":))))) )", 8, 1, 0);
        oled_print_sram("Herman er gay", 5, 7, 0);
        oled_read_screen_sram();
        //oled_read_page_sram(1);



        if (get_joystick_direction(pos) != dir){
            arrowPos = cursor_counter(varMenu, dir, arrowPos);
            printf("%d\n", arrowPos );
            print_dir(dir);
            dir =  get_joystick_direction(pos);

            //update_menu_page(varMenu, dir, arrowPos, varMenu.options);
            //printf("%s",mainMenu.options[2].name);
/*
            if (dir == RIGHT && varMenu.options[arrowPos] != NULL){
                oled_reset();
                varMenu = *varMenu.options[arrowPos];
            }
            if (dir == LEFT && varMenu.parent != NULL){
                oled_reset();
                varMenu = *varMenu.parent;
            }
*/

        }
        _delay_ms(250);

      //write_d(0x00);
      //writed_d(0xFF);
      //write_c(0xA4);

    }
    return 0;
}
