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
#include "Statemachine.h"

volatile static can_message msg;

/* Define flags here */
volatile static uint8_t update_oled = 0;
volatile static int received_message = 0;



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

    cli();
    /* OLED AND SRAM TESTING */
    timer_interrupt_for_oled_init();
    oled_clear_sram();
    // Variables used for animation (probably a better way to code it)
    timer_interrupt_for_can_init();
    uint8_t distanceFromStart = 120;
    uint8_t sign = 1;


    /* INTERRUPT ENABLE */
    // Button input
    DDRE &= ~(1<<PE0);
    // Disable global interrupts
    // Interrupt on falling edge PE0
    EMCUCR &= ~(1<<ISC2);
    // Enable interrupt on PE0
    GICR |= (1<<INT2);

    // Enable global interrupts
    sei();
    /*INTERRUPT ENABLE FINISHED*/






    menu_page mainMenu = menu_initialize();
    struct joystick_angle pos = calculate_angle();
    enum joystick_direction dir=get_joystick_direction(pos);
    enum joystick_direction lastDir;
    int arrowPos = 0;
    update_menu_page(mainMenu, dir, arrowPos, mainMenu.options);
    menu_page varMenu = mainMenu;
    while(1){
      //startTime = ;
      if (received_message){//messageTimer >= 1/60 received_message){
        //messageTimer = 0;
        msg = can_message_receive();
        //printf("%d",msg.id);
        received_message = 0;
      }
      if (update_oled){
        oled_clear_sram();

        oled_animation_shoot_ball_sram(7, 20, &distanceFromStart, &sign);
        //oled_read_screen_sram();
        // IMPORTANT SEND FUNCTION
        //motor_input_can_send();
        pos = calculate_angle();

        // ***** OLED AND MENU STUFF *****
        if (get_joystick_direction(pos) != dir){
          arrowPos = cursor_counter(varMenu, dir, arrowPos);
        }
        lastDir = dir;
        dir =  get_joystick_direction(pos);
        update_menu_page(varMenu, dir, arrowPos, varMenu.options);
        if (dir == RIGHT && varMenu.options[arrowPos] != NULL && lastDir != dir){
          varMenu = *varMenu.options[arrowPos];
        }
        if (dir == LEFT && varMenu.parent != NULL && lastDir != dir){
          varMenu = *varMenu.parent;
        }
        oled_reset();
        oled_read_screen_sram();
        //reset interrupt flag
        update_oled = 0;
        state_machine(&varMenu, &msg);

    }
    //endTime =
    //messageTimer += endTime - startTime;

  }
    return 0;
}

//Interrupt for received can message
ISR(INT2_vect){
  received_message = 1;

}

// Interrupt for OLED, 60 Hz
ISR(TIMER1_COMPA_vect){
  update_oled = 1;
}
