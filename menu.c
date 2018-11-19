#include "oled.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>           // for _delay_ms()
#include "usbBoard.h"
#include "menu.h"

menu_page page_init(const char* name, const menu_page* parent, const uint8_t size, const menu_page** options, const int id ){
    menu_page page;
    page.name = name;
    page.size = size;
    page.parent = parent;
    for (int i = 0; i < page.size; i++){
        page.options[i] = options[i];
    }
    page.id = id;
    return page;
}


menu_page menu_initialize(){

  menu_page mainMenu, StartNewGame, SetDifficulty, SeeResetHighscore;

  menu_page Yes = page_init("Yes", &StartNewGame, 0, NULL, 1);
  menu_page No = page_init("No", &StartNewGame, 0, NULL, 2);
  menu_page startOpt[] = {&Yes, &No};
  StartNewGame = page_init("Start New Game", &mainMenu, 2, startOpt, 3);

  menu_page CalibrateJoystick = page_init("Calibrate Joystick", &mainMenu, 0, NULL, 4);

  menu_page Easy = page_init("Easy", &SetDifficulty, 0, NULL, 5);
  menu_page Medium = page_init("Medium", &SetDifficulty, 0, NULL, 6);
  menu_page Hard = page_init("Hard", &SetDifficulty, 0, NULL, 7);
  menu_page difOpt[] = {&Easy, &Medium, &Hard};
  SetDifficulty = page_init("Set Difficulty", &mainMenu, 3, difOpt,8);


  menu_page Debugging = page_init("Debug", &mainMenu, 0, NULL, 9);

  menu_page See = page_init("See", &SeeResetHighscore, 0, NULL, 10);
  menu_page Reset = page_init("Reset", &SeeResetHighscore, 0, NULL, 11);
  menu_page seReOpt[] = {&See, &Reset};
  SeeResetHighscore = page_init("See/Reset Higscore", &mainMenu, 2, seReOpt, 12);

  menu_page mainOpt[] = {&StartNewGame, &SeeResetHighscore, &CalibrateJoystick, &SetDifficulty};//, &Debugging};

  mainMenu = page_init("Main Menu", NULL, 4, mainOpt, 13);
  return mainMenu;
}


void OLED_print_arrow(uint8_t row, uint8_t col){
    oled_goto_pos(row, col);
    write_d(0b00011000);
    write_d(0b00011000);
    write_d(0b01111110);
    write_d(0b00111100);
    write_d(0b00011000);
}
void OLED_erase_arrow(uint8_t row, uint8_t col){
    oled_goto_pos(row, col);
    write_d(0b00000000);
    write_d(0b00000000);
    write_d(0b00000000);
    write_d(0b00000000);
    write_d(0b00000000);
}


void update_menu_page(menu_page page, enum joystick_direction dir,int position, menu_page** options){
    //oled_goto_pos(0, 0);//63-(strlen(page.name)/2));
    int varFont = 8;
    uint8_t strlength;
    for(strlength = 0; page.name[strlength+1] != '\0'; ++strlength);
    if(strlength > 16){
      varFont=5;
    }
    oled_print_sram(page.name,varFont, 0, 0);
    oled_read_page_sram(0);
    for(int i=0; i < page.size ; i++){
        if(i == position){
            OLED_print_arrow_sram(2+(position), 0);
        }
        //oled_goto_page(i+2);
        //oled_goto_column(10);
        oled_print_sram((*options[i]).name, 4, i+2, 10);
        //oled_read_page_sram(i+2);
    }
    //oled_read_screen_sram();

}

int cursor_counter(menu_page page, enum joystick_direction dir, int arrowPos){
    if (dir == UP){
        arrowPos--;
            if((arrowPos)<0){
                arrowPos=page.size-1;
            }
        }
    else if (dir == DOWN){
        arrowPos++;
        if(arrowPos > page.size-1){
            arrowPos = 0;
        }
    }
    return arrowPos;
}
