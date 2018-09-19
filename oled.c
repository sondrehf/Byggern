#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include "oled.h"

void write_c(uint8_t command){
  volatile char * oled_c = (char *) 0x1000;
  oled_c[0] = command;

}

void write_d(uint8_t data){
  volatile char * oled_d = (char *) 0x1200;
  oled_d[0] = data;
}

void fill_all_pages(){
  for (int i = 0; i < 8; i++){
    write_c(0xB0+i);
    write_c(0x00);        //Set lower column start Address
    write_c(0x10);        //Set higher column start Address

    for(int j = 0; j < 8; j++){
      write_d(0xFF);
    }
  }
}


void oled_init(){
  write_c(0xae);        //  display  off
  write_c(0xa1);        //segment  remap
  write_c(0xda);        //common  pads  hardware:  alternative
  write_c(0x12);
  write_c(0xc8);        //common  output scan direction:com63~com0
  write_c(0xa8);        //multiplex  ration  mode:63
  write_c(0x3f);
  write_c(0xd5);        //display  divide ratio/osc. freq. mode
  write_c(0x80);
  write_c(0x81);        //contrast  control
  write_c(0x50);
  write_c(0xd9);        //set  pre-charge  period
  write_c(0x21);
  write_c(0x20);        //Set  Memory  Addressing  Mode
  write_c(0x02);        //Horizontal addressing mode
  write_c(0xdb);        //VCOM  deselect  level  mode
  write_c(0x30);
  write_c(0xad);        //master  configuration
  write_c(0x00);
  write_c(0xa4);        //out  follows  RAM  content
  write_c(0xa6);        //set  normal  display
  write_c(0xaf);        //  display  on

/*
  write_c(0x21);        //Set column
  write_c(0x00);        //Set start Address
  write_c(0x00);        //Set end start Address

  write_c(0x22);
  write_c(0x00);
  write_c(0x00);
*/


}

void oled_reset(){
  write_c(0xae);
  write_c(0xaf);
}

void oled_home(){

}

uint8_t oled_goto_line(uint8_t line){

}

uint8_t oled_goto_column(uint8_t column){
  /*Insert code here*/
}