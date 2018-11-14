#ifndef F_CPU
#define F_CPU 4915200
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <stdio.h>
#include <util/delay.h>
#include "menu.h"
#include "CAN.h"
#include "Statemachine.h"

enum States{YES = 1, NO, STARTNEWGAME, CALIBRATEJOYSTICK, EASY, MEDIUM, HARD, SETDIFFICULTY};
volatile static can_message msg;
volatile static char* difficulty;
volatile static can_message diffMsg;



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
  oled_print_sram(difficulty, 8, 4, 0);
  oled_reset();
  oled_read_screen_sram();
  while(!gameover){
    startTime = 0;
    if (timeCount == endTime) {
        motor_input_can_send();
        timeCount = 0;
    }
    timeCount++;
    if((msg).id==69){
      printf("Å jasså");
      oled_clear_sram();
      oled_print_sram("YOUR SCORE WAS: ", 8, 4 ,0);
      oled_print_sram((char)(msg.data[0]), 8, 5, 0);
      oled_reset();
      oled_read_screen_sram();
      _delay_ms(3000);
      (msg).id = 0;
      gameover = 1;

    }
    //How many ticks for 60Hz
    endTime = 245760;
  }
  printf("HMMMM");
}



void state_machine(menu_page* page){
    switch ((*page).id){
      case STARTNEWGAME: //Start new game
        break;
      case YES: //Start new game -> Yes
        play_game();//difficulty);
        (*page).id = NO;
        break;
      case NO: //Start new game -> Yes
        *page = menu_initialize();
        break;
      case SETDIFFICULTY: //Set Difficulty
        break;
      case EASY: //Set Difficulty -> Easy
        diffMsg.id = 5;
        diffMsg.length = 2;
        diffMsg.data[0] = 1.5;
        diffMsg.data[1] = 1.7;
        can_message_send(&diffMsg);
        difficulty = "Easy";
        (*page).id = YES;
        break;
      case 6: //Set Difficulty -> Medium
        diffMsg.id = 6;
        diffMsg.length = 2;
        diffMsg.data[0] = 2.5;
        diffMsg.data[1] = 2.3;
        can_message_send(&diffMsg);
        difficulty = "Medium";
        (*page).id = YES;
        break;
      case 7: //Set Difficulty -> Hard
        diffMsg.id = 7;
        diffMsg.length = 2;
        diffMsg.data[0] = 5.5;
        diffMsg.data[1] = 1.7;
        can_message_send(&diffMsg);
        difficulty = "Hard";
        (*page).id = YES;
        break;
      default:
        break;
  }
}



//Interrupt for received can message
ISR(INT2_vect){
  //received_message = 1;
  msg = can_message_receive();
  printf("%d\n",msg.id );
}
