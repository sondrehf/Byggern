#ifndef F_CPU
#define F_CPU 4915200
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include <avr/interrupt.h>
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

volatile static can_message msg;

/* Define flags here */
volatile static uint8_t update_oled = 0;



int main(void){



    _delay_ms(1000);
    USART_init(MYUBRR);
    printf_init();
    extern_mem_init();
    oled_init();
    oled_reset();
    mcp2515_init();
    can_init();
    can_set_normal_mode();


    /* OLED AND SRAM TESTING */
    timer_interrupt_for_oled_init();
    oled_clear_sram();
    // Variables used for animation (probably a better way to code it)
    uint8_t distanceFromStart = 120;
    uint8_t distanceFromStart2 = 30;
    uint8_t distanceFromStart3 = 80;
    uint8_t sign = 1;
    uint8_t sign2 = 1;
    uint8_t sign3 = 1;


    /* INTERRUPT ENABLE */


    // Button input

    DDRE &= ~(1<<PE0);
    // Disable global interrupts
    cli();
    // Interrupt on falling edge PE0
    EMCUCR &= ~(1<<ISC2);
    // Enable interrupt on PE0
    GICR |= (1<<INT2);

    // Enable global interrupts
    sei();
    /*INTERRUPT ENABLE FINISHED*/
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

    while(1){
      //_delay_ms(100);

      if (update_oled){
        oled_reset();
        oled_clear_sram();
        //oled_write_platform_horizontal_sram(3, 20, 4);
        //oled_animation_circle_horizontal_sram(6, &distanceFromStart, &sign);
        //oled_animation_circle_horizontal_sram(4, &distanceFromStart2, &sign2);
        //oled_animation_circle_horizontal_sram(2, &distanceFromStart3, &sign3);
        oled_animation_shoot_ball_sram(7, 20, &distanceFromStart, &sign);
        //oled_animation_shoot_ball_sram(4, 20, &distanceFromStart2, &sign2);
        //oled_animation_shoot_ball_sram(2, 20, &distanceFromStart3, &sign3);
        oled_read_screen_sram();
        update_oled = 0;
      }








/*      msg.id = 69;
      msg.length = 8;
      msg.data[0] = (uint8_t)'Y';
      msg.data[1] = (uint8_t)'o';
      msg.data[2] = (uint8_t)'M';
      msg.data[3] = (uint8_t)'o';
      msg.data[4] = (uint8_t)'m';
      msg.data[5] = (uint8_t)'G';
      msg.data[6] = (uint8_t)'a';
      msg.data[7] = (uint8_t)'y';
      can_message_send(&msg);*/

      // IMPORTANT SEND FUNCTION
      //motor_input_can_send();



      /*  msg.data[2] = (uint8_t)'r';
        msg.data[3] = (uint8_t)'m';
        msg.data[4] = (uint8_t)'E';
        msg.data[5] = (uint8_t)'G';
        msg.data[6] = (uint8_t)'a';
        msg.data[7] = (uint8_t)'y';*/
        //can_message_send(&msg);
      //mcp2515_write(0x36, 0x15);
      //uint8_t value = mcp2515_read(0x36);
      //printf("%d\r\n", value);

            /*  printf("%s","CANSTAT: " );
      printf("%x\n\r",mcp2515_read(MCP_CANSTAT));
      printf("%s","CANINTF: " );
      printf("%x\n\r",mcp2515_read(MCP_CANINTF));
      printf("%s","EFLG: " );
      printf("%x\n\r\n\r",mcp2515_read(MCP_EFLG));*/

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


        // ***** OLED AND MENU STUFF *****


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
            print_int8_t interrupt = mcp2515_read(MCP_CANINTF);
  uint8_t RX0_flag = interrupt & 0b00000001;
  printf("%s\n","Hei der, general Kenobi" );
  //check to see if received data.
  if (RX0_flag){
    msg = can_message_receive();
    printf("%d, %x\n\r", msg.id, msg.length);
    printf("%c, %c\n", msg.data[0], msg.data[1]);

  }
  /*dir(dir);
            dir =  get_joystick_direction(pos);
            update_menu_page(varMenu, dir, arrowPos, varMenu.options);
            //printf("%s",mainMenu.opSRAM_testtions[2].name);
            if (dir == RIGHT && varMenu.options[arrowPos] != NULL){
                oled_reset();
                varMenu = *varMenu.options[arrowPos];
            }      mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0); //Resetter RX0IF

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


ISR(INT2_vect){
  uint8_t interrupt = mcp2515_read(MCP_CANINTF);
  uint8_t RX0_flag = interrupt & 0b00000001;
  //check to see if received data.
  if (RX0_flag){
    msg = can_message_receive();

  }
}

// Interrupt for OLED, 60 Hz
ISR(TIMER1_COMPA_vect){
  update_oled = 1;
}
