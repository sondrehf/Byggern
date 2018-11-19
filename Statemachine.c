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
#include "sRAM.h"
#include "usbBoard.h"

enum States{PLAY = 1, STARTNEWGAME = 3, CALIBRATEJOYSTICK, EASY, MEDIUM, HARD, SETDIFFICULTY, SEE = 10, RESET = 11, SEERESETHIGHSCORE = 12 };
volatile static can_message msg;
volatile static char* difficulty;
volatile static can_message diffMsg;
volatile static uint8_t sendMessage = 0;
volatile static uint8_t receivedMessage = 0;

void display_scores(){
  //printf("\n\r%c %c %c: %d\n\r", read_from_EEPROM(0), read_from_EEPROM(1), read_from_EEPROM(2), read_from_EEPROM(3));
  char number[4];
  oled_clear_sram();
  oled_print_sram("Highscore", 8, 0, 0);
  uint8_t i = 1;
  char lineNumber[3];
  for(uint8_t j = 3; j<24; j+=4){
    sprintf(lineNumber, "%d", i);
    lineNumber[1] = '.';
    lineNumber[2] = '\0';
    oled_print_sram(&lineNumber, 5, i, 0);
    oled_write_letter_sram(read_from_EEPROM(j-3)-128, 5, i, 16);
    oled_write_letter_sram(read_from_EEPROM(j-2)-128, 5, i, 24);
    oled_write_letter_sram(read_from_EEPROM(j-1)-128, 5, i, 32);
    sprintf(number, "%d", read_from_EEPROM(j));
    oled_print_sram(&number, 5, i, 50);
    i++;
  }
  oled_read_screen_sram();
  while(get_x_raw_value() > 5);
}

void play_game(){

  uint32_t startTime, endTime, timeCount;
  timeCount = 0;
  can_message startGame;
  startGame.id = 34;
  startGame.length = 0;
  uint8_t gameover = 0;
  can_message_send(&startGame);

  char score[10];
  while(!gameover){
    /*if (receivedMessage){
      msg = can_message_receive();
    }*/

    motor_input_can_send();
    sendMessage = 0;

    sprintf(score, "%d", msg.data[0]);
    oled_clear_sram();
    oled_print_sram(&score, 5, 7, 0);
    oled_print_sram("Playing Game", 8, 0, 0);
    oled_print_sram(difficulty, 8, 4, 0);
    oled_read_screen_sram();
    if((msg).id==1){
      oled_clear_sram();
      oled_print_sram("YOUR SCORE WAS: ", 8, 4 ,0);
      oled_print_sram("ENTER NAME ON PC", 5, 3, 0);
      itoa(msg.data[0], score,10);
      printf("%d\n",msg.data[0] );
      oled_print_sram(&score, 8, 5, 0);
      oled_read_screen_sram();
      saveHighScore(msg.data[0]);
      (msg).id = 0;
      gameover = 1;
    }
    //Tried to implement timer interrupt, but didnt work, something about timing destroyed our code.
    _delay_ms(1000/60.0);
  }
  //Disable interrupt when we dont need it
  //GICR &= ~(1<<INT2);
}



void state_machine(menu_page* page){
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
        diffMsg.data[1] = 1.2;
        can_message_send(&diffMsg);
        difficulty = "Easy";
        (*page).id = PLAY;
        break;
      case MEDIUM: //Set Difficulty -> Medium
        diffMsg.id = 6;
        diffMsg.length = 2;
        diffMsg.data[0] = 2.5;
        diffMsg.data[1] = 1.8;
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
      case SEERESETHIGHSCORE:
        //Needed to hard update because the EEPROM and Menu somehow interacted
        update_menu_page(*page, 0, 1, page->options);
        break;
      case SEE:
        display_scores();
        (*page).id = SETDIFFICULTY;
        //(page) = page->parent;
        break;
      case RESET:
        init_highScore();
        display_scores();
        (*page).id = SETDIFFICULTY;
        break;
      default:
        break;
    }
}



//Interrupt for received can message
ISR(INT2_vect){
  receivedMessage = 1;
  msg = can_message_receive();
  //printf("%d\n",msg.id );
}

/*ISR(TIMER3_COMPA_vect){
  printf("%s\n\r","MHVAFF" );
  //sendMessage = 1;
}*/
