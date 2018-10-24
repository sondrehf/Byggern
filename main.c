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
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"



int main(void){
    USART_init(MYUBRR);
    printf_init();
    extern_mem_init();
    //SRAM_test();
    oled_init();
    oled_reset();
    //mcp2515_reset();
    mcp2515_init();
    can_init();
    can_set_normal_mode();

    can_message msg;
    msg.id = 69;
    msg.length = 8;
    msg.data[0] = (uint8_t)'H';
    msg.data[1] = (uint8_t)'e';
    msg.data[2] = (uint8_t)'r';
    msg.data[3] = (uint8_t)'m';
    msg.data[4] = (uint8_t)'E';
    msg.data[5] = (uint8_t)'G';
    msg.data[6] = (uint8_t)'a';
    msg.data[7] = (uint8_t)'y';
    can_message_send(&msg);
/*
    //SPI_read();
    menu_page mainMenu = menu_initialize();
    //mainMenu.options[1].name = "Heisann";
    //printf("%s\n", (*mainMenu.options[1]).name);
    struct joystick_angle pos = calculate_angle();
    enum joystick_direction dir=get_joystick_direction(pos);
    int arrowPos = 0;
    update_menu_page(mainMenu, dir, arrowPos, mainMenu.options);
    menu_page varMenu = mainMenu;*/


    //kan hende vi frya can transmit
    while(1){
      _delay_ms(150);

      if((mcp2515_read(MCP_CANINTF) & 0b00000100)) { //Checking to see if the controller is currently transmitting (TXREQ is high)

        joystick_can_send();
      /*  msg.data[2] = (uint8_t)'r';
        msg.data[3] = (uint8_t)'m';
        msg.data[4] = (uint8_t)'E';
        msg.data[5] = (uint8_t)'G';
        msg.data[6] = (uint8_t)'a';
        msg.data[7] = (uint8_t)'y';*/
        //can_message_send(&msg);
      }
      //mcp2515_write(0x36, 0x15);
      //uint8_t value = mcp2515_read(0x36);
      //printf("%d\r\n", value);

      printf("%s","CANSTAT: " );
      printf("%x\n\r",mcp2515_read(MCP_CANSTAT));
      printf("%s","CANINTF: " );
      printf("%x\n\r",mcp2515_read(MCP_CANINTF));
      printf("%s","EFLG: " );
      printf("%x\n\r\n\r",mcp2515_read(MCP_EFLG));
        /*can_message msg2;
        msg2 = can_message_receive();
        printf("%d, %x\n\r", msg2.id, msg2.length);
        for (size_t i = 0; i < msg2.length; i++) {
          printf("%c ", (char)msg2.data[i]);
        }
        printf("%s","CANSTAT: " );
        printf("%x\n\r",mcp2515_read(MCP_CANSTAT));
        printf("%s","CANINTF: " );
        printf("%x\n\r",mcp2515_read(MCP_CANINTF));
*/

        /*pos = calculate_angle();
        //printf("%s",mainMenu.ouint8_tptions[6].name);            oled_write_letter_sram('A', 8, 0, 0);
        //oled_init_sram();
        //oled_write_letter_sram('A', 8, 0, 0);
        //oled_write_letter_sram('A', 8, 0, 8);
        //oled_read_page_sram(0);
        oled_print_sram("YIA YIA",8,0,0);
        oled_print_sram(":))))) )", 8, 1, 0);
        oled_print_sram("Herman", 5, 7, 0);
        oled_read_screen_sram();
        //oled_read_page_sram(1);
        if (get_joystick_direction(pos) != dir){
            arrowPos = cursor_counter(varMenu, dir, arrowPos);
            printf("%d\n", arrowPos );
            print_dir(dir);
            dir =  get_joystick_direction(pos);
            update_menu_page(varMenu, dir, arrowPos, varMenu.options);
            //printf("%s",mainMenu.opSRAM_testtions[2].name);
            if (dir == RIGHT && varMenu.options[arrowPos] != NULL){
                oled_reset();
                varMenu = *varMenu.options[arrowPos];
            }
            if (dir == LEFT && varMenu.parent != NULL){
                oled_reset();
                varMenu = *varMenu.parent;
            }
        }
        _delay_ms(250);
      //write_d(0x00);
      //writed_d(0xFF);
      //write_c(0xA4);
*/
  }
    return 0;
}
