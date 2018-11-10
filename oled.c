#include <util/delay.h>           // for _delay_ms()
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
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
/* MEMORY FOR SRAM CORRESPONDING TO OLED STARTS AT 0x1800 */

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

void oled_clear_sram(){
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

// There are a total of 64 rows
// The old bits lying at the particular col are deleted
// Not very useful function
void oled_write_pixel_sram(uint8_t row, uint8_t col){
  uint8_t page = row / 8;
  uint8_t row_in_page = row - (page * 8);

  uint8_t data = 1 << row_in_page;

  oled_write_d_sram(data, page, col);
}

/* Animation and stuff. Uses SRAM */
void oled_write_circle_sram(uint8_t line, uint8_t col){
  // Hardcoding a circle
  oled_write_d_sram(0b00111100, line, col);
  oled_write_d_sram(0b01000010, line, col+1);
  oled_write_d_sram(0b10000001, line, col+2);
  oled_write_d_sram(0b10000001, line, col+3);
  oled_write_d_sram(0b10000001, line, col+4);
  oled_write_d_sram(0b10000001, line, col+5);
  oled_write_d_sram(0b01000010, line, col+6);
  oled_write_d_sram(0b00111100, line, col+7);
}

// A little trouble when the circle is all the way left
void oled_animation_circle_horizontal_sram(uint8_t line, uint8_t* distanceFromStart, uint8_t* sign){
  oled_write_circle_sram(line, *distanceFromStart);
  if(*distanceFromStart + 8 > 127){
    *sign = -1;
  }
  // Set this to 2 because it simplified shoot_ball animation
  else if (*distanceFromStart == 2){
    *sign = 1;
  }
  *distanceFromStart += *sign;
}

// Obsolete
void oled_write_platform_vertical_sram(uint8_t line, uint8_t col, uint8_t height){
  uint8_t top = 0b10000000;
  top = top >> (height - 1);
  uint8_t pole = 0b11111111;
  pole &= (pole << (8 - height));
  for (int i = 0; i <9 ; i++){
    if (i == 4){
        oled_write_d_sram(pole, line, col + i);
    }
    else{
        oled_write_d_sram(top, line, col + i);
    }
  }
}

void oled_write_platform_horizontal_sram(uint8_t line, uint8_t col, uint8_t height){
  for (int i = 0; i < height; i++){
    if (i == (height - 1)){
      oled_write_d_sram(0b11111111, line, col+i);
    }
    else{
      oled_write_d_sram(0b00011000, line, col +i);
    }
  }
}

void oled_animation_shoot_ball_sram(uint8_t line, uint8_t startHeightPlat, uint8_t* distanceFromStart, uint8_t* sign){

  if (*distanceFromStart <= (startHeightPlat + 1)){
    oled_write_platform_horizontal_sram(line, 0, *distanceFromStart);
    oled_animation_circle_horizontal_sram(line, distanceFromStart, sign);
  }
  //printf("Height: %d\n\r", height);
  //printf("Distance: %d\r\n", *distanceFromStart);
  else{
    oled_write_platform_horizontal_sram(line, 0, startHeightPlat);
    oled_animation_circle_horizontal_sram(line, distanceFromStart, sign);
  }
}

/* END OF SRAM USE HERE */

/* --------------------Timer interrupt ----------------------------------*/
// Uses the 16-bit module 1
void timer_interrupt_for_oled_init(){
  //Setting Timer/counter module 1 in Clear timer on Compare match mode (CTC).
  TCCR1B |= (1<<WGM12);
  //Clock prescaling, N = 8
  TCCR1B |= (1<<CS11);
  // Default value for duty cycle, 60Hz
  //OCR1AH was 0x0A
  OCR1AH = 0x0F;
  OCR1AL = 0xF3;
  // Clear counter
  TCNT1H = 0;
  TCNT1L = 0;
  //Enable interrupt out of port OCIE1A
  TIMSK |= (1<<OCIE1A);
}
