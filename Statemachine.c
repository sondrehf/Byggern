#include <stdio.h>
#include "menu.h"
#include "CAN.h"
#include "Statemachine.h"

volatile static sendMessage = 0;

void play_game(can_message *msg){
  uint8_t gameover = 0;
  //timer_interrupt_for_can_init();
  uint8_t startTime, endTime, timeCount;
  timeCount = 0;
  oled_clear_sram();
  oled_print_sram("Playing Game", 8, 0, 0);
  oled_reset();
  oled_read_screen_sram();
  while(!gameover){
    startTime = 0;
    if (timeCount == endTime) {
        motor_input_can_send();
        timeCount = 0;
    }
    timeCount++;
    if((*msg).id==69){
      oled_print_sram("Slipp meg inn", 8, 1,0 );
      (*msg).id = 0;
      gameover = 1;
    }
    //How many ticks for 60Hz
    endTime = 245760;
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


ISR(TIMER1_COMPB_vect){
  printf("%s\n\r", "lyf");
  sendMessage = 1;
}
