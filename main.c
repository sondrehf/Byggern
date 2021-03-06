#ifndef F_CPU
#define F_CPU 4915200
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/pgmspace.h>
#include "Statemachine.h"
#include "uart.h"
#include "sRAM.h"

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

    /* INTERRUPT ENABLE */
    cli();
    /* OLED AND SRAM TESTING */
    timer_interrupt_for_oled_init();
    timer_interrupt_for_can_init();
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

    // Variables used for animation
    oled_clear_sram();
    uint8_t distanceFromStart = 120;
    uint8_t sign = 1;

    //menu_page initialize
    menu_page mainMenu = menu_initialize();
    struct joystick_angle pos = calculate_angle();
    enum joystick_direction dir=get_joystick_direction(pos);
    enum joystick_direction lastDir;
    int arrowPos = 0;
    update_menu_page(mainMenu, dir, arrowPos, mainMenu.options);
    menu_page varMenu = mainMenu;

    //Standard values for PI-controller (Easy mode)
    can_message diffMsg;
    diffMsg.id = 5;
    diffMsg.length = 2;
    diffMsg.data[0] = 1.5;
    diffMsg.data[1] = 1.7;
    can_message_send(&diffMsg);

    while(1){
      if (update_oled){
        menu_page mainMenu = menu_initialize();
        oled_clear_sram();
        oled_animation_shoot_ball_sram(7, 20, &distanceFromStart, &sign);
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
          arrowPos = 0;
        }
        if (dir == LEFT && varMenu.parent != NULL && lastDir != dir){
          varMenu = *varMenu.parent;
          arrowPos = 0;
        }
        oled_read_screen_sram();
        //reset interrupt flag
        update_oled = 0;
        state_machine(&varMenu);
      }
    }
    return 0;
}

// Interrupt for OLED, 60 Hz
ISR(TIMER1_COMPA_vect){
  update_oled = 1;
}
