#include <stdio.h>
#include "menu.h"
#include "CAN.h"
#include "Statemachine.h"

volatile static can_message msg;


void play_game(){
  uint8_t gameover = 0;
  //timer_interrupt_for_can_init();
  uint8_t startTime, endTime, timeCount;
  timeCount = 0;
  can_message startGame;
  startGame.id = 34;
  startGame.length = 0;
  can_message_send(&startGame);
  oled_clear_sram();
  oled_print_sram("Playing Game", 8, 0, 0);
  oled_reset();
  oled_read_screen_sram();
  while(!gameover){
    startTime = 0;
    if (timeCount == endTime) {
        motor_input_can_send();
        timeCount = 0;
        printf("%s\n", "YIA");
    }
    timeCount++;
    if((msg).id==69){
      printf("%s\n", "I said game over" );
      (msg).id = 0;
      gameover = 1;
    }
    //How many ticks for 60Hz
    endTime = 245760;
  }
}

void state_machine(menu_page* page){
    switch ((*page).id){
      case 3:
        break;
      case 1:
        play_game();//difficulty);
        *page = menu_initialize();
        break;
      case 2:
        *page = menu_initialize();
        break;
      default:
        break;
  }
}



//Interrupt for received can message
ISR(INT2_vect){
  //received_message = 1;
  msg = can_message_receive();
}
