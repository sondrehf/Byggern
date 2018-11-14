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
#include <avr/interrupt.h>

enum States{PLAY = 1, STARTNEWGAME = 3, CALIBRATEJOYSTICK, EASY, MEDIUM, HARD, SETDIFFICULTY};
volatile static can_message msg;
volatile static char* difficulty;
volatile static can_message diffMsg;



void play_game(){
  cli();
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

  uint8_t startTime, endTime, timeCount;
  timeCount = 0;
  can_message startGame;
  startGame.id = 34;
  startGame.length = 0;
  uint8_t gameover = 0;
  can_message_send(&startGame);
  oled_clear_sram();
  oled_print_sram("Playing Game", 8, 0, 0);
  oled_print_sram(difficulty, 8, 4, 0);
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
      oled_read_screen_sram();
      _delay_ms(3000);
      (msg).id = 0;
      gameover = 1;

    }
    //How many ticks for 60Hz
    endTime = 245760;
  }
}



void state_machine(menu_page* page){
    switch ((*page).id){
      case STARTNEWGAME: //Start new game
      *page = *page->parent->options[3];
        break;
      case PLAY: //Start new game -> Yes
        play_game();//difficulty);
        (*page) = *page->parent->parent;
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
        (*page).id = PLAY;
        break;
      case MEDIUM: //Set Difficulty -> Medium
        diffMsg.id = 6;
        diffMsg.length = 2;
        diffMsg.data[0] = 2.5;
        diffMsg.data[1] = 2.3;
        can_message_send(&diffMsg);
        difficulty = "Medium";
        (*page).id = PLAY;
        break;
      case HARD: //Set Difficulty -> Hard
        diffMsg.id = 7;
        diffMsg.length = 2;
        diffMsg.data[0] = 5.5;
        diffMsg.data[1] = 1.7;
        can_message_send(&diffMsg);
        difficulty = "Hard";
        (*page).id = PLAY;
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
