#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "oled.h"
#include "fonts.h"

#ifndef F_CPU
#define F_CPU 4915200
#endif

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void write_c(uint8_t command){
  volatile char * oled_c = (char *) 0x1000;
  oled_c[0] = command;

}

void write_d(uint8_t data){
  volatile char * oled_d = (char *) 0x1200;
  oled_d[0] = data;
}



void oled_init(){
  write_c(0xae);        //  display  off
  write_c(0xa1);        //segment letter remap
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


  write_c(0xB0);        //Set page
  write_c(0x00);        //Set lower column start Address
  write_c(0x10);        //Set higher column end start Address



}


/* --------------------USES OLED MEMORY HERE--------------------------*/
void oled_clear_line(uint8_t line){
  write_c(0xB0+line);
  for(int j=0; j<128;j++){
    write_d(0x00);
  }
}

void oled_reset(){
  for(int i = 0; i<8; i++){
    oled_clear_line(i);
  }
  write_c(0xB0);
}


/*
menu_page* oled_home(menu_page page){
  //Obsolete?
}
*/
void oled_goto_page(uint8_t page){
  write_c(0xB0 + page);
}

// line 63 is on top, line 0 is the one below. Consistent from here.
void oled_goto_line(uint8_t line){
  /*Please sir, may i have some more... Pants*/
  write_c(0x40 + (63-line));
}


void oled_goto_column(uint8_t column){
  uint8_t lowerBits = column & 0x0F;
  uint8_t upperBits = column & 0xF0;
  upperBits = upperBits >> 4;
  write_c(0x10 + upperBits);
  write_c(0x00 + lowerBits);
}

void oled_write_letter_P(const uint8_t letter, uint8_t fontSize){
  switch(fontSize){
    case 4:
      for(int i = 0; i<4; i++){
        write_d(pgm_read_byte(font4[letter]+i));
      }
      break;
    case 5:
      for(int i = 0; i<5; i++){
        write_d(pgm_read_byte(font5[letter]+i));
      }
      break;
    case 8:
      for(int i = 0; i<8; i++){
        write_d(pgm_read_byte(font8[letter]+i));
      }
      break;
    default:
      for(int i = 0; i<8; i++){
        write_d(pgm_read_byte(font8[53]+i));
      }
      break;
  }
}


void oled_print(const char* data, uint8_t fontSize){
  uint8_t i  = 0;
  while (data[i] != '\0'){
    oled_write_letter_P(data[i] - 32, fontSize);
    i++;
  }
}

void oled_goto_pos(uint8_t row, uint8_t col){
  //oled_goto_line(row*8 + 1);

  oled_goto_page(row);
  oled_goto_column(col);
}

/* END OF OLED MEMORY USE FUNCTIONS */




/* --------------------SRAM MEMORY TO OLED FROM HERE DOWN ----------------------------------*/
void oled_write_d_sram(uint8_t data, uint8_t line, uint8_t col){
  /*  Divides the SRAm into pages corresponding to oledRAM
      Line is page!!!
  */
  volatile char *ext_ram = (char *) 0x1800 + (128*line);
  ext_ram[col] = data;
}


void oled_write_letter_sram(const char letter, uint8_t fontSize, uint8_t line, uint8_t col){
  uint8_t newletter = letter -32;
  switch(fontSize){
    case 4:
      for(int i = 0; i<4; i++){
        oled_write_d_sram(pgm_read_byte(font4[newletter]+i), line, col + i);
      }
      break;
    case 5:
      for(int i = 0; i<5; i++){
        oled_write_d_sram(pgm_read_byte(font5[newletter]+i), line, col + i);
      }
      break;
    case 8:
      for(int i = 0; i<8; i++){
        oled_write_d_sram(pgm_read_byte(font8[newletter]+i), line, col + i);
      }
      break;
    default:
      for(int i = 0; i<8; i++){
        oled_write_d_sram(pgm_read_byte(font8[newletter]+i), line, col + i);
      }
      break;
  }
}

void oled_init_sram(){
  volatile char *ext_ram = (char *) 0x1800;
  for (int i = 0; i < 8*128; i++){
    ext_ram[i] = 0;
  }
}

uint8_t oled_read_d_sram(uint8_t line, uint8_t col){
  /* Divides the SRAm into pages corresponding to oledRAM */
  volatile char *ext_ram = (char *) 0x1800 + (128*line);
  return ext_ram[col];
}

void  oled_read_page_sram(uint8_t line){
  write_c(0xB0 + line);
  for (int i = 0; i < 128; i++){
    write_d(oled_read_d_sram(line, i));
  }
}
void oled_read_screen_sram(){
  for (int i = 0; i < 8; i++){
    oled_read_page_sram(i);
  }
  //_delay_ms(50);
}


void oled_print_sram(const char* data, uint8_t fontSize, uint8_t line, uint8_t col){
  uint8_t i  = 0;
  while (data[i] != '\0'){
    oled_write_letter_sram(data[i], fontSize, line, col + i*8);
    i++;
  }
}
/* END OF SRAM USE HERE */
