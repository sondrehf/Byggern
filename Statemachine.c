#include <stdio.h>
#include "menu.h"
#include "CAN.h"
#include "Statemachine.h"



void play_game(can_message *msg){
  uint8_t gameover = 0;
  while(!gameover){
    oled_reset();
    oled_clear_sram();
    oled_print_sram("Playing Game", 8, 0, 0);
    oled_read_screen_sram();
    motor_input_can_send();
    if((*msg).id==69){
      oled_print_sram("Slipp meg inn", 8, 1,0 );
      (*msg).id = 0;
      gameover = 1; 
    }
  }
}

void state_machine(menu_page* page, can_message *msg){
    switch ((*page).id){
      case 3:
        break;
      case 1:
        play_game(msg);//difficulty);
        //break;
        //*page = menu_initialize();
      case 2:
        *page = menu_initialize();
        break;
      default:
        break;
  }
}
